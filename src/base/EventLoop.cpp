//
// Created by lee on 16-9-14.
//

#include "EventLoop.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace KQEvent {

    void EventLoop::registerSubject(Subject::SubjectPtr subject) {
        assertOwner();
        _poller->addToPoll(subject);
    }

    void EventLoop::unregisterSubject(int fd) {
        assertOwner();
        _poller->removeFromPoll(fd);
    }

    void EventLoop::loop() {
        assertOwner();
        _looping = true;
        _poller->poll();
    }

    EventLoop::EventLoop() :
            _tid(syscall(SYS_gettid)),
            _looping(false)
    {
        _poller = Poller::newInstance([](Poller::PollerPtr p){});
    }


    void EventLoop::assertOwner() throw(KQEventCommonException){
        if ( _tid != syscall(SYS_gettid)){
            throw KQEventCommonException(
                    "Loop can just running in "
                            "the thread which is the owner of it"
            );
        }
    }
}