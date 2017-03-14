//
// Created by lee on 17-3-13.
//

#include "BussinessWorker.h"

namespace KQEvent{
    void BussinessWorker::runTask(KQEvent::TimerTaskQueue::Task task) noexcept {
        auto taskQueue = _loop->getTimerTaskQueue();
        taskQueue->runTask(task);
    }

    void BussinessWorker::pushConnection(Connection::ConnectionPtr conn) noexcept {
        runTask([conn, this](){
            conn->attachReadHandler(_onReadHandler);
            conn->attachExceptHandler(_onExceptionHandler);
            conn->attachCloseHandler([this](ConnectionPtr c){

                auto pos = std::find(_connectionPool.begin(),
                                     _connectionPool.end(), c);
                if (pos == _connectionPool.end()) {
                    //处理故障，打印log。目前log和全局故障尚未完成，只能简单退出
                    return;
                }
                //连接的生命周期结束
                _onCloseHandler(c);
                _connectionPool.erase(pos);
            });

            _connectionPool.push_back(conn);
            _loop->registerSubject(conn->getSubject());
            _loop->getTimerTaskQueue()->wakeUp();
        });
    }

    BussinessWorker::BussinessWorker() {
        _onReadHandler = [](ConnectionPtr, char *, size_t){};
        _onCloseHandler = [](ConnectionPtr){};
        _onExceptionHandler = [](ConnectionPtr){};

        _thread = std::make_shared<std::thread>([this](){
            _loop = EventLoop::newInstance();
            _loop->loop();
        });
    }
}
