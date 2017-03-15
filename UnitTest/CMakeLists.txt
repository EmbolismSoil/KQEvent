SET(RUNNER ${PROJECT_SOURCE_DIR}/UnitTest/runner.cpp)
SET(TEST_SUIT ${PROJECT_SOURCE_DIR}/UnitTest/TestSuit.h
        ${PROJECT_SOURCE_DIR}/UnitTest/TestConnection.h
        ${PROJECT_SOURCE_DIR}/UnitTest/TestTimerTaskQueue.h
        ${PROJECT_SOURCE_DIR}/UnitTest/TestTCPServer.h
        ${PROJECT_SOURCE_DIR}/UnitTest/TestWeb.h)

ADD_CUSTOM_COMMAND(OUTPUT ${RUNNER}
        COMMAND /home/lee/github/cxxtest/bin/cxxtestgen --error-printer -o ${RUNNER} ${TEST_SUIT}
        )

SET(SOURCE_LIST ${RUNNER} ../src/common/KQEventCommonException.cpp ../includes/common/KQEventCommonException.h
        ../includes/common/common.h TestSuit.h ../includes/net/Connection.h
        ../includes/net/AbstractAcceptor.h TestTimerTaskQueue.h
        ../includes/base/TimerTaskQueue.h ../includes/net/AbstractAcceptor.h
        ../includes/net/TCPServer.h TestWeb.h ../src/net/AbstractMessage.cpp ../includes/net/AbstractMessage.h ../src/net/IOMessage.cpp ../includes/net/IOMessage.h ../includes/net/BussinessWorker.cpp ../includes/net/BussinessWorker.h)
INCLUDE_DIRECTORIES(../includes/base ../includes/net ../includes/common /home/lee/github/cxxtest)
# 测试用
ADD_EXECUTABLE(KQEventTestSuit ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(KQEventTestSuit net base pthread)
