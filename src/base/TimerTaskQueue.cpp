//
// Created by lee on 16-9-17.
//

#include "TimerTaskQueue.h"
#include <sys/eventfd.h>
#include <unistd.h>

namespace KQEvent {
    TimerTaskQueue::TimerTaskQueue() {
        _eventfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        _eventTaskSubject = Subject::newInstance(_eventfd);
        _timerQueue = TimerQueue::newInstance();
        _eventObserver = Observer::newInstance();
        _eventObserver->setHandle([this](Subject::SubjectPtr subject) {
            uint64_t tmp;
            ::read(subject->getFd(), &tmp, sizeof(tmp));
            if (_taskList.empty())
                return Observer::ALIVE;
            auto pos = _taskList.begin();
            while (pos != _taskList.end()) {
                (*pos)();
                pos = _taskList.erase(pos);
            }
            return Observer::ALIVE;
        });
        _eventTaskSubject->attachReadObserver(_eventObserver);
    }

    TimerTaskQueue::~TimerTaskQueue() {

    }

    void TimerTaskQueue::runTask(const TimerTaskQueue::Task &task) {
        _taskList.push_back(task);
        uint64_t tmp = 10;
        ::write(_eventfd, &tmp, sizeof(tmp));
    }

    void TimerTaskQueue::runTaskAt(const TimerTaskQueue::Task &task,
                                   Timer::TimePoint tp) {
        auto timer = Timer::newInstance(tp, Timer::Milliseconds(0), task);
        _timerQueue->addTimer(timer);
    }

    void TimerTaskQueue::runTaskAfter(const TimerTaskQueue::Task &task,
                                      Timer::Milliseconds ms) {
        auto tp = Timer::Clock::now() + ms;
        runTaskAt(task, tp);
    }

    void TimerTaskQueue::runTaskEvery(const TimerTaskQueue::Task &task,
                                      Timer::Milliseconds period) {
        auto tp = Timer::Clock::now() + period;
        auto timer = Timer::newInstance(tp, period, task);
        _timerQueue->addTimer(timer);
    }

    Subject::SubjectPtr const &TimerTaskQueue::getTimerQueueSubect() {
        return _timerQueue->getSubject();
    }

    Subject::SubjectPtr const &TimerTaskQueue::getEventfdSubject() {
        return _eventTaskSubject;
    }

#if 0
    TimerTaskQueue::TimerTaskQueue(EventLoop::EventLoopPtr loop):
        TimerTaskQueue()
    {
        loop->registerSubject(_timerQueue->getSubject());
        loop->registerSubject(_eventTaskSubject);
    }
#endif
}


