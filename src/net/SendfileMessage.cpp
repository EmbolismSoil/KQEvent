//
// Created by lee on 16-9-20.
//

#include "SendfileMessage.h"
#include <sys/stat.h>
#include <sys/sendfile.h>

namespace  KQEvent{

    SendfileMessage::SendfileMessage(int fd) :
            _fd(fd), _errorCode(0){
        struct stat statbuff;
        ::fstat(fd, &statbuff);
        _size = statbuff.st_size;
    }

    AbstractMessage::statusE SendfileMessage::send(int fd) {
        int n = 0;
        while ((n = ::sendfile(fd, _fd, NULL, _size)) > 0) {
            _size -= n;
        }

        if (n < 0 && (errno == EAGAIN ||
                        errno == EWOULDBLOCK)) {
            return CONTINUE;
        }else if (n == 0) {
            return FINISHED;
        }else{
            _errorCode = errno;
            return ERROR;
        }
    }

    int SendfileMessage::getErrorCode() {
        return _errorCode;
    }
}