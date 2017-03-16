//
// Created by lee on 16-9-18.
//

#include "TCPServer.h"
#include <thread>
#include <sstream>

namespace KQEvent {

    TCPServer::TCPServer(std::string const &ip,int numberOfWorkers, int backlog):
            _numberOfWorkers(numberOfWorkers),
            _indexOfCurrentWorker(0)
    {
        _address = IPAddress::fromIPAddress(ip);
        _socket = Socket::newInstance();
        _socket->bind(_address);
        _socket->listen(backlog);//非阻塞
        _acceptor = AbstractAcceptor::newInstance(_socket);
        auto onConn = std::bind(&TCPServer::onNewConnection, this, std::placeholders::_1);
        _acceptor->setOnConnectHandle(onConn);

        _loop = EventLoop::newInstance();
        _loop->registerSubject(_acceptor->getSubject());

        auto defHandler = [](ConnectionPtr unused) {};
        _connNewHandler = defHandler;
        _connExceptHandler = defHandler;
        _connCloseHandler = defHandler;
        _connReadHandler = [](ConnectionPtr conn, char *buf, size_t len) {};

        std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;

        if (_numberOfWorkers == -1){
            _numberOfWorkers = std::thread::hardware_concurrency();
            for (int cnt = 0; cnt < _numberOfWorkers; ++cnt){
                std::stringstream s;
                s << "Thread_" << cnt;
                auto worker = BussinessWorker::newInstance(s.str());
                _bussinessWorkers.push_back(worker);
                std::cout << s.str() << " id = " << worker->getId() << std::endl;
            }
        }

    }

    Connection::Handle_t TCPServer::_connHandlerWrap(TCPServer::Handle_t handle) {
        auto tmp = [handle](ConnectionPtr conn) {
            handle(conn);
            return Observer::ALIVE;
        };
        return tmp;
    }

    void TCPServer::onNewConnection(TCPServer::ConnectionPtr conn) {
        _connNewHandler(conn);
        if (_numberOfWorkers == 0){
            auto tmp = std::bind(&TCPServer::onReadHandler, this,
                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            conn->attachReadHandler(tmp);
            conn->attachExceptHandler(_connHandlerWrap(_connExceptHandler));
            conn->setConnected();
            auto closeHandler = std::bind(&TCPServer::connCloseHandler,
                                          this, std::placeholders::_1);
            conn->attachCloseHandler(closeHandler);
            _connectionPool.push_back(conn);
            _loop->registerSubject(conn->getSubject());
        }else{
            dispatchConntion(std::move(conn));
        }
    }

    void TCPServer::onReadHandler(TCPServer::ConnectionPtr conn,
                                  char *buf, size_t n) {
        _connReadHandler(conn, buf, n);
    }

    void TCPServer::_closeConnection(TCPServer::ConnectionPtr conn) {
        auto pos = std::find(_connectionPool.begin(),
                             _connectionPool.end(), conn);
        if (pos == _connectionPool.end()) {
            //处理故障，打印log。目前log和全局故障尚未完成，只能简单退出
            return;
        }
        //连接的生命周期结束
        _connCloseHandler(conn);
        _connectionPool.erase(pos);
    }

    void TCPServer::connCloseHandler(TCPServer::ConnectionPtr conn) {
        _closeConnection(conn);
    }

    void TCPServer::dispatchConntion(TCPServer::ConnectionPtr conn) {
        auto worker = _bussinessWorkers[_indexOfCurrentWorker++];
        _indexOfCurrentWorker %= _numberOfWorkers;
        {
            std::lock_guard<std::mutex> guard(worker->getLoopMutex());
            worker->pushConnection(std::move(conn));
        }
    }
}