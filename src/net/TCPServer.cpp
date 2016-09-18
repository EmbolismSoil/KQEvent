//
// Created by lee on 16-9-18.
//

#include "TCPServer.h"

namespace  KQEvent{

    TCPServer::TCPServer(std::string const &ip, int backlog) {
        _address = IPAddress::fromIPAddress(ip);
        _socket = Socket::newInstance();
        _socket->bind(_address);
        _socket->listen(backlog);//非阻塞
        _acceptor = AbstractAcceptor::newInstance(_socket);
        auto onConn = std::bind(&TCPServer::onNewConnection, this, std::placeholders::_1);
        _acceptor->setOnConnectHandle(onConn);

        _loop = EventLoop::newInstance();
        _loop->registerSubject(_acceptor->getSubject());

        auto defHandler = [](ConnectionPtr unused){};
        _connNewHandler = defHandler;
        _connExceptHandler = defHandler;
        _connCloseHandler = defHandler;
        _connReadHandler = [](ConnectionPtr conn, char *buf, size_t len){};
    }

    Connection::Handle_t TCPServer::_connHandlerWrap(TCPServer::Handle_t handle) {
        auto tmp = [handle](ConnectionPtr conn){
            handle(conn);
            return Observer::ALIVE;
        };
        return  tmp;
    }

    void TCPServer::onNewConnection(TCPServer::ConnectionPtr conn) {
        auto tmp = std::bind(&TCPServer::onReadHandler, this, std::placeholders::_1);
        conn->attachReadHandler(tmp);
        conn->attachExceptHandler(_connHandlerWrap(_connExceptHandler));
        conn->setConnected();
        _connectionPool.push_back(conn);
        _loop->registerSubject(conn->getSubject());
        _connNewHandler(conn);
    }

    Observer::Command_t TCPServer::onReadHandler(TCPServer::ConnectionPtr conn) {
        char buf[32768];
        int n = ::read(conn->getFd(), buf, sizeof(buf));
        if (n == 0){
            _closeConnection(conn);
            return Observer::ALIVE;
        }
        _connReadHandler(conn, buf, n);
        if (conn->getStatus() ==
                Connection::Disconnecting){
            conn->setDisconnected();
            _closeConnection(conn);
        }
        return Observer::ALIVE;
    }

    void TCPServer::_closeConnection(TCPServer::ConnectionPtr conn) {
        _connCloseHandler(conn);
        auto pos = std::find(_connectionPool.begin(),
                             _connectionPool.end(), conn);
        if (pos == _connectionPool.end()){
            //处理故障，打印log。目前log和全局故障尚未完成，只能简单退出
            return;
        }
        //连接的生命周期结束
        _connectionPool.erase(pos);
    }
}