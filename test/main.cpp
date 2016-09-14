//
// Created by lee on 16-9-11.
//
#include "IPAddress.h"
#include <iostream>
#include "Socket.h"
#include "TCPInfo.h"
using namespace KQEvent;
int main(void) {
    auto address = IPAddress::fromHostName(std::string("www.baidu.com"));
    auto sock = Socket::newInstance(AF_INET, SOCK_STREAM);
    auto tcpInfo = TCPInfo::fromTCPSocketFd(sock->getFd());

    std::cout << tcpInfo->toString() << std::endl;
    for (auto &item : address){
        std::cout << item->toString() << std::endl;
    }
    return 0;
}
