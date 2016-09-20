#include "Subject.h"

namespace KQEvent {
    Subject::Subject(int fd)
            : _fd(fd) {

    }

    void Subject::notifyReadObserver() {
        if (_readObserver.empty()) {
            //没有人对读事件感兴趣了，所以就屏蔽读事件，poller会用到这个标志
            setReadEvent(false);
            return;
        }

        for (auto Opos = _readObserver.begin();
             Opos != _readObserver.end();) {
            auto tmp = Opos;
            auto observer = Opos->lock();

            if (!observer) {//观察者对象生命周期结束
                Opos = _readObserver.erase(Opos);
                continue;
            }

            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE)//debug
                Opos = _readObserver.erase(tmp);
            else
                ++Opos;
        }
    }

    void Subject::notifyWriteObserver() {
        if (_writeObserver.empty()) {
            //没人对写事件感兴趣了，所以就屏蔽这个事件,poller会用到这个标志
            setWriteEvent(false);
            return;
        }

        for (auto pos = _writeObserver.begin();
             pos != _writeObserver.end();) {
            auto tmp = pos;
            auto observer = pos->lock();

            if (!observer) {
                pos = _writeObserver.erase(pos);
                continue;
            }

            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE) {
                pos = _writeObserver.erase(tmp);
            } else {
                ++pos;
            }
        }
    }

    void Subject::notifyExceptObserver() {
        if (_exceptObserver.empty()) {
            //没人对异常事件感兴趣了，所以就屏蔽这个事件,poller会用到这个标志
            setExceptEvent(false);
            return;
        }
        for (auto pos = _exceptObserver.begin();
             pos != _exceptObserver.end();) {
            auto tmp = pos;
            auto observer = pos->lock();
            if (!observer) {
                pos = _exceptObserver.erase(pos);
                continue;
            }
            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE) {
                pos = _exceptObserver.erase(tmp);
            } else {
                ++pos;
            }
        }
    }

    void Subject::attachReadObserver(Observer::ObserverWeakPtr observer) {
        auto obj = observer.lock();
        if (!obj)
            return;

        auto handle = obj->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().READ)
            setReadEvent(true);
        _readObserver.push_back(obj);
    }

    void Subject::attachWriteObserver(Observer::ObserverWeakPtr observer) {
        auto obj = observer.lock();

        if (!obj)
            return;

        auto handle = obj->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().WRITE)
            setWriteEvent(true);
        _writeObserver.push_back(obj);
    }

    void Subject::attachExceptObserver(Observer::ObserverWeakPtr observer) {
        auto obj = observer.lock();
        if (!obj) {
            return;
        }

        auto handle = obj->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().EXCEPT)
            setExceptEvent(true);
        _exceptObserver.push_back(obj);
    }

#if 1

    void Subject::detachReadObserver(Observer::ObserverWeakPtr observer) {
        //如果该对象已经不存在，subject会在notify的时候清除它
        auto target = observer.lock();
        if (!target)
            return;

        for (auto pos = _readObserver.begin();
             pos != _readObserver.end();) {
            auto obj = pos->lock();
            if (obj == target) {
                auto handle = obj->getHandle();
                handle(getPtr());
                pos = _readObserver.erase(pos);
                continue;
            } else {
                ++pos;
            }
        }
    }

    void Subject::detachWriteObserver(Observer::ObserverWeakPtr observer) {
        auto target = observer.lock();
        if (!target)
            return;

        for (auto pos = _writeObserver.begin();
             pos != _writeObserver.end();) {
            auto obj = pos->lock();
            if (obj == target) {
                auto handle = obj->getHandle();
                handle(getPtr());
                pos = _writeObserver.erase(pos);
                continue;
            } else {
                ++pos;
            }
        }

    }

    void Subject::detachExceptObserver(Observer::ObserverWeakPtr observer) {
        auto target = observer.lock();
        if (!target)
            return;

        for (auto pos = _exceptObserver.begin();
             pos != _exceptObserver.end();) {
            auto obj = pos->lock();
            if (obj == target) {
                auto handle = obj->getHandle();
                handle(getPtr());
                pos = _exceptObserver.erase(pos);
                continue;
            } else {
                ++pos;
            }
        }

        return;
    }

#endif

}