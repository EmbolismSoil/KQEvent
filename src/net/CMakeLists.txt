SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST Socket.cpp IPAddress.cpp TCPInfo.cpp Connection.cpp ../../includes/net/Connection.h TCPClient.cpp ../../includes/net/TCPClient.h Connector.cpp ../../includes/net/Connector.h)

INCLUDE_DIRECTORIES(../../includes/net ../../includes/common)
ADD_LIBRARY(net ${SRC_LIST})
