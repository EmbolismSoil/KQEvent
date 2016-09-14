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

using namespace KQEvent;
class KQEventTest : public CxxTest::TestSuite{
public:
    void testComponent(void){
        auto address = IPAddress::fromHostName(std::string("www.baidu.com"));
        auto sock = Socket::newInstance(AF_INET, SOCK_STREAM);
        auto tcpInfo = TCPInfo::fromTCPSocketFd(sock->getFd());

        std::cout << tcpInfo->toString() << std::endl;
        for (auto &item : address){
            std::cout << item->toString() << std::endl;
        }
    }

    static Observer::Command_t func(Subject::SubjectPtr subject){
        char buf[512];
        int ret = ::read(subject->getFd(), buf, sizeof(buf));
        if (ret == 0)
            return Observer::DELETE;
        std::cout << buf << std::endl;
        return Observer::ALIVE;
    }
    void testEventLoop(void){
        auto eventLoop = EventLoop::newInstance();
        int fd = ::open("/home/lee/test", O_RDONLY);
        int flags = ::fcntl(fd, F_GETFL, 0);
        ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        auto subject = Subject::newInstance(fd);
        auto observer = Observer::newInstance();
        observer->setHandle(func);
        subject->attachReadObserver(observer);
        eventLoop->registerSubject(subject);
        eventLoop->loop();
    }

};

#endif //KQEVENT_TESTSUIT_H
