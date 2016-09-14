#ifndef __ABSSUBJECT_H__
#define __ABSSUBJECT_H__

#include "Observer.h"
#include <memory>
#include <map>

namespace KQEvent {
    class Subject : public std::enable_shared_from_this<Subject> {
    public:
        Subject() = default;

        Subject(Subject const &) = delete;

        Subject const &operator=(Subject const &) = delete;

        using SubjectPtr = std::shared_ptr<Subject>;

        template<typename ..._Args>
        static SubjectPtr newInstance(_Args &&...args) {
            auto aNew = new Subject();
            return Subject::SubjectPTr(aNew);
        }

        std::shared_ptr<Subject> getPtr(void) {
            return this->shared_from_this();
        }

        virtual bool attach(std::shared_ptr<Observer> observer) {
            std::weak_ptr<Observer> ptr = observer;
            _observers[observer.get()] = ptr;
            auto handle = observer->getOnAttachHandle();
            handle(getPtr());
            return true;
        }

        virtual bool detach(std::shared_ptr<Observer> observer) {
            _observers.erase(observer.get());
            auto handle = observer->getOnDetachHandle();
            handle(getPtr());
            return true;
        }

        virtual bool notify(void) {
            bool ret = true;
            for (auto pos = _observers.begin();
                 pos != _observers.end();) {
                auto tmp = pos;
                auto ptr = pos->second;
                if (auto observer = ptr.lock()) {
                    if (observer->update(getPtr()) == Observer::DELETE) {
                        ++pos;
                        _observers.erase(tmp);
                    } else {
                        ++pos;
                    }
                } else {
                    ret = false;
                    ++pos;
                    _observers.erase(tmp);
                }
            }
            return ret;
        }

    private:
        //__gnu_cxx::hash_map<AbsObserver*, std::weak_ptr<AbsObserver>> _observers;
        std::map<Observer *, std::weak_ptr<Observer>> _observers;
    };
}
#endif
