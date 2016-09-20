//
// Created by lee on 16-9-20.
//

#ifndef KQEVENT_SENDFILEMESSAGE_H
#define KQEVENT_SENDFILEMESSAGE_H

#include "AbstractMessage.h"

namespace KQEvent{
    class SendfileMessage : public AbstractMessage{
    public:
        using SendfileMessagePtr = std::shared_ptr<SendfileMessage>;

        virtual ~SendfileMessage();
        virtual statusE send(int d) override ;
        virtual int getErrorCode() override ;

        SendfileMessage(SendfileMessage const &) = delete;
        SendfileMessage const &operator=(SendfileMessage const &) = delete;

        template <typename ..._Args>
        static SendfileMessagePtr newInstance(_Args&& ...args){
            auto aNew = new SendfileMessage(std::forward<_Args>(args)...);
            return SendfileMessagePtr(aNew);
        }

    private:

        int _fd;
        __off_t _size;
        int _errorCode;
        SendfileMessage(int fd);

    };
}



#endif //KQEVENT_SENDFILEMESSAGE_H
