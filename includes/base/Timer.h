//
// Created by lee on 16-9-15.
//

#ifndef KQEVENT_TIMER_H
#define KQEVENT_TIMER_H

#include <memory>
#include <map>
#include <thread>

namespace KQEvent{
    class Timer {
        /*
         *  利用std::map构建红黑树，每次定时器超时后更新时，从红黑树中取出顶点，
         *  重新设置定时器超时时间即可。除此之外，还可以设置超时周期，但是不能
         *  设置多个超时周期，每次设置不同的超时周期，只会更新超时周期，而不会有
         *  多个超时周期。
         *
         *  TODO: 关于时区和获取系统时间需要对STL进行包装
         * */
    public:
        Timer(Timer &const) = delete;

        Timer const &operator=(Timer const &) = delete;

        using TimerPtr = std::shared_ptr<Timer>;

        template<typename ..._Args>
        TimerPtr newInstance(_Args &&...args) {
            auto aNew = new Timer(std::forward<_Args>(args)...);
            return TimerPtr(aNew);
        }

        virtual void addTimoutAt(std::chrono::time_point timePoint);

        virtual void addTimeoutAfter(std::chrono::milliseconds ms);

        virtual void updatePeriod(std::chrono::milliseconds ms);

    private:
        Timer();
        int _fd;
        typedef bool __fakeType;
        bool const __fakeValue = false;
        std::map<std::chrono::time_point, __fakeType> _times;
    };
}


#endif //KQEVENT_TIMER_H
