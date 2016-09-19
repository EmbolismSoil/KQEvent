//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_TCPCLIENT_H
#define KQEVENT_TCPCLIENT_H

#include <memory>
#include "Connection.h"
#include "Socket.h"

namespace KQEvent{
    class TCPClient {
    public:
        using TCPClientPtr = std::shared_ptr<TCPClient>;
        using ConnectionPtr = Connection::ConnectionPtr ;

        TCPClient(TCPClient const&) = delete;
        TCPClient const &operator=(TCPClient const&) = delete;

        template <typename  ..._Args>
        static TCPClientPtr  newInstance(_Args ...args)
        {
            auto aNew = new TCPClient(std::forward<_Args>(args)...);
            return TCPClientPtr(aNew);
        }


    private:
        TCPClient() = default;
        ConnectionPtr _connection;
        Socket::SocketPtr _socket;
    };
};

#endif //KQEVENT_TCPCLIENT_H
