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
#include "../includes/net/Connection.h"
#include <unistd.h>
#include <Observer.h>
#include <fcntl.h>

using namespace KQEvent;

class TestAcceptor : public CxxTest::TestSuite{
public:
    void TestServer(void){
		int ffd = ::open("/home/lee/pic1.png", O_RDWR);
		if (ffd < 0){
			std::cout << "failed in open file" << std::endl;
			exit(0);
		}

        Connection::ConnectionPtr  keepAlive;
        auto sock = Socket::newInstance();
        sock->bind(IPAddress::fromIPAddress("10.42.0.1:10000"));
        sock->listen(135);
        auto acceptor = AbstractAcceptor::newInstance(sock);
        auto loop = EventLoop::newInstance();
        acceptor->setOnConnectHandle([loop,ffd, &keepAlive](Connection::ConnectionPtr conn){
            conn->attachReadHandler([loop, ffd](Connection::ConnectionPtr c){
                auto *peek = c.get();
                char buf[1024];
                int n = ::read(c->getFd(), buf, sizeof(buf));
				int s = ::write(ffd, buf, n);
				std::cout << "write size : " << s << std::endl;
				if (n == 0)
					exit(0);
				if (n < 0)							
					return Observer::ALIVE;
                return Observer::ALIVE;
            });
            auto *peek = conn.get();
            conn->setConnected();
            keepAlive = conn;
            std::cout << "new connecion from " << conn->getPeerAddr()->toString()
                      << " to " << conn->getHostAddr()->toString() << std::endl;
            loop->registerSubject(conn->getSubject());
        });

        loop->registerSubject(acceptor->getSubject());
        loop->loop();
    }

    void TestClient(void){
        auto sock = Socket::newInstance();
        sock->bind(IPAddress::fromIPAddress("127.0.0.1:31456"));
        auto serverAddr = IPAddress::fromIPAddress("127.0.0.1:13000");
        int sockfd = sock->getFd();
        int flag = ::fcntl(sockfd, F_GETFL, 0);
        flag &=~ O_NONBLOCK;
        ::fcntl(sockfd, F_SETFL, flag );
        int fd = sock->connect(serverAddr);
        if (fd < 0){
            std::cout << "failed in connect" << std::endl;
            return;
        }
        char buf[] = "hello server\n";
        char rbuf[1024];
        for (;;){
            ::sleep(1);
            ::write(fd, buf, sizeof(buf));
            ::read(fd, rbuf, sizeof(rbuf));
            std::cout << rbuf << std::endl;
        }
    }
};


#endif //KQEVENT_TESTACCEPTOR_H
