//
// Created by lee on 16-9-16.
//

#include "TimerQueue.h"
#include <algorithm>
#include <unistd.h>

namespace KQEvent {

    void TimerQueue::addTimer(Timer::TimerPtr timer) {
        _timers[timer] = __fakeValue;
        _updateTimeoutPoint();
    }

    void TimerQueue::delTimer(Timer::TimerPtr timer) {
        _timers.erase(timer);
        _updateTimeoutPoint();
    }

    void TimerQueue::handleTimeout() {
        if (_timers.empty()) {
            uint64_t exp;
            ssize_t s = read(_timefd, &exp, sizeof(uint64_t));
            return;
        }

        auto now = Timer::Clock::now();
        auto pos = _timers.begin();
        std::vector<Timer::TimerPtr> ret;

        for (; pos != _timers.end(); ++pos) {
            if (pos->first->getTimeoutPoint() > now)
                break;
            ret.push_back(pos->first);
        }

        _timers.erase(_timers.begin(), pos);
        /*
         * 这里要分开为两次迭代操作，防止handle执行时间太长造成定时不准
         * 这里要从定时器先超时的开始执行
         * */
        for (auto pos = ret.rbegin(); pos != ret.rend(); ++pos) {
            (*pos)->handle();
        }

        for (auto &item : ret) {
            if (item->getPeriod() > Timer::Milliseconds(0)) {
                auto now = Timer::Clock::now();
                item->setTimoutAt(now + item->getPeriod());
                _timers[item] = __fakeValue;
            }
        }
        uint64_t exp;
        ssize_t s = read(_timefd, &exp, sizeof(uint64_t));
        _updateTimeoutPoint();
    }

    TimerQueue::TimerQueue() throw(KQEventCommonException) {
        _timefd = timerfd_create(CLOCK_REALTIME, 0);
        if (_timefd < 0)
            throw KQEventCommonException("failed in  TimerQueue::TimerQueue()");

        _subject = Subject::newInstance(_timefd);
        _observer = Observer::newInstance();
        _observer->setHandle([this](Subject::SubjectPtr subject) {
            handleTimeout();
            return Observer::ALIVE;
        });
        _subject->attachReadObserver(_observer);
        _subject->setReadEvent(true);
    }

    int const TimerQueue::getTimerfd() {
        return _timefd;
    }

    void TimerQueue::_updateTimeoutPoint() {
        if (_timers.empty())
            return;

        auto timer = _timers.begin()->first;
        auto timePoint = timer->getTimeoutPoint().time_since_epoch();
        auto ms = std::chrono::duration_cast<Timer::Milliseconds>(timePoint).count();

        ::itimerspec time = {
                .it_interval = {0, 0},
                .it_value = {ms / 1000, (ms % 1000) * 1000000}
        };

        ::timerfd_settime(_timefd, TFD_TIMER_ABSTIME, &time, NULL);
    }

    TimerQueue::~TimerQueue() {
        ::close(_timefd);
    }

}