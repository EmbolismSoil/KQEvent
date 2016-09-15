//
// Created by lee on 16-9-15.
//

#ifndef KQEVENT_CONNECTION_H
#define KQEVENT_CONNECTION_H

#include <memory>
namespace KQEvent {
    class Connection {
    public:
        using ConnectionPtr = std::shared_ptr<Connection>;
        Connection(Connection const &) = delete;
        Connection const &operator=(Connection const &) = delete;

    private:
    };
}

#endif //KQEVENT_CONNECTION_H
