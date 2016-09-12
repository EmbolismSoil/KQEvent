SET(SOURCE_LIST main.cpp ../includes/common/KQEventCommonException.cpp ../includes/common/KQEventCommonException.h ../includes/common/common.h)
INCLUDE_DIRECTORIES(../includes/base ../includes/net ../includes/common)
ADD_EXECUTABLE(test ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(test base net)

