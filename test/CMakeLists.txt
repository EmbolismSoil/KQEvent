SET(SOURCE_LIST main.cpp ../includes/common/KQEventCommonException.cpp ../includes/common/KQEventCommonException.h)
INCLUDE_DIRECTORIES(../includes/base ../includes/net)
ADD_EXECUTABLE(test ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(test base net)

