//
// Created by lee on 16-9-20.
//
#include <iostream>
#include "TCPServer.h"

using namespace KQEvent;
using ConnectionPtr = Connection::ConnectionPtr ;

void TestServer(void){
    auto server = TCPServer::newInstance("127.0.0.1:12000");
    server->setConnectionNewHandler([](ConnectionPtr conn){
        std::cout << "new connection from "
                  << conn->getPeerAddr()->toString()
                  << " to " << conn->getHostAddr()->toString()
                  << std::endl;
    });

    server->setConnectionReadHandler([](ConnectionPtr conn, char *buf, size_t len){
        conn->sendFile("doc/simpleServer.cpp");
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

int main(void)
{
    TestServer();
    return 0;
}
