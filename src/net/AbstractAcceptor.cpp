//
// Created by lee on 16-9-16.
//

#include "AbstractAcceptor.h"
#include <iostream>

namespace KQEvent {

    void AbstractAcceptor::setOnConnectHandle(const AbstractAcceptor::Handle_t &handle) {
        _onConnect = handle;
    }

    AbstractAcceptor::AbstractAcceptor(Socket::SocketPtr socket) :
            _socket(socket) {
        _subject = Subject::newInstance(_socket->getFd());
        _observer = Observer::newInstance();
        _observer->setHandle([this](Subject::SubjectPtr unused) {
            handleReadEvent();
            return Observer::ALIVE;
        });

        _subject->attachReadObserver(_observer);
        _socket->listen();
    }

    void AbstractAcceptor::handleReadEvent() {
        auto peerAddr = IPAddress::IPAddressPtr();
        peerAddr.reset();
        auto connfd = _socket->accept4(peerAddr);
        if (connfd < 0)
            return;

        Socket::SocketPtr socket = Socket::newInstance(connfd);
        auto connection = Connection::newInstance(std::move(socket), peerAddr);

        {
            _onConnect(std::move(connection));
        }

        return;
    }

    Subject::SubjectPtr const &AbstractAcceptor::getSubject() {
        return _subject;
    }
}