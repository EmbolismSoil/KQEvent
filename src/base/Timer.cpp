//
// Created by lee on 16-9-15.
//

#include "Timer.h"

namespace KQEvent {

    void Timer::handle() {
        _handle();
    }


    Timer::Timer(Timer::TimePoint timeout,
                 Timer::Milliseconds period,
                 Timer::Handle_t handle) :
            _timeout(timeout),
            _period(period),
            _handle(handle) {

    }

    Timer::Timer(Timer::Handle_t handle) :
            Timer(Timer::Clock::now(), Timer::Milliseconds(0), handle){

    }

    void Timer::setTimoutAt(Timer::TimePoint timePoint) {
        _timeout = timePoint;
    }

    void Timer::setTimeoutAfter(Timer::Milliseconds ms) {
        auto timePoint = std::chrono::high_resolution_clock::now() + ms;
        setTimoutAt(timePoint);
    }

    void Timer::updatePeriod(Timer::Milliseconds ms) {
        _period = ms;
    }

    const Timer::Milliseconds &Timer::getPeriod() {
        return _period;
    }

    const Timer::TimePoint &Timer::getTimeoutPoint() {
        return _timeout;
    }

}
