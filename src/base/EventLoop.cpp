//
// Created by lee on 16-9-14.
//

#include "EventLoop.h"
#include <sys/syscall.h>
#include <unistd.h>

namespace KQEvent {

    void EventLoop::registerSubject(Subject::SubjectPtr subject) {
        assertOwner();
        _subjects[subject->getFd()] = subject;
    }

    Subject::SubjectPtr EventLoop::unregisterSubject(int fd) {
        assertOwner();
        Subject::SubjectPtr ptr = _subjects[fd];
        _subjects.erase(fd);
        return ptr;
    }

    void EventLoop::loop() {

    }

    EventLoop::EventLoop() :
            _tid(syscall(SYS_gettid)),
            _looping(false) {

    }

    Subject::SubjectPtr
    EventLoop::unregisterSubject(Subject::SubjectPtr subject) {
        assertOwner();
        auto pos = _subjects.begin();
        while(pos != _subjects.end()){
            if (subject == pos->second)
                break;
        }
        auto tmp = pos->second;
        _subjects.erase(pos);
        return tmp;
    }

    void EventLoop::assertOwner() throw(KQEventCommonException){
        assertOwner();
        if ( _tid != syscall(SYS_gettid)){
            throw KQEventCommonException(
                    "Loop can just running in "
                            "the thread which is the owner of it"
            );
        }
    }
}