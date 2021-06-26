SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (SRC_LIST KQEventCommonException.cpp)

INCLUDE_DIRECTORIES(../../includes/base ./ ../../includes/common)
ADD_LIBRARY(common ${SRC_LIST})
set_property(TARGET comm PROPERTY POSITION_INDEPENDENT_CODE ON)
