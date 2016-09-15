#include "Subject.h"

namespace KQEvent{
    Subject::Subject(int fd)
            : _fd(fd)
    {

    }

     void Subject::notifyReadObserver() {
         if (_readObserver.empty())
             return;

        for (auto pos = _readObserver.begin();
             pos != _readObserver.end();) {
            auto tmp = pos;
            auto observer = *pos;
            auto handler = observer->getHandle();
            if (handler(getPtr()) == Observer::DELETE)
                pos = _readObserver.erase(tmp);
            else
                ++pos;
        }
    }

     void Subject::notifyWriteObserver() {
         if (_writeObserver.empty())
             return;

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
         if (_exceptObserver.empty())
             return;
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
        _readObserver.push_back(observer);
    }

    void Subject::attachWriteObserver(Observer::ObserverPtr observer){
        auto handle = observer->getOnAttachHandle();
        handle(getPtr());
        _writeObserver.push_back(observer);
    }

    void Subject::attachExceptObserver(Observer::ObserverPtr observer){
        auto handle = observer->getOnAttachHandle();
        handle(getPtr());
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