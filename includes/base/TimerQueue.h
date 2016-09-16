//
// Created by lee on 16-9-16.
//

#ifndef KQEVENT_TIMERQUEUE_H
#define KQEVENT_TIMERQUEUE_H

#include <map>
#include "Timer.h"
#include <sys/timerfd.h>
#include <time.h>
#include "KQEventCommonException.h"
#include "Subject.h"
#include "Observer.h"

namespace KQEvent{
    class TimerQueue {
        /*
         * 利用map构建红黑树，在每次超时事件选出超时时间点离
         * 现在最近的定时器，更新为下一次超时时间点。
         * */
    public:
        using TimerQueuePtr = std::shared_ptr<TimerQueue>;
        using TimerQueueWeakPtr = std::weak_ptr<TimerQueue>;

        TimerQueue(TimerQueue const &) = delete;
        TimerQueue const &operator=(TimerQueue const &) = delete;
        virtual ~TimerQueue();

        template <typename ..._Args>
        static TimerQueuePtr newInstance(_Args&& ...args){
            auto aNew = new TimerQueue(std::forward<_Args>(args)...);
            return TimerQueuePtr(aNew);
        }

        void addTimer(Timer::TimerPtr timer);

        void delTimer(Timer::TimerPtr timer);

        void handleTimeout();

        int const getTimerfd();

        Subject::SubjectPtr const &getSubject(){
            return _subject;
        }

    private:
        using __fakeType = bool;
        __fakeType const __fakeValue = false;
        class _cmpTimer{
        public:
            bool operator()(Timer::TimerPtr x, Timer::TimerPtr y){
                return x->getTimeoutPoint() < y->getTimeoutPoint();
            }
        };

        TimerQueue() throw(KQEventCommonException);
        void _updateTimeoutPoint();

        Subject::SubjectPtr _subject;
        Observer::ObserverPtr _observer;
        int _timefd;
        ::timespec _time;
        std::map<Timer::TimerPtr, __fakeType, _cmpTimer> _timers;
    };
}

#endif //KQEVENT_TIMERQUEUE_H
