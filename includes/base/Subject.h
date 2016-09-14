#ifndef __ABSSUBJECT_H__
#define __ABSSUBJECT_H__

#include "Observer.h"
#include <memory>
#include <algorithm>
#include <vector>

namespace KQEvent {
    class Subject : public std::enable_shared_from_this<Subject> {
    public:
        struct EventType {
            bool READ = true;
            bool WRITE = true;
            bool EXCEPT = true;
        };

        Subject(int fd) : _fd(fd){}

        Subject(Subject const &) = delete;

        Subject const &operator=(Subject const &) = delete;

        using SubjectPtr = std::shared_ptr<Subject>;

        template <class ..._Arg>
        static SubjectPtr newInstance(_Arg ...args){
            auto aNew = new Subject(std::forward<_Arg>(args)...);
            return SubjectPtr(aNew);
        }

        SubjectPtr getPtr(void) {
            return shared_from_this();
        }

        virtual void notifyReadObserver() {
            for (auto pos = _readObserver.begin();
                        pos != _readObserver.end();) {
                auto tmp = pos;
                auto observer = *pos;
                auto handler = observer->getHandle();
                ++pos;
                if (handler(getPtr()) == Observer::DELETE)
                    _readObserver.erase(tmp);
            }
        }

        virtual void notifyWriteObserver() {
            for (auto pos = _writeObserver.begin();
                    pos != _writeObserver.end(); ++pos){
                auto tmp = pos;
                auto observer = *pos;
                auto handler = observer->getHandle();
                ++pos;
                if (handler(getPtr()) == Observer::DELETE){
                    _writeObserver.erase(tmp);
                }
            }
        }

        virtual void notifyExceptObserver(){
            for (auto pos = _exceptObserver.begin();
                        pos != _exceptObserver.end();){
                auto tmp = pos;
                auto observer = *pos;
                auto handler = observer->getHandle();
                ++pos;
                if (handler(getPtr()) == Observer::DELETE){
                    _exceptObserver.erase(tmp);
                }
            }
        }

        void attachReadObserver(Observer::ObserverPtr observer){
            auto handle = observer->getOnAttachHandle();
            handle(getPtr());
            _readObserver.push_back(observer);
        }

        void attachWriteObserver(Observer::ObserverPtr observer){
            auto handle = observer->getOnAttachHandle();
            handle(getPtr());
            _writeObserver.push_back(observer);
        }

        void attachExceptObserver(Observer::ObserverPtr observer){
            auto handle = observer->getOnAttachHandle();
            handle(getPtr());
            _exceptObserver.push_back(observer);
        }

        Observer::ObserverPtr
        detachReadObserver(Observer::ObserverPtr observer){
            auto pos = std::find(_readObserver.begin(),
                                 _readObserver.end(), observer);
            auto tmp = *pos;
            auto handle = tmp->getOnDetachHandle();
            handle(getPtr());
            return tmp;
        }

        Observer::ObserverPtr
        detachWriteObserver(Observer::ObserverPtr observer){
            auto pos = std::find(_writeObserver.begin(),
                                 _writeObserver.end(), observer);
            auto tmp = *pos;
            auto handle = tmp->getOnDetachHandle();
            handle(getPtr());
            return tmp;
        }

        Observer::ObserverPtr
        detachExceptObserver(Observer::ObserverPtr observer){
            auto pos = std::find(_exceptObserver.begin(),
                                 _exceptObserver.end(), observer);
            auto tmp = *pos;
            auto handle = tmp->getOnDetachHandle();
            handle(getPtr());
            return tmp;
        }

        struct EventType getEventNow() {
            return _eventMask;
        }

        void setReadEvent(bool on) {
            _eventMask.READ = on;
        }

        void setWriteEvent(bool on) {
            _eventMask.WRITE = on;
        }

        void setExceptEvent(bool on) {
            _eventMask.EXCEPT = on;
        }

        int getFd(){return _fd;}
        void setFd(int fd){_fd = fd;}
    private:
        std::vector<Observer::ObserverPtr> _readObserver;
        std::vector<Observer::ObserverPtr> _writeObserver;
        std::vector<Observer::ObserverPtr> _exceptObserver;
        EventType _eventMask;
        int _fd;
    };
}
#endif
