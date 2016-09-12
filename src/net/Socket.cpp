#include "Socket.h"
#include "KQEventCommonException.h"
#include <string.h>
#include <linux/tcp.h>
#include "common.h"

namespace KQEvent {
    Socket::Socket(int domain, int type) {
        _fd = socket(domain, type, 0);
        if (_fd < 0) {
            char buf[512] = {0};
            strerror_r(errno, buf, sizeof(buf));
            throw KQEventCommonException(std::string(buf));
        }
        _address.reset();
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
        if (ret < 0){
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
        int ret = ::bind(_fd, (sockaddr*)(&sock), address->getSocketLen());
        if (ret < 0){
            __setErrorString(errno);
        }
        return ret;
    }

    int Socket::accept(IPAddress::IPAddressPtr &peerAddr) {
        ::sockaddr_in peeraddr;
        ::socklen_t len;
        int acceptFd = ::accept(_fd, (sockaddr*)(&peeraddr), &len);
        peerAddr = IPAddress::fromSockAddr(peeraddr);
        if (acceptFd < 0){
            __setErrorString(errno);
        }

        return acceptFd;
    }
}