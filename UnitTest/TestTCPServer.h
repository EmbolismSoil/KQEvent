//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_TESTTCPSERVER_H
#define KQEVENT_TESTTCPSERVER_H

#include "cxxtest/TestSuite.h"
#include <iostream>
#include "TCPServer.h"

using namespace KQEvent;
using ConnectionPtr = TCPServer::ConnectionPtr;
class TestTCPServer : public CxxTest::TestSuite{
public:
    void TestServer(void){
        auto server = TCPServer::newInstance("127.0.0.1:12000");
        server->setConnectionNewHandler([](ConnectionPtr conn){
            std::cout << "new connection from "
                      << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString()
                      << std::endl;
        });

        server->setConnectionReadHandler([](ConnectionPtr conn, char *buf, size_t len){
            buf[len] = '\0';
            std::cout << "Client : " << buf << std::endl;
            char msg[] = "Server : git it !\n";
            conn->sendMessage(msg, sizeof(msg));
        });

        server->setConnectionCloseHandler([](ConnectionPtr conn){
            std::cout << " disconnect from "
                      << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString()
                      << std::endl;
        });

        server->run();
    }
};

#endif //KQEVENT_TESTTCPSERVER_H
