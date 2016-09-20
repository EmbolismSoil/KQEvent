//
// Created by lee on 16-9-20.
//

#ifndef KQEVENT_IOMESSAGE_H
#define KQEVENT_IOMESSAGE_H

#include "AbstractMessage.h"
namespace KQEvent {
    class IOMessage : public AbstractMessage{
    public:
        using IOMessagePtr = std::shared_ptr<IOMessage>;

        virtual statusE send(int d) override ;
        virtual int getErrorCode() override ;

        IOMessage(IOMessage const &) = delete;
        IOMessage const &operator=(IOMessage const&) = delete;

        template <typename ..._Args>
        static IOMessagePtr newInstance(_Args&& ...args){
            auto aNew = new IOMessage(std::forward<_Args>(args)...);
            return IOMessagePtr(aNew);
        }

    private:
        IOMessage(std::string const &msg);
        IOMessage(std::string &&msg);
        IOMessage(char const *buf, size_t len);

        std::string _message;
        std::string::iterator _curPos;
        size_t _curSize;
        int _errorCode;
    };
}



#endif //KQEVENT_IOMESSAGE_H
