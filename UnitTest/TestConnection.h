//
// Created by lee on 16-9-16.
//

#ifndef KQEVENT_TESTCONNECTION_H
#define KQEVENT_TESTCONNECTION_H

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
class TestConnection : public CxxTest::TestSuite {
public:
    static Observer::Command_t func(Subject::SubjectPtr subject) {
        char buf[512];
        int ret = ::read(subject->getFd(), buf, sizeof(buf));
        if (ret == 0)
            return Observer::DELETE;
        std::cout << buf << std::endl;
        return Observer::ALIVE;
    }

    void testConnect(void) {
        return;
        int fd = ::open("/home/lee/test.cpp", O_APPEND | O_RDWR);
        auto conn = Connection::newInstance(fd);
        conn->attachReadHandler([](Connection::ConnectionPtr c) {
            char buf[128];
            int fd = c->getFd();
            int n = 0;
            while ((n = ::read(fd, buf, sizeof(buf))) > 0);
            std::cout << buf << std::endl;
            return Observer::ALIVE;
        });

        char buf[] = "\nfrom test suit\n";
        conn->sendMessage(buf, sizeof(buf));
        auto loop = EventLoop::newInstance();
        loop->registerSubject(conn->getSubject());
        loop->loop();
    }
};



#endif //KQEVENT_TESTCONNECTION_H
