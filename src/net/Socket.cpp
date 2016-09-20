#include "Socket.h"
#include "KQEventCommonException.h"
#include <string.h>
#include <linux/tcp.h>
#include "common.h"
#include <fcntl.h>
#include <chrono>

namespace KQEvent {
    Socket::Socket(int domain, int type) {
        _fd = ::socket(domain, type | SOCK_CLOEXEC, 0);
        if (_fd < 0) {
            char buf[512] = {0};
            ::strerror_r(errno, buf, sizeof(buf));
            throw KQEventCommonException(std::string(buf));
        }
        int flag = ::fcntl(_fd, F_GETFL, 0);
        ::fcntl(_fd, F_SETFL, flag | O_NONBLOCK);
    }

    Socket::Socket() : Socket(AF_INET, SOCK_STREAM) {

    }

    Socket::Socket(int fd) :
            _fd(fd) {
        int flag = ::fcntl(_fd, F_GETFL, 0);
        ::fcntl(_fd, F_SETFL, flag | O_NONBLOCK);
    }

    int Socket::setNoDelay(bool on) {
        int opt = on;
        int ret = ::setsockopt(_fd, IPPROTO_TCP,
                               TCP_NODELAY, &opt, sizeof(opt));
        if (ret < 0) {
            __setErrorString(errno);
        }

        return ret;
    }

    int Socket::setReuseAddr(bool on) {
        int opt = on;
        int ret = ::setsockopt(_fd, SOL_SOCKET,
                               SO_REUSEADDR, &opt, sizeof(opt));
        if (ret < 0) {
            __setErrorString(errno);
        }

        return ret;
    }

    int Socket::setKeepAlive(bool on) {
        int opt = on;
        int ret = ::setsockopt(_fd, SOL_SOCKET,
                               SO_KEEPALIVE, &opt, sizeof(opt));
        if (ret < 0) {
            __setErrorString(errno);
        }

        return ret;
    }

    void Socket::__setErrorString(int err) {
        char buf[__ERROR_STR_LEN] = {0};
        ::strerror_r(err, buf, sizeof(buf));
        _messageError = std::string(buf);
    }

    int Socket::bind(IPAddress::IPAddressPtr address) {
        auto sock = address->getSockAddr();
        int ret = ::bind(_fd, (sockaddr *) (&sock), address->getSocketLen());
        if (ret < 0) {
            __setErrorString(errno);
        }
        return ret;
    }

    int Socket::connect(IPAddress::IPAddressPtr const &serverAddr) {
        auto sock = serverAddr->getSockAddr();
        int ret = ::connect(_fd, (sockaddr *) (&sock), serverAddr->getSocketLen());
        if (ret < 0) {
            __setErrorString(errno);
        }
        return ret;
    }

    int Socket::accept(IPAddress::IPAddressPtr &peerAddr) {
        ::sockaddr_in peeraddr;
        ::socklen_t len = sizeof(peeraddr);
        int acceptFd = ::accept(_fd, (sockaddr *) (&peeraddr), &len);
        peerAddr = IPAddress::fromSockAddr(peeraddr);
        if (acceptFd < 0) {
            __setErrorString(errno);
        }

        return acceptFd;
    }

    int Socket::accept4(IPAddress::IPAddressPtr &peerAddr) {
        ::sockaddr_in peeraddr;
        ::socklen_t len = sizeof(peeraddr);
        int acceptFd = ::accept4(_fd, (sockaddr *) (&peeraddr), &len,
                                 SOCK_CLOEXEC | SOCK_NONBLOCK);
        peerAddr = IPAddress::fromSockAddr(peeraddr);
        if (acceptFd < 0) {
            __setErrorString(errno);
        }

        return acceptFd;
    }

    IPAddress::IPAddressPtr Socket::getIPAddress(void) {
        ::sockaddr_in sockAddress;
        ::socklen_t len;
        ::getsockname(_fd, (sockaddr *) (&sockAddress), &len);
        return IPAddress::fromSockAddr(sockAddress);
    }

    int Socket::listen(int backlog) {
        return ::listen(_fd, backlog);
    }

    int Socket::getSocketError(void) {
        int ret;
        socklen_t len = static_cast<socklen_t>(sizeof ret);
        int err = ::getsockopt(_fd, SOL_SOCKET, SO_ERROR, &ret, &len);
        if (err < 0) {
            return errno;
        } else {
            return ret;
        }
    }

    IPAddress::IPAddressPtr Socket::getPeerAddr() {
        ::sockaddr_in peer;
        ::socklen_t len = sizeof(peer);
        ::getpeername(_fd, (::sockaddr *) (&peer), &len);
        return IPAddress::fromSockAddr(peer);
    }
}