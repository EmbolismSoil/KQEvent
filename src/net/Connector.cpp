//
// Created by lee on 16-9-19.
//

#include "Connector.h"

namespace KQEvent{

    void Connector::setSucessHandler(Connector::SuccessHandle_t handler) {
        _onConnectedHanle = handler;
    }

    void Connector::setErrorHandler(Connector::ErrorHandle_t handler) {
        _onError = handler;
    }

    Observer::Command_t Connector::_handleWrite(Connector::SubjectPtr sub) {
        //查看是错误还是建立成功
        int err = _socket->getSocketError();
        //如果是错误，则回调用户的错误处理

        if (err == EINPROGRESS){
            return Observer::ALIVE;
        }else if (err == 0){
            auto peer = _socket->getPeerAddr();
            //connection会对socket注册事件监听，这样connector对socket的事件监听会被覆盖掉
            auto conn = Connection::newInstance(_socket, peer);
            _onConnectedHanle(conn);
            _socket.reset();
        }else{
            _onError(_socket, err);
        }

        return  Observer::ALIVE;
    }

    Connector::Connector(std::string const& serverAddr)
    {
        _serverAddr = IPAddress::fromIPAddress(serverAddr);
        _writeObserver = Observer::newInstance();
        _socket = Socket::newInstance();
        _writeSubject = Subject::newInstance(_socket->getFd());
        auto tmp = std::bind(&Connector::_handleWrite, this, std::placeholders::_1);
        _writeObserver->setHandle(tmp);
        _writeSubject->attachWriteObserver(_writeObserver);
        _socket->connect(_serverAddr);
    }

    void Connector::connect() {
        if(!_socket){
            _socket = Socket::newInstance();
        }

        _socket->connect(_serverAddr);
    }

}