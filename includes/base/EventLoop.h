//
// Created by lee on 16-9-14.
//

#ifndef KQEVENT_EVENTLOOP_H
#define KQEVENT_EVENTLOOP_H

#include <thread>
#include "Subject.h"

namespace KQEvent {
    class EventLoop {
    public:

        EventLoop();

        EventLoop(EventLoop const &) = delete;

        EventLoop const &operator=(EventLoop const &) = delete;

    private:
        bool _looping;
        std::thread::id _tid;
    };
}


#endif //KQEVENT_EVENTLOOP_H
