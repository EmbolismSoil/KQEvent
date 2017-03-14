//
// Created by lee on 16-9-17.
//

#ifndef KQEVENT_TIMERTASKQUEUE_H
#define KQEVENT_TIMERTASKQUEUE_H

#include "TimerQueue.h"
#include "Subject.h"
#include <mutex>

namespace KQEvent {
    class TimerTaskQueue {
    public:
        using TimerTaskQueuePtr = std::shared_ptr<TimerTaskQueue>;
        using Task = std::function<void(void)>;
        using TaskList = std::vector<Task>;

        virtual ~TimerTaskQueue();

        TimerTaskQueue(TimerTaskQueue const &) = delete;

        TimerTaskQueue const &operator=(TimerTaskQueue const &) = delete;

        template<typename ..._Args>
        static TimerTaskQueuePtr newInstance(_Args &&...args) {
            auto aNew = new TimerTaskQueue(std::forward<_Args>(args)...);
            return TimerTaskQueuePtr(aNew);
        }

        virtual void runTask(Task const &);

        virtual void runTaskAt(Task const &, Timer::TimePoint);

        virtual void runTaskAfter(Task const &, Timer::Milliseconds);

        virtual void runTaskEvery(Task const &task, Timer::Milliseconds period);

        virtual void wakeUp();

        Subject::SubjectPtr const &getTimerQueueSubect();

        Subject::SubjectPtr const &getEventfdSubject();

    protected:
        explicit TimerTaskQueue();
        // TimerTaskQueue(EventLoop::EventLoopPtr loop);

    private:
        Subject::SubjectPtr _eventTaskSubject;
        Observer::ObserverPtr _eventObserver;
        TimerQueue::TimerQueuePtr _timerQueue;
        TaskList _taskList;
        int _eventfd;
        std::mutex _taskListMutex;
    };
}
#endif //KQEVENT_TIMERTASKQUEUE_H
