//
// Created by lee on 16-9-20.
//

#include "IOMessage.h"
#include <unistd.h>

namespace KQEvent {

    IOMessage::IOMessage(std::string const &msg) :
            _message(msg),
            _errorCode(0) {
        _curPos = _message.begin();
        _curSize = _message.size();

    }

    IOMessage::IOMessage(std::string &&msg) :
            _message(std::move(msg)),
            _errorCode(0) {
        _curPos = _message.begin();
        _curSize = _message.size();
    }

    IOMessage::IOMessage(char const *buf, size_t len) :
            _message(buf, buf + len),
            _errorCode(0) {
        _curPos = _message.begin();
        _curSize = _message.size();
    }

    AbstractMessage::statusE IOMessage::send(int fd) {
        if (_curSize == 0)
            return FINISHED;

        int n = 0;

        while ((n = ::write(fd, &(*_curPos), _curSize)) > 0) {
            _curSize -= n;
            _curPos += n;
        }

        bool isError = !(errno == EINTR ||
                         errno == EWOULDBLOCK || errno == EAGAIN);

        if (n < 0 && isError) {
            _errorCode = errno;
            return ERROR;
        }

        if (_curSize == 0)
            return FINISHED;

        return CONTINUE;
    }

    int IOMessage::getErrorCode() {
        return _errorCode;
    }

}