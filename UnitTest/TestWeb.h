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
        Connection::ConnectionPtr keep;
        auto loop = EventLoop::newInstance();
        auto contor = Connector::newInstance(loop, "127.0.0.1:12000");
        contor->setSucessHandler([&loop, &keep](Connection::ConnectionPtr conn){
            keep = conn;
            conn->setConnected();
            std::cout << "\nconnect to " << conn->getPeerAddr()->toString() << "..Ok" << std::endl;
            conn->attachReadHandler([](Connection::ConnectionPtr c, char *buf, int n){
                buf[n] = '\0';
                std::cout << buf << std::endl;
                char msg[] = "from Client\n";
                c->sendMessage(msg, sizeof(msg));
            });
            conn->attachCloseHandler([&keep](Connection::ConnectionPtr c){
                std::cout << "disconnect from " << c->getHostAddr()->toString()
                          << " to " << c->getPeerAddr()->toString() << std::endl;
                keep.reset();
            });

            loop->registerSubject(conn->getSubject());
        });

        contor->setErrorHandler([&loop, &contor](Socket::SocketPtr socket, int err){
           perror("Connector : ");
            static int retryCount = 0;
            if (++retryCount == 3){
                std::cout << "connect to server failed \n eixt now\n" << std::endl;
                exit(0);
            }
            return Connector::RETRY;
        });
        loop->loop();
    }
};

#endif //KQEVENT_TESTWEB_H
