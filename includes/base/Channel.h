//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_CHANNEL_H
#define KQEVENT_CHANNEL_H

#include <functional>

namespace KQEvent{
    class Channel {
    public:
        using Handle_t = std::function<void()>;
    private:
        int _fd;
    };
}

#endif //KQEVENT_CHANNEL_H
