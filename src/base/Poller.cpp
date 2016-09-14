//
// Created by lee on 16-9-14.
//

#include "Poller.h"
#include <iterator>

namespace KQEvent{

    void Poller::addToPoll(Subject::SubjectPtr subject) {
        _subjects[subject->getFd()] = subject;
        ::pollfd fd;
        fd.fd = 0;
        fd.events = 0;
        fd.revents = 0;
        if (subject->isFocusOnRead()){
            fd.events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI);
        }

        if (subject->isFocusOnWrite()){
            fd.events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
        }

        if (subject->isFocusOnExcept()){
            fd.events |= (POLLERR | POLLHUP | POLLNVAL);
        }
        fd.fd = subject->getFd();
        _pollfds.push_back(fd);
    }

    void Poller::removeFromPoll(int fd){

        for (auto pos = _pollfds.begin();
             pos != _pollfds.end(); ++pos){
            if (pos->fd == fd){
                _pollfds.erase(pos);
                _subjects.erase(fd);
                break;
            }
        }
    }

    void Poller::poll() {
        _exit = false;
        while(!_exit){
            if (!_pollfds.empty()){
                _retValue = ::poll(&*_pollfds.begin(), _pollfds.size(), _timeout);

                if (_retValue <= 0){//timeout or error
                    _handle(getPtr());
                    continue;
                }

                decltype(_pollfds.size()) cnt = _retValue;

                for (auto &item : _pollfds){
                    if (cnt == 0)
                        break;

                    if ((item.revents & (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP)) &&
                        _subjects[item.fd]->getEventMask().READ)
                    {
                        _subjects[item.fd]->notifyReadObserver();
                    }

                    if ((item.revents & (POLLOUT | POLLWRBAND | POLLWRNORM)) &&
                         _subjects[item.fd]->getEventMask().WRITE)
                    {
                        _subjects[item.fd]->notifyWriteObserver();
                    }

                    if ((item.revents & (POLLERR | POLLNVAL)) &&
                        _subjects[item.fd]->getEventMask().EXCEPT)
                    {
                        _subjects[item.fd]->notifyExceptObserver();
                    }

                    --cnt;
                }
            }
        }
    }

    void Poller::exit() {
        _exit = true;
    }

    void Poller::setTimeout(int timeout) {
        _timeout = timeout;
    }


    Poller::PollerPtr Poller::getPtr() {
        return shared_from_this();
    }


    Poller::Poller(Poller::Handle_t handle, int timeout) :
        _exit(true),
        _timeout(timeout),
        _pollfds(),
        _retValue(0),
        _handle(handle)
    {

    }

}