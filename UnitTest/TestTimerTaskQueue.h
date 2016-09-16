//
// Created by lee on 16-9-17.
//

#ifndef KQEVENT_TESTTIMERTASKQUEUE_H
#define KQEVENT_TESTTIMERTASKQUEUE_H

using namespace KQEvent;

#include <iostream>
#include "cxxtest/TestSuite.h"
#include "TimerTaskQueue.h"

class TestTimerTaskQueue : public CxxTest::TestSuite{
public:
    void testTimerTaskQueue(void){
        auto loop = EventLoop::newInstance();
        auto taskQueue = TimerTaskQueue::newInstance(loop);
        auto period = Timer::Milliseconds(1000);
        auto tp = Timer::Clock::now() + period;
        taskQueue->runTask([](){std::cout << "run task now " << std::endl;});
        taskQueue->runTaskAt([](){std::cout << "run task at " << std::endl;}, tp);
        taskQueue->runTaskAfter([](){std::cout << "run task afer 3s";}, 3 * period);
        taskQueue->runTaskEvery([](){std::cout << "run task every 300ms" << std::endl;}, Timer::Milliseconds(300));
        taskQueue->runTaskEvery([](){std::cout << "run task every 400ms" << std::endl;}, Timer::Milliseconds(400));
        taskQueue->runTaskEvery([](){std::cout << "run task every 500ms" << std::endl;}, Timer::Milliseconds(500));
        taskQueue->runTaskEvery([](){std::cout << "run task every 600ms" << std::endl;}, Timer::Milliseconds(600));
        loop->loop();
    }
};

#endif //KQEVENT_TESTTIMERTASKQUEUE_H
