#ifndef __ABSOBSERVER_H__
#define __ABSOBSERVER_H__

#include <functional>
#include <string>
#include <atomic>
#include <memory>
#include <thread>

namespace KQEvent {
    class Subject;

    class Observer : public std::enable_shared_from_this<Observer> {
    public:
        typedef enum {
            DELETE, ALIVE
        } Command_t;
        using ObserverPtr = std::shared_ptr<Observer>;
        using ObserverWeakPtr = std::weak_ptr<Observer>;
        //为了应对多线程的情况，这里要求Handle_t的实现必须是线程安全的。
        using Handle_t = std::function<Command_t(std::shared_ptr<Subject>)>;

        /*
         * 不推荐直接使用构造函数new对象，因为KQEvent内并不鼓励使用原生指针
         */
        Observer(Handle_t const &handle, std::string const &name) :
                _handle(handle),
                _name(name)
        {
        }

        /*
         * 推荐使用该方法new对象
         */
        template<typename ..._Args>
        static ObserverPtr newInstance(_Args &&...args) {
            auto aNew = new Observer(std::forward<_Args>(args)...);
            return Observer::ObserverPtr(aNew);
        }

        void setHandle(Handle_t const &handle) { _handle = handle; }

        Handle_t const &getHandle() { return _handle; }

        virtual Command_t update(std::shared_ptr<Subject> &&subject)
        {
            return _handle(std::move(subject));
        }

        ObserverPtr getPtr(void)
        {
            return this->shared_from_this();
        }

        //for log
        //virtual void onAttach(void) {}
        //virtual void onDetach(void) {}
        void setOnAttachHandle(Handle_t handle) { _onAttach = handle; }
        void setOnDetachHandle(Handle_t handle) { _onDetach = handle; }

        Handle_t const &getOnAttachHandle(void) { return _onAttach; }
        Handle_t const &getOnDetachHandle(void) { return _onDetach; }

    private:
        Observer(Observer const &) = delete;
        Observer &operator=(Observer const &) = delete;
        explicit Observer()
        {
            auto handle = [](std::shared_ptr<Subject>) { return ALIVE; };
            _onAttach = handle;
            _onDetach = handle;
        }

    private:
        Handle_t _handle; //fixme : atomic
        Handle_t _onAttach; // fixme : atomic
        Handle_t _onDetach; //fixme : atomic
        //std::mutex _mtxLock;
        std::string _name;//used by log
    };
}
#endif
