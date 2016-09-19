//
// Created by lee on 16-9-19.
//

#ifndef KQEVENT_CONNECTOR_H
#define KQEVENT_CONNECTOR_H

#include <memory>
#include "Socket.h"
#include ""

namespace KQEvent{
    class Connector {
    public:
        using ConnectorPtr = std::shared_ptr<Connector>;
        using SocketPtr = Socket::SocketPtr ;

        Connector(Connector const&) = delete;
        Connector const &operator=(Connector const&) = delete;

        template <typename _Args>
        static  ConnectorPtr newInstance(_Args&& ...args){
            auto aNew = new Connector(std::forward<_Args>(args)...);
            return ConnectorPtr(aNew);
        }

    private:
        Connector(SocketPtr socket);
        SocketPtr _socket;

    };
}



#endif //KQEVENT_CONNECTOR_H
