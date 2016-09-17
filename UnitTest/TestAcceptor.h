//
// Created by lee on 16-9-17.
//

#ifndef KQEVENT_TESTACCEPTOR_H
#define KQEVENT_TESTACCEPTOR_H

#include "cxxtest/TestSuite.h"
#include <iostream>
#include "AbstractAcceptor.h"
#include "Socket.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "Connection.h"
#include <unistd.h>
#include <Observer.h>
#include <fcntl.h>

using namespace KQEvent;

class TestAcceptor : public CxxTest::TestSuite{
public:
    void TestServer(void){
        auto sock = Socket::newInstance();
        sock->bind(IPAddress::fromIPAddress("127.0.0.1:12300"));
        sock->listen(135);
        auto acceptor = AbstractAcceptor::newInstance(sock);
        auto loop = EventLoop::newInstance();
        acceptor->setOnConnectHandle([loop](Connection::ConnectionPtr conn){
            conn->attachReadHandler([loop](Connection::ConnectionPtr c){
                char buf[1024];
                ::read(c->getFd(), buf, sizeof(buf));
                std::cout << "Client : " << buf << std::endl;
                char msg[] = "Server : This is KQEvent\n";
                c->sendMessage(msg, sizeof(msg));
                return Observer::ALIVE;
            });
            conn->setConnected();
            loop->registerSubject(conn->getSubject());
        });

        loop->registerSubject(acceptor->getSubject());
        loop->loop();
    }

    void TestClient(void){
        auto sock = Socket::newInstance();
        auto serverAddr = IPAddress::fromIPAddress("127.0.0.1:12300");
        int sockfd = sock->getFd();
        int flag = ::fcntl(sockfd, F_GETFL, 0);
        flag &=~ O_NONBLOCK;
        ::fcntl(sockfd, F_SETFL, flag );
        int fd = sock->connect(serverAddr);
        char buf[] = "hello server\n";
        char rbuf[1024];
        for (;;){
            ::write(fd, buf, sizeof(buf));
            ::read(fd, rbuf, sizeof(rbuf));
            std::cout << rbuf << std::endl;
        }
    }
};


#endif //KQEVENT_TESTACCEPTOR_H
