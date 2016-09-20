//
// Created by lee on 16-9-20.
//
#include <iostream>
#include "TCPServer.h"
#include "TCPClient.h"

using namespace KQEvent;

void TestClient(void) {
    std::string serverAddr("127.0.0.1:12000");
    auto client = TCPClient::newInstance(serverAddr);

    client->onConnected([](TCPClient::ConnectionPtr conn) {
        std::cout << "connect from " << conn->getHostAddr()->toString()
                  << " to " << conn->getPeerAddr()->toString()
                  << "...OK" << std::endl;
    });

    client->onRead([&client](char *buf, size_t n) {
        buf[n] = '\0';
        std::cout << buf << std::endl;
        char msg[] = "message from Client\n";
        client->sendMsg(msg, sizeof(msg));
    });

    client->onClose([](TCPClient::ConnectionPtr conn) {
        std::cout << "disconnect from " << conn->getHostAddr()->toString()
                  << " to " << conn->getPeerAddr()->toString()
                  << "...OK" << std::endl;
        ::exit(0);
    });

    client->run();
}

int main(void) {
    TestClient();
    return 0;
}