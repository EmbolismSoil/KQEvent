//
// Created by lee on 16-9-19.
//

#include "Connector.h"

namespace KQEvent {

    void Connector::setSucessHandler(Connector::SuccessHandle_t handler) {
        _onConnectedHanle = handler;
    }

    void Connector::setErrorHandler(Connector::ErrorHandle_t handler) {
        _onError = handler;
    }

    Observer::Command_t Connector::_handleWrite(Connector::SubjectPtr sub) {
        //查看是错误还是建立成功
        int err = _socket->getSocketError();
        if (err == EINPROGRESS) {
            return Observer::ALIVE;
        } else if (err == 0) {
            auto peer = _socket->getPeerAddr();
            //connection会对socket注册事件监听，这样connector对socket的事件监听会被覆盖掉
            auto conn = Connection::newInstance(_socket, peer);
            _onConnectedHanle(conn);
            _socket.reset();
            _isConnected = true;
        } else {
            //Connector出错的时候不会主动进行重试，因为要不要重试
            //是用户决定的,如果用户不想继续重试了，可以在_onError里面
            //结束Connector的生命周期。
            if (_onError(_socket, err) == RETRY) {
                __retry();
            }
        }

        return Observer::ALIVE;
    }

    Connector::Connector(EventLoopPtr loop,
                         std::string const &serverAddr,
                         std::string const &localAddr,
                         int maxRetry) :
            _loop(loop),
            _isConnected(false),
            _retryCount(0),
            _maxRetry(maxRetry) {
        _socket = Socket::newInstance();
        _serverAddr = IPAddress::fromIPAddress(serverAddr);
        if (localAddr.empty()) {
            _localAddr = IPAddress::addrAny();
        } else {
            _localAddr = IPAddress::fromIPAddress(localAddr);
        }
        _socket->bind(_localAddr);
        _writeObserver = Observer::newInstance();
        _writeSubject = Subject::newInstance(_socket->getFd());
        auto tmp = std::bind(&Connector::_handleWrite, this, std::placeholders::_1);
        _writeObserver->setHandle(tmp);
        _writeSubject->attachWriteObserver(_writeObserver);
        _socket->connect(_serverAddr);
        auto defaultErrorHandler = [](Socket::SocketPtr, int) {
            //打印log
            return RETRY;
        };
        loop->registerSubject(getSubject());
        _onError = defaultErrorHandler;
    }

    void Connector::connect() {
        if (!_socket) {
            _socket = Socket::newInstance();
        }

        _socket->connect(_serverAddr);
    }

    void Connector::__retry() {
        ++_retryCount;
        //旧socket生命周期结束，自动关闭连接
        _socket = Socket::newInstance();
        _socket->bind(_localAddr);
        //旧subject生命周期结束，取消对旧socket的事件监听
        _writeSubject = Subject::newInstance(_socket->getFd());
        _writeSubject->attachWriteObserver(_writeObserver);
        auto delayUint = Timer::Milliseconds(10000);
        auto taskQueue = _loop->getTimerTaskQueue();
        auto retryHandle = [this]() {
            _loop->registerSubject(_writeSubject);
            _socket->connect(_serverAddr);
        };
        taskQueue->runTaskAfter(retryHandle, _retryCount * delayUint);
    }

}