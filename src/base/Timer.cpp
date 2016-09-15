//
// Created by lee on 16-9-15.
//

#include "Timer.h"

namespace KQEvent{

    void Timer::addTimoutAt(std::chrono::time_point timePoint) {
        _times[timePoint] = __fakeValue;
    }

    void Timer::addTimeoutAfter(std::chrono::milliseconds ms) {
        auto timePoint = std::chrono::high_resolution_clock::now() + ms;
        addTimoutAt(timePoint);
    }

    void Timer::updatePeriod(std::chrono::milliseconds ms) {

    }

    Timer::Timer() {

    }
}
