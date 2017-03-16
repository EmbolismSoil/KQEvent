//
// Created by lee on 17-3-15.
//

#ifndef KQEVENT_TESTPOLL_H
#define KQEVENT_TESTPOLL_H

#include "cxxtest/TestSuite.h"
#include <thread>
#include <bits/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include "Socket.h"


class TestPoll : public CxxTest::TestSuite {
public:
    void TestCloseFdWhenPolling(){
        int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        auto addr = KQEvent::IPAddress::fromIPAddress("127.0.0.1:17000");
        auto server_addr = addr->getSockAddr();
        ::bind(fd, (sockaddr*)&server_addr, addr->getSocketLen());
        ::listen(fd, 10);

        std::vector<::pollfd> fds;
        ::pollfd pfd;
        pfd.fd = fd;

        pfd.events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI);
        pfd.events |= (POLLOUT | POLLWRBAND | POLLWRNORM);

        fds.push_back(pfd);
        std::thread* close_thread;

        for (;;){
            ::poll(&*fds.begin(), fds.size(), 60000);
            for (auto &item : fds){
                if (item.fd == fd && item.revents & (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP)){
                    int connfd = accept4(fd, nullptr, nullptr, SOCK_CLOEXEC | SOCK_NONBLOCK);
                    pfd.fd = connfd;
                    fds.push_back(pfd);

                    struct ::sockaddr_in peer;
                    ::socklen_t len = sizeof(peer);
                    ::getpeername(connfd, (struct ::sockaddr *)&peer, &len);
                    auto peer_addr = KQEvent::IPAddress::fromSockAddr(peer);
                    std::cout << "new connection from " << peer_addr->toString() << std::endl;

                    close_thread = new std::thread([connfd, peer_addr](){
                        std::cout << "close connection " << peer_addr->toString() << std::endl;
                        ::close(connfd);
                    });

                    char buf[1024] = "HTTP/1.1 200 OK\n"
                            "Server: GitHub.com\n"
                            "Date: Mon, 19 Sep 2016 05:06:46 GMT\n"
                            "Content-Type: text/html\n"
                            "charset=utf-8\n"
                            "\n";

                    ::write(connfd, buf, sizeof(buf));
                }
            }
        }
    }
};

#endif //KQEVENT_TESTPOLL_H
