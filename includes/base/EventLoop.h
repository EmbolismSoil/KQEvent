//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_EVENTLOOP_H
#define KQEVENT_EVENTLOOP_H

#include <thread>
#include <map>
#include "Subject.h"
#include "KQEventCommonException.h"

namespace KQEvent {
    class EventLoop {
    public:

        EventLoop();

        EventLoop(EventLoop const &) = delete;

        EventLoop const &operator=(EventLoop const &) = delete;

        void registerSubject(Subject::SubjectPtr);

        Subject::SubjectPtr  unregisterSubject(int fd);
        Subject::SubjectPtr  unregisterSubject(Subject::SubjectPtr subject);

        virtual void loop();

    private:
        void assertOwner() throw(KQEventCommonException);
        bool _looping;
        long _tid;
        std::map<int, Subject::SubjectPtr> _subjects;
    };
}


#endif //KQEVENT_EVENTLOOP_H
