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

class KQEventTest : public CxxTest::TestSuite{
public:
    void testTimerQueue(void){

    }
};

#endif //KQEVENT_TESTSUIT_H
