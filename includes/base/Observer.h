#ifndef __ABSOBSERVER_H__
#define __ABSOBSERVER_H__

#include <functional>
#include <string>
#include <atomic>
#include <memory>

namespace KQEvent {
    class AbsSubject;

    class AbsObserver : public std::enable_shared_from_this<AbsObserver> {
    public:
        typedef enum {
            DELETE, ALIVE
        } Command_t;
        using AbsObserverPtr = std::shared_ptr<AbsObserver>;
        using Handle_t = std::function<Command_t(std::shared_ptr<AbsSubject>)>;

        /*不推荐直接使用构造函数new对象，因为KQEvent内并不鼓励使用原生指针*/
        AbsObserver(Handle_t const &handle, std::string const &name) :
                _handle(handle),
                _name(name) {

        }

        /*推荐使用该方法new对象*/
        template<typename ..._Args>
        static AbsObserverPtr newInstance(_Args &&...args) {
            auto aNew = new AbsObserver(std::forward<_Args>(args)...);
            return AbsObserver::AbsObserverPtr(aNew);
        }

        AbsObserver(AbsObserver const &) = delete;

        AbsObserver &operator=(AbsObserver const &) = delete;

        void setHandle(Handle_t const &handle) {
            _handle = handle;
        }

        Handle_t getHandle() { return _handle; }

        virtual Command_t update(std::shared_ptr<AbsSubject> subject) {
            return _handle(subject);
        }
/*
    std::shared_ptr<AbsObserver> getPtr(void){
		return this->shared_from_this();
    }
*/
        //for log
        virtual void onAttach(void) {}

        virtual void onDetach(void) {}

    private:
    	AbsObserver() = default;
        Handle_t _handle;
        std::string _name;//used by log
    };
}
#endif
