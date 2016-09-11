SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST ./AbsObserver.cpp ./AbsSubject.cpp)

INCLUDE_DIRECTORIES(../../includes/base ./)
ADD_LIBRARY(base ${SRC_LIST})
