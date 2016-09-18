//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_TESTWEB_H
#define KQEVENT_TESTWEB_H


#include "cxxtest/TestSuite.h"
#include <iostream>
#include "TCPServer.h"

using namespace KQEvent;
using ConnectionPtr = TCPServer::ConnectionPtr;
class TestWeb : public CxxTest::TestSuite{
public:
    void TestServer(void){
        auto server = TCPServer::newInstance("127.0.0.1:10000");
        server->setConnectionNewHandler([](ConnectionPtr conn){
            std::cout << "new connection from "
                      << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString()
                      << std::endl;
        });

        server->setConnectionReadHandler([](ConnectionPtr conn, char *buf, size_t len){
            std::cout << "Client : " << buf << std::endl;
            //char msg[] = "Server : git it !\n";
            char msg[] =
                    "HTTP/1.1 200 OK\n"
                    "Date: Sat, 31 Dec 2005 23:59:59 GMT\n"
                    "Content-Type: text/html;charset=utf8\n"
                    "\n"
                    "＜html＞\n"
                    "＜head＞\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
                    "＜title＞Wrox Homepage＜/title＞\n"
                    "＜/head＞\n"
                    "＜body＞\n"
                    "＜!-- body goes here --＞\n"
                    "＜/body＞\n"
                    "＜/html＞";
            conn->sendMessage(msg, sizeof(msg));
            conn->setDisconnecting();
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

#endif //KQEVENT_TESTWEB_H
