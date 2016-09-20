SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST AbstractAcceptor.cpp Connection.cpp
        Connector.cpp IPAddress.cpp Socket.cpp
        TCPClient.cpp TCPServer.cpp TCPInfo.cpp
        ../../includes/net/Connection.h
        ../../includes/net/TCPClient.h Connector.cpp
        ../../includes/net/Connector.h
        IOMessage.cpp
        SendfileMessage.cpp
        ../../includes/net/SendfileMessage.h
        )

INCLUDE_DIRECTORIES(../../includes/net ../../includes/common ../../includes/base)
ADD_LIBRARY(net ${SRC_LIST})
