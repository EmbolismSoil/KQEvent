//
// Created by lee on 16-9-15.
//

#include "Timer.h"

namespace KQEvent{

    void Timer::handle() {
        _handle();
    }


    Timer::Timer(std::chrono::high_resolution_clock::time_point timeout,
                    std::chrono::milliseconds period,
                                Timer::Handle_t handle):
        _timeout(timeout),
        _period(period),
        _handle(handle)
    {

    }

    void Timer::setTimoutAt(std::chrono::high_resolution_clock::time_point timePoint) {
        _timeout = timePoint;
    }

    void Timer::setTimeoutAfter(std::chrono::milliseconds ms) {
        auto timePoint = std::chrono::high_resolution_clock::now() + ms;
        setTimoutAt(timePoint);
    }

    void Timer::updatePeriod(std::chrono::milliseconds ms) {
        _period = ms;
    }
}
