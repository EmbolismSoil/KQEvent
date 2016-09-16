SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST Observer.cpp Subject.cpp ThreadPool.cpp
        ../../includes/base/ThreadPool.h EventLoop.cpp
        ../../includes/base/EventLoop.h Poller.cpp
        ../../includes/base/Poller.h Timer.cpp ../../includes/base/Timer.h TimerQueue.cpp ../../includes/base/TimerQueue.h TimerTaskQueue.cpp ../../includes/base/TimerTaskQueue.h)

INCLUDE_DIRECTORIES(../../includes/base ./ ../../includes/common)
ADD_LIBRARY(base ${SRC_LIST})
