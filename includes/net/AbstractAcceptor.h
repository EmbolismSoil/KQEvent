//
// Created by lee on 16-9-16.
//

#ifndef KQEVENT_ABSTRACTACCEPTOR_H
#define KQEVENT_ABSTRACTACCEPTOR_H

#include "Socket.h"
#include "Connection.h"
#include "Subject.h"

namespace KQEvent {
    /**
     *  @brief AbstractAcceptor
     *  Acceptor的职责是监听socket, 当连接事件到来时
     *  需要生成并且返回一个新的链接
    */
    class AbstractAcceptor {
    public:
        using Handle_t = std::function<void(Connection::ConnectionPtr)>;
        using AbstractAcceptorPtr = std::shared_ptr<AbstractAcceptor>;
        virtual ~AbstractAcceptor(){};

        //禁止拷贝
        AbstractAcceptor(AbstractAcceptor const&) = delete;
        AbstractAcceptor const&operator=(AbstractAcceptor const&) = delete;

        void setOnConnectHandle(Handle_t const &);
        Subject::SubjectPtr const &getSubject();

        template <typename ..._Args>
                static AbstractAcceptorPtr newInstance(_Args&& ...args){
            auto aNew = new AbstractAcceptor(std::forward<_Args>(args)...);
            return AbstractAcceptorPtr(aNew);
        }

    protected:
        explicit AbstractAcceptor(Socket::SocketPtr socket);

    private:
        void handleReadEvent();
        Socket::SocketPtr _socket;
        Subject::SubjectPtr _subject;
        Handle_t _onConnect;
        Observer::ObserverPtr _observer;
    };
}

#endif //KQEVENT_ABSTRACTACCEPTOR_H
