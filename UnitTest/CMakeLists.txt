set(RUNNER "runner.cpp")
add_custom_command(OUTPUT ${RUNNER}
        COMMAND cxxtestgen --error-printer -o runner.cpp ~/github/KQEvent/test/TestSuit.h
        )

SET(SOURCE_LIST ${RUNNER} ../includes/common/KQEventCommonException.cpp ../includes/common/KQEventCommonException.h ../includes/common/common.h TestSuit.h)
INCLUDE_DIRECTORIES(../includes/base ../includes/net ../includes/common /home/lee/github/cxxtest)
ADD_EXECUTABLE(test ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(test base net)

