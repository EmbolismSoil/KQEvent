SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST Socket.cpp IPAddress.cpp TCPInfo.cpp ../../includes/net/TCPInfo.h)

INCLUDE_DIRECTORIES(../../includes/net ../../includes/common)
ADD_LIBRARY(net ${SRC_LIST})
