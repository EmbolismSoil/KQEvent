#include "Socket.h"
#include "KQEventCommonException.h"
#include <string.h>

namespace KQEvent{
    Socket::Socket(int domain, int type){
       _fd = socket(domain, type, 0);
        if (_fd < 0){
            char buf[512] = {0};
            strerror_r(errno, buf, sizeof(buf));
            throw  KQEventCommonException(std::string(buf));
        }
    }
}