SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST Socket.cpp IPAddress.cpp)

INCLUDE_DIRECTORIES(../../includes/net ./)
ADD_LIBRARY(net ${SRC_LIST})
