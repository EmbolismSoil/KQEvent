//
// Created by lee on 16-9-12.
//

#include "TCPInfo.h"
#include <netinet/tcp.h>
#include "KQEventCommonException.h"
#include <common.h>

namespace KQEvent {
    const TCPInfo::TCPInfoPtr TCPInfo::fromTCPSocketFd(int fd)
                                    throw(KQEventCommonException){
        ::socklen_t len = 0;
        auto aNew = new TCPInfo();
        auto info = TCPInfoPtr(aNew);

        int ret =  ::getsockopt(fd, SOL_TCP, TCP_INFO,
                                (tcp_info*)(info.get()), &len);
        if (ret < 0) {
            std::string err = errno2String(errno);
            throw (KQEventCommonException(err));
        }

        return info;
    }

    std::string const &TCPInfo::toString(){
        return _infoString;
    }
}
