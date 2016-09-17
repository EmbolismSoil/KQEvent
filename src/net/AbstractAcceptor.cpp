//
// Created by lee on 16-9-16.
//

#include "AbstractAcceptor.h"

namespace KQEvent{

    void AbstractAcceptor::setOnConnectHandle(const AbstractAcceptor::Handle_t &handle){
        _onConnect = handle;
    }

    AbstractAcceptor::AbstractAcceptor(Socket::SocketPtr socket):
        _socket(socket)
    {
        _subject = Subject::newInstance(_socket->getFd());
        auto observer = Observer::newInstance();
        observer->setHandle([this](Subject::SubjectPtr unused){
            handleReadEvent();
            return Observer::ALIVE;
        });
    }

    void AbstractAcceptor::handleReadEvent() {
        auto peerAddr = IPAddress::IPAddressPtr();
        peerAddr.reset();
        auto connfd = _socket->accept(peerAddr);
        if (connfd < 0)
            return;
        auto connection = Connection::newInstance(connfd, peerAddr);
        _onConnect(connection);
    }
}