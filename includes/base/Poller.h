//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_POLLER_H
#define KQEVENT_POLLER_H

#include <functional>
#include <poll.h>
#include <memory>
#include <vector>
#include "Subject.h"
#include <map>

namespace KQEvent {
    class Poller : public std::enable_shared_from_this<Poller> {
    public:
        using PollerPtr = std::shared_ptr<Poller>;
        using Handle_t =  std::function<void(PollerPtr)>;
        template <typename  ..._Args>
        static PollerPtr newInstance(_Args&& ...args){
            auto aNew = new Poller(std::forward<_Args>(args)...);
            return PollerPtr(aNew);
        }

        Poller(Poller const &) = delete;

        Poller const &operator=(Poller const &) = delete;

        virtual void addToPoll(Subject::SubjectPtr const &subject);

        virtual void removeFromPoll(int fd);

        virtual void poll();

        PollerPtr getPtr();

        void exit();

        void setTimeout(int timeout);

    private:
        Poller(Handle_t hanle, int timeout = 60000);

        bool _exit;
        int _timeout;
        std::vector<::pollfd> _pollfds;
        std::map<int, Subject::SubjectPtr> _subjects;
        int _retValue;
        Handle_t _handle; //处理非正常poll返回
    };
}

#endif //KQEVENT_POLLER_H
