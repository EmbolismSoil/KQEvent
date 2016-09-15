//
// Created by lee on 16-9-15.
//

#ifndef KQEVENT_TIMER_H
#define KQEVENT_TIMER_H

#include <memory>
#include <map>
#include <chrono>

namespace KQEvent{
    class Timer {
        /*
         *  @ 当定时器周期为0时表示该定时器只触发一次
         * */
    public:
        Timer(Timer &const) = delete;

        Timer const &operator=(Timer const &) = delete;

        using TimerPtr = std::shared_ptr<Timer>;
        using Handle_t = std::function<void()>;

        template<typename ..._Args>
        TimerPtr newInstance(_Args &&...args) {
            auto aNew = new Timer(std::forward<_Args>(args)...);
            return TimerPtr(aNew);
        }

        virtual void setTimoutAt(std::chrono::time_point timePoint);

        virtual void setTimeoutAfter(std::chrono::milliseconds ms);

        virtual void updatePeriod(std::chrono::milliseconds ms);

        virtual void handle();

    private:
        Timer(std::chrono::time_point timeout,
              std::chrono::milliseconds period, Handle_t handle);
        std::chrono::time_point _timeout;
        std::chrono::milliseconds _period;
        Handle_t _handle;
    };
}


#endif //KQEVENT_TIMER_H
