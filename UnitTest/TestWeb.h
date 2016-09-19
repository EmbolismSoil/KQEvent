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
        auto server = TCPServer::newInstance("127.0.0.1:12000");
        server->setConnectionNewHandler([](ConnectionPtr conn){
            std::cout << "new connection from "
                      << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString()
                      << std::endl;
        });

        server->setConnectionReadHandler([](ConnectionPtr conn, char *buf, size_t len){
            char msg[] =
                    "HTTP/1.1 200 OK\n"
                    "Server: GitHub.com\n"
                    "Date: Mon, 19 Sep 2016 05:06:46 GMT\n"
                    "Content-Type: text/html; charset=utf-8\n"
                    "\n"
                    "<html>\n"
                            "<body>\n"
                            "\n"
                            "<a href=\"http://github.com/EmbolismSoil/KQEvent\">\n"
                            "View source</a>\n"
                            "\n"
                            "</body>\n"
                            "</html>";
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
