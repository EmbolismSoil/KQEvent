#include "Subject.h"

namespace KQEvent{
    Subject::Subject(int fd)
            : _fd(fd)
    {

    }

     void Subject::notifyReadObserver() {
         if (_readObserver.empty()){
             //没有人对读事件感兴趣了，所以就屏蔽读事件，poller会用到这个标志
             setReadEvent(false);
             return;
         }

        for (auto Opos = _readObserver.begin();
             Opos != _readObserver.end();) {
            auto tmp = Opos;
            auto observer = *Opos;
            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE)
                Opos = _readObserver.erase(tmp);
            else
                ++Opos;
        }
    }

     void Subject::notifyWriteObserver() {
         if (_writeObserver.empty()){
             //没人对写事件感兴趣了，所以就屏蔽这个事件,poller会用到这个标志
             setWriteEvent(false);
             return;
         }

        for (auto pos = _writeObserver.begin();
             pos != _writeObserver.end();){
            auto tmp = pos;
            auto observer = *pos;
            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE){
                pos = _writeObserver.erase(tmp);
            }else{
                ++pos;
            }
        }
    }

     void Subject::notifyExceptObserver(){
         if (_exceptObserver.empty()){
             //没人对异常事件感兴趣了，所以就屏蔽这个事件,poller会用到这个标志
             setExceptEvent(false);
             return;
         }
        for (auto pos = _exceptObserver.begin();
             pos != _exceptObserver.end();){
            auto tmp = pos;
            auto observer = *pos;
            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE){
                pos = _exceptObserver.erase(tmp);
            }else{
                ++pos;
            }
        }
    }

    void Subject::attachReadObserver(Observer::ObserverPtr observer){
        auto handle = observer->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().READ)
            setReadEvent(true);
        _readObserver.push_back(observer);
    }

    void Subject::attachWriteObserver(Observer::ObserverPtr observer){
        auto handle = observer->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().WRITE)
            setReadEvent(true);
        _writeObserver.push_back(observer);
    }

    void Subject::attachExceptObserver(Observer::ObserverPtr observer){
        auto handle = observer->getOnAttachHandle();
        handle(getPtr());
        if (!getEventMask().EXCEPT)
            setReadEvent(true);
        _exceptObserver.push_back(observer);
    }

    Observer::ObserverPtr
    Subject::detachReadObserver(Observer::ObserverPtr observer){
        auto pos = std::find(_readObserver.begin(),
                             _readObserver.end(), observer);
        auto tmp = *pos;
        auto handle = tmp->getOnDetachHandle();
        handle(getPtr());
        return tmp;
    }

    Observer::ObserverPtr
    Subject::detachWriteObserver(Observer::ObserverPtr observer){
        auto pos = std::find(_writeObserver.begin(),
                             _writeObserver.end(), observer);
        auto tmp = *pos;
        auto handle = tmp->getOnDetachHandle();
        handle(getPtr());
        return tmp;
    }

    Observer::ObserverPtr
    Subject::detachExceptObserver(Observer::ObserverPtr observer){
        auto pos = std::find(_exceptObserver.begin(),
                             _exceptObserver.end(), observer);
        auto tmp = *pos;
        auto handle = tmp->getOnDetachHandle();
        handle(getPtr());
        return tmp;
    }

}