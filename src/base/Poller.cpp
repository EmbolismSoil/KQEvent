//
// Created by lee on 16-9-14.
//

#include "Poller.h"
#include <iterator>

namespace KQEvent {

    void Poller::addToPoll(Subject::SubjectPtr const &subject) {
        ::pollfd fd;
        fd.fd = 0;
        fd.events = 0;
        fd.revents = 0;
        if (subject->getEventMask().READ) {
            fd.events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI);
        }

        if (subject->getEventMask().WRITE) {
            fd.events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
        }

        if (subject->getEventMask().EXCEPT) {
            fd.events |= (POLLERR | POLLHUP | POLLNVAL);
        }

        fd.fd = subject->getFd();
        auto pos = _subjects.find(subject->getFd());
        //如果插入的文件描述符已经被监听，那么就覆盖掉
        if (pos != _subjects.end()) {
            for (auto pos = _pollfds.begin(); pos != _pollfds.end(); ++pos) {
                if (pos->fd == fd.fd) {
                    _pollfds.erase(pos);
                    break;
                }
            }
        }

        if (_inHandle)
            _pollfdsHelper.push_back(fd);
        else
            _pollfds.push_back(fd);

        _subjects[subject->getFd()] = subject;

    }

    void Poller::removeFromPoll(int fd) {

        for (auto pos = _pollfds.begin();
             pos != _pollfds.end(); ++pos) {
            if (pos->fd == fd) {
                _pollfds.erase(pos);
                _subjects.erase(fd);
                break;
            }
        }
    }

    void Poller::poll() {
        _exit = false;
        while (!_exit) {
            checkNewSubject();
            _retValue = ::poll(&*_pollfds.begin(), _pollfds.size(), _timeout);

            if (_retValue <= 0) {//timeout or error
                _handle(getPtr());
                continue;
            }

            auto cnt = _retValue;
            _inHandle = true;
            for (auto pos = _pollfds.begin(); pos != _pollfds.end();) {
                if (cnt == 0)
                    break;

                Subject::SubjectPtr subject = _subjects[pos->fd].lock();
                if (!subject) {
                    _subjects.erase(pos->fd);
                    pos = _pollfds.erase(pos);
                    continue;
                }

                auto *peek = subject.get();
                if ((pos->revents & (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP))) {

                    subject->notifyReadObserver();
                    --cnt;
                }

                if ((pos->revents & (POLLOUT | POLLWRBAND | POLLWRNORM))) {
                    subject->notifyWriteObserver();
                    --cnt;
                }

                if ((pos->revents & (POLLERR | POLLNVAL))) {
                    subject->notifyExceptObserver();
                    --cnt;
                }

                if (!subject->getEventMask().READ) {
                    pos->events &= ~(POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                } else {
                    pos->events |= (POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI | POLLHUP);
                }

                if (!subject->getEventMask().WRITE) {
                    pos->events &= ~(POLLOUT | POLLWRBAND | POLLWRNORM);
                } else {
                    pos->events |= (POLLOUT | POLLWRBAND | POLLWRNORM);
                }

                if (!subject->getEventMask().EXCEPT) {
                    pos->events &= ~((POLLERR | POLLNVAL));
                } else {
                    pos->events |= ((POLLERR | POLLNVAL));
                }
                ++pos;
            }
            _inHandle = false;
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
            _inHandle(false),
            _timeout(timeout),
            _pollfds(),
            _retValue(0),
            _handle(handle) {

    }

    void Poller::checkNewSubject() {
        if (_pollfdsHelper.empty())
            return;

        for (auto &item : _pollfdsHelper) {
            _pollfds.push_back(item);
        }

        _pollfdsHelper.clear();
    }

}