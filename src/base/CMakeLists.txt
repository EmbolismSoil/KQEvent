SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST ./AbsObserver.cpp ./AbsSubject.cpp ThreadPool.cpp ../../includes/base/ThreadPool.h EventLoop.cpp ../../includes/base/EventLoop.h)

INCLUDE_DIRECTORIES(../../includes/base ./ ../../includes/common)
ADD_LIBRARY(base ${SRC_LIST})
