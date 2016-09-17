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
        auto taskQueue = loop->getTimerTaskQueue();
        auto period = Timer::Milliseconds(1000);
        auto tp = Timer::Clock::now() + period;
        taskQueue->runTask([](){std::cout << "run task now " << std::endl;});
        taskQueue->runTask([](){std::cout << "run task at " << std::endl;});
        taskQueue->runTask([](){std::cout << "run task afer 3s";});
        taskQueue->runTask([](){std::cout << "run task every 300ms" << std::endl;});
        taskQueue->runTask([](){std::cout << "run task every 400ms" << std::endl;});
        taskQueue->runTask([](){std::cout << "run task every 500ms" << std::endl;});
        taskQueue->runTask([](){std::cout << "run task every 600ms" << std::endl;});
        loop->loop();
    }
};

#endif //KQEVENT_TESTTIMERTASKQUEUE_H
