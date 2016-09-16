//
// Created by lee on 16-9-16.
//

#ifndef KQEVENT_ABSTRACTACCEPTOR_H
#define KQEVENT_ABSTRACTACCEPTOR_H

#include "Socket.h"
#include "Connection.h"

namespace KQEvent {
    /**
     *  @brief AbstractAcceptor
     *  Acceptor的职责是监听socket, 当连接事件到来时
     *  需要生成并且返回一个新的链接
    */
    class AbstractAcceptor {
    public:
        virtual Connection::ConnectionPtr acceptHandle() = 0;
        virtual ~AbstractAcceptor(){};
        //禁止拷贝
        AbstractAcceptor(AbstractAcceptor const&) = delete;
        AbstractAcceptor const&operator=(AbstractAcceptor const&) = delete;

    protected:
        AbstractAcceptor(Socket::SocketPtr socket);

    private:
        Socket::SocketPtr _socket;
    };
}

#endif //KQEVENT_ABSTRACTACCEPTOR_H
