#include "Acceptor.h"
#include "KQEventCommonException.h"
#include "Connection.h"
#include <string.h>
#include <fcntl.h>

namespace KQEvent {
    Acceptor::Acceptor(Socket::SocketPtr socket, IPAddressPtr addrress)
    : AbstractAcceptor(socket), socket(_socket), address(_address)
    {
        bool on = true;
        _socket->setReuseAddr(on);
        _socket->bind(address);
        _socket->listen(LISTENQ);   // LISTENQ isn't defined
    }

    Connection::ConnectionPtr Acceptor::acceptHandle() {
        int acceptFd = socket->accept(nullptr);
        if (acceptFd < 0) {
            char buf[512] = {0};
            ::strerror_r(errno, buf, sizeof(buf));
            throw KQEventCommonException(std::string(buf));
        } 
        int flag = ::fcntl(acceptFd, F_GETFL, 0);
        ::fcntl(acceptFd, F_SETFL, flag | O_NONBLOCK);
        return Connection::newInstance(acceptFd);
    }
}
