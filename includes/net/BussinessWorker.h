//
// Created by lee on 17-3-13.
//

#ifndef KQEVENT_BUSSINESSWORKER_H
#define KQEVENT_BUSSINESSWORKER_H

#include "EventLoop.h"
#include "list"
#include "Connection.h"
#include "AbstractAcceptor.h"

namespace KQEvent{
    class BussinessWorker {
    public:
        using ConnectionPtr = Connection::ConnectionPtr;
        using BussinessWorkerPtr = std::shared_ptr<BussinessWorker>;
        using ReadHandle_t = std::function<void(ConnectionPtr, char *, size_t)>;
        using Handle_t = AbstractAcceptor::Handle_t;

        void pushConnection(Connection::ConnectionPtr conn) noexcept;
        void runTask(TimerTaskQueue::Task task) noexcept;

        template <class ...Args>
        static BussinessWorkerPtr newInstance(Args&&... args){
            auto ptr = new BussinessWorker(std::forward(args)...);
            return BussinessWorkerPtr(ptr);
        }

        void setConnectionReadHandler(ReadHandle_t handler){
            _onReadHandler = handler;
        }

        void setConnectionCloseHandler(Handle_t handle){
            _onCloseHandler = handle;
        }

        void setConnectionExceptHandler(Handle_t handle){
            _onExceptionHandler = handle;
        }

    private:
        BussinessWorker();
        BussinessWorker(BussinessWorker const&) = delete;
        BussinessWorker const&operator=(BussinessWorker const&) = delete;
        using ConnectionPool_t = std::list<Connection::ConnectionPtr>;

        ConnectionPool_t _connectionPool;
        EventLoop::EventLoopPtr _loop;

        ReadHandle_t _onReadHandler;
        Handle_t _onCloseHandler;
        Handle_t _onExceptionHandler;


        std::shared_ptr<std::thread> _thread;
    };
}



#endif //KQEVENT_BUSSINESSWORKER_H
