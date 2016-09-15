//
// Created by lee on 16-9-14.
//

#include "Poller.h"
#include <iterator>

namespace KQEvent{

    void Poller::addToPoll(Subject::SubjectPtr const & subject) {
        _subjects[subject->getFd()] = subject;
        ::pollfd fd;
        fd.fd = 0;
        fd.events = 0;
        fd.revents = 0;
        if (subject->getEventMask().READ){
            fd.events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI);
        }

        if (subject->getEventMask().WRITE){
            fd.events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
        }

        if (subject->getEventMask().EXCEPT){
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
                    auto subject = _subjects[item.fd];
                    if ((item.revents & (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP)))
                    {
                        subject->notifyReadObserver();
                    }

                    if ((item.revents & (POLLOUT | POLLWRBAND | POLLWRNORM)))
                    {
                        subject->notifyWriteObserver();
                    }

                    if ((item.revents & (POLLERR | POLLNVAL)))
                    {
                        subject->notifyExceptObserver();
                    }

                    if (!subject->getEventMask().READ){
                        item.events &= ~(POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                    }else{
                        item.events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                    }

                    if (!subject->getEventMask().WRITE){
                        item.events &= ~(POLLOUT | POLLWRBAND | POLLWRNORM);
                    }else{
                        item.events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
                    }

                    if (!subject->getEventMask().EXCEPT){
                        item.events &= ~((POLLERR | POLLNVAL));
                    }else{
                        item.events |= ((POLLERR | POLLNVAL));
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