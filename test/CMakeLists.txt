SET(SOURCE_LIST main.cpp)
INCLUDE_DIRECTORIES(../includes/base ../includes/net)
ADD_EXECUTABLE(test ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(test base net)

