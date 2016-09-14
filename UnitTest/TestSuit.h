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

    void testObserver(void){
        auto subject = Subject::newInstance(12);
        auto observer = Observer::newInstance();
    }

};

#endif //KQEVENT_TESTSUIT_H
