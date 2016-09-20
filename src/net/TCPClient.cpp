//
// Created by lee on 16-9-19.
//

#include "TCPClient.h"
#include <iostream>

namespace KQEvent {

    TCPClient::TCPClient(std::string const &serverAddr,
                         const std::string &localAddr) :
            _maxRetry(5) {
        _connection.reset();
        _loop = EventLoop::newInstance();
        _connector = Connector::newInstance(_loop, serverAddr, localAddr);

        auto defaultHandler = [](TCPClient::ConnectionPtr conn) {};
        auto defaultReadHandler = [](char *, size_t) {};
        auto defaultCloseHandler = [](TCPClient::ConnectionPtr) {};

        auto defaultErrorHandler = [this](SocketPtr, int) {
            static int retryCnt = 0;
            if (++retryCnt == _maxRetry)
                exit(0);
            return Connector::RETRY;
        };

        auto defaultExceptHandler = [](TCPClient::ConnectionPtr conn) {
            auto info = TCPInfo::fromTCPSocketFd(conn->getFd());
            exit(0);
        };

        _onConnectedCallback = defaultHandler;
        _onReadCallback = defaultReadHandler;
        _onCloseCallback = defaultCloseHandler;
        _onConnectErrorCallBack = defaultErrorHandler;
        _onExceptCallback = defaultExceptHandler;

        auto onErrorHandler = std::bind(&TCPClient::__onError, this,
                                        std::placeholders::_1, std::placeholders::_2);
        _connector->setErrorHandler(onErrorHandler);
        auto onConnectedHandler = std::bind(&TCPClient::__onConnected,
                                            this, std::placeholders::_1);
        _connector->setSucessHandler(onConnectedHandler);

    }

    void TCPClient::__onConnected(TCPClient::ConnectionPtr conn) {
        _connection = conn;
        //attach
        auto closeHandler = std::bind(&TCPClient::__onClose,
                                      this, std::placeholders::_1);
        auto readHandler = std::bind(&TCPClient::__onRead, this, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3);
        auto exceptHandler = std::bind(&TCPClient::__onExcept,
                                       this, std::placeholders::_1);

        _connection->attachCloseHandler(closeHandler);
        _connection->attachReadHandler(readHandler);
        _connection->attachExceptHandler(exceptHandler);

        _onConnectedCallback(_connection);
        _connection->setConnected();
        _loop->registerSubject(_connection->getSubject());
    }

    void TCPClient::__onClose(TCPClient::ConnectionPtr conn) {
        _onCloseCallback(conn);
        _connection.reset();//断开连接
    }

    bool TCPClient::sendMsg(char *msg, size_t len) {
        if (!_connection) {
            return false;
        }
        _connection->sendMessage(msg, len);
        return true;
    }

    Connector::optionE TCPClient::__onError(TCPClient::SocketPtr socket, int err) {
        return _onConnectErrorCallBack(socket, err);
    }

    void TCPClient::__onExcept(TCPClient::ConnectionPtr conn) {
        _onExceptCallback(conn);
    }

    void TCPClient::__onRead(TCPClient::ConnectionPtr, char *buf, size_t len) {
        _onReadCallback(buf, len);
    }
}