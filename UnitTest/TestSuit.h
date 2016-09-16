//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_TESTSUIT_H
#define KQEVENT_TESTSUIT_H

#include "cxxtest/TestSuite.h"
#include "IPAddress.h"
#include <iostream>
#include <chrono>
#include "Socket.h"
#include "TCPInfo.h"
#include "Observer.h"
#include "Subject.h"
#include "EventLoop.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Connection.h"
#include "TimerQueue.h"

using namespace KQEvent;

class KQEventTest : public CxxTest::TestSuite {
public:
    void testTimerQueue(void){
        auto tq = TimerQueue::newInstance();
        auto period = Timer::Milliseconds(1000);
        auto timout = Timer::Clock::now() + period;
        auto handle =  [](){std::cout << std::endl << "test timer" << std::endl;};
        tq->addTimer(Timer::newInstance(timout + period, Timer::Milliseconds(0), handle));
        tq->addTimer(Timer::newInstance(timout + 5*period, Timer::Milliseconds(0), handle));
        tq->addTimer(Timer::newInstance(timout + 10*period, Timer::Milliseconds(1000), handle));

        auto loop = EventLoop::newInstance();
        loop->registerSubject(tq->getSubject());
        loop->loop();
    }

};

#endif //KQEVENT_TESTSUIT_H
