//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_EVENTLOOP_H
#define KQEVENT_EVENTLOOP_H

#include <thread>
#include <map>
#include "Subject.h"
#include "KQEventCommonException.h"
#include "Poller.h"
#include "TimerTaskQueue.h"

namespace KQEvent {
    class EventLoop : public std::enable_shared_from_this<EventLoop> {
    public:
        using TimerTaskQueuePtr = TimerTaskQueue::TimerTaskQueuePtr;
        using EventLoopPtr = std::shared_ptr<EventLoop>;

        template<typename ..._Args>
        static EventLoopPtr newInstance(_Args &&...args) {
            auto aNew = new EventLoop(std::forward<_Args>(args)...);
            return EventLoopPtr(aNew);
        }

        EventLoop(EventLoop const &) = delete;

        EventLoop const &operator=(EventLoop const &) = delete;

        void registerSubject(Subject::SubjectPtr const &);

        void unregisterSubject(int fd);

        virtual void loop();

        void exit() { _poller->exit(); }

        EventLoopPtr getPtr() { return shared_from_this(); }

        TimerTaskQueuePtr getTimerTaskQueue() { return _timerTaskQueue; }

    private:
        explicit EventLoop();

        void assertOwner() throw(KQEventCommonException);

        Poller::PollerPtr _poller;
        bool _looping;
        long _tid;

        TimerTaskQueuePtr _timerTaskQueue;
    };
}


#endif //KQEVENT_EVENTLOOP_H
