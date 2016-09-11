//
// Created by lee on 16-9-11.
//

#ifndef KQEVENT_KQEVENTCOMMONEXCEPTION_H
#define KQEVENT_KQEVENTCOMMONEXCEPTION_H

#include <exception>
#include <string>

namespace KQEvent {
    class KQEventCommonException : public std::exception {
    public:
        KQEventCommonException(std::string const &message) :
                _message(message) {

        }

        const char *what() const override {
            return _message.c_str();
        }

    private:
        std::string _message;
    };
}

#endif //KQEVENT_KQEVENTCOMMONEXCEPTION_H
