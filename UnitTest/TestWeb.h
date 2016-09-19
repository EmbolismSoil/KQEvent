//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_TESTWEB_H
#define KQEVENT_TESTWEB_H


#include "cxxtest/TestSuite.h"
#include <iostream>
#include "TCPServer.h"
#include "Connector.h"
#include "../includes/net/Connector.h"
#include "TCPClient.h"

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
            conn->softClose();
        });

        server->setConnectionCloseHandler([](ConnectionPtr conn){
            std::cout << " disconnect from "
                      << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString()
                      << std::endl;
        });

        server->run();
    }

    void TestClient(void){
        auto client = TCPClient::newInstance("127.0.0.1:12000");
        client->onConnected([](TCPClient::ConnectionPtr conn){
            std::cout << "connect from " << conn->getHostAddr()->toString()
                      << " to " << conn->getPeerAddr()->toString()
                      << "...OK" << std::endl;
        });

        client->onRead([&client](char *buf, size_t n){
            buf[n] = '\0';
            std::cout << buf << std::endl;
            char msg[] = "message from Client\n";
            client->sendMsg(msg, sizeof(msg));
        });

        client->onClose([](TCPClient::ConnectionPtr conn){
            std::cout << "disconnect from " << conn->getHostAddr()->toString()
                      << " to " << conn->getPeerAddr()->toString()
                      << "...OK" << std::endl;
            ::exit(0);
        });

        client->run();
    }
};

#endif //KQEVENT_TESTWEB_H
