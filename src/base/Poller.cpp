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
            //if (!_pollfds.empty()){ //fixme
                _retValue = ::poll(&*_pollfds.begin(), _pollfds.size(), _timeout);

                if (_retValue <= 0){//timeout or error
                    _handle(getPtr());
                    continue;
                }

                auto cnt = _retValue;

                for (auto pos = _pollfds.begin(); pos != _pollfds.end();){
                //for (auto &item : _pollfds){ //fixme
                    if (cnt == 0)
                        break;

                    Subject::SubjectPtr subject = _subjects[pos->fd].lock();
                    if(!subject){
                        _subjects.erase(pos->fd);
                        pos = _pollfds.erase(pos);
                        continue;
                    }

                    if ((pos->revents & (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP)))
                    {
                        subject->notifyReadObserver();
                        --cnt;
                    }

                    if ((pos->revents & (POLLOUT | POLLWRBAND | POLLWRNORM)))
                    {
                        subject->notifyWriteObserver();
                        --cnt;
                    }

                    if ((pos->revents & (POLLERR | POLLNVAL)))
                    {
                        subject->notifyExceptObserver();
                        --cnt;
                    }

                    if (!subject->getEventMask().READ){
                        pos->events &= ~(POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                    }else{
                        pos->events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                    }

                    if (!subject->getEventMask().WRITE){
                        pos->events &= ~(POLLOUT | POLLWRBAND | POLLWRNORM);
                    }else{
                        pos->events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
                    }

                    if (!subject->getEventMask().EXCEPT){
                        pos->events &= ~((POLLERR | POLLNVAL));
                    }else{
                        pos->events |= ((POLLERR | POLLNVAL));
                    }
                    //--cnt;
                    ++pos;
                }
            //}
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