//
// Created by lee on 16-9-18.
//

#ifndef KQEVENT_TCPSERVER_H
#define KQEVENT_TCPSERVER_H

#include "Connection.h"
#include "EventLoop.h"
#include "AbstractAcceptor.h"
#include "Socket.h"
#include <list>
#include "IPAddress.h"

namespace KQEvent{
    class TCPServer {
    public:
        using TCPServerPtr = std::shared_ptr<TCPServer>;
        using ConnectionPtr = Connection::ConnectionPtr;
        using AcceptorPtr = AbstractAcceptor::AbstractAcceptorPtr;
        using EventLoopPtr = EventLoop::EventLoopPtr ;
        using SocketPtr = Socket::SocketPtr ;
        using ConnectionsPool = std::list<ConnectionPtr >;
        using IPAddressPtr = IPAddress::IPAddressPtr ;
        using Handle_t = AbstractAcceptor::Handle_t ;
        using ReadHandle_t = std::function<void(ConnectionPtr, char *, size_t)>;

        TCPServer(TCPServer const &) = delete;
        TCPServer const &operator=(TCPServer const &) = delete;

        template <typename ..._Args>
        static TCPServerPtr newInstance(_Args&& ...args){
            auto aNew = new TCPServer(std::forward<_Args>(args)...);
            return TCPServerPtr(aNew);
        }

        void setConnectionNewHandler(Handle_t handle){
            _connNewHandler = handle;
        }

        void setConnectionReadHandler(ReadHandle_t handle){
            _connReadHandler = handle;
        }

        void setConnectionExceptHandler(Handle_t handle){
            _connExceptHandler = handle;
        }

        void setConnectionCloseHandler(Handle_t handle){
            _connCloseHandler = handle;
        }

        void run(){
            _loop->loop();
        }
    private:
        void onNewConnection(ConnectionPtr conn);
        Observer::Command_t onReadHandler(ConnectionPtr conn);

        Connection::Handle_t _connHandlerWrap(Handle_t handle);

        ConnectionsPool _connectionPool;
        Handle_t _connNewHandler;
        ReadHandle_t _connReadHandler;
        Handle_t _connExceptHandler;
        Handle_t _connCloseHandler;

        IPAddressPtr _address;
        SocketPtr  _socket;
        AcceptorPtr _acceptor;
        EventLoopPtr _loop;

        explicit TCPServer(std::string const &ip, int backlog = 65535);

        void _closeConnection(TCPServer::ConnectionPtr conn);
    };
}



#endif //KQEVENT_TCPSERVER_H
