/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_KQEventTest_init = false;
#include "/home/lee/github/KQEvent/UnitTest/TestSuit.h"

static KQEventTest suite_KQEventTest;

static CxxTest::List Tests_KQEventTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_KQEventTest( "/home/lee/github/KQEvent/UnitTest/TestSuit.h", 26, "KQEventTest", suite_KQEventTest, Tests_KQEventTest );

static class TestDescription_suite_KQEventTest_testTimerQueue : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_KQEventTest_testTimerQueue() : CxxTest::RealTestDescription( Tests_KQEventTest, suiteDescription_KQEventTest, 28, "testTimerQueue" ) {}
 void runTest() { suite_KQEventTest.testTimerQueue(); }
} testDescription_suite_KQEventTest_testTimerQueue;

#include "/home/lee/github/KQEvent/UnitTest/TestConnection.h"

static TestConnection suite_TestConnection;

static CxxTest::List Tests_TestConnection = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestConnection( "/home/lee/github/KQEvent/UnitTest/TestConnection.h", 25, "TestConnection", suite_TestConnection, Tests_TestConnection );

static class TestDescription_suite_TestConnection_testConnect : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestConnection_testConnect() : CxxTest::RealTestDescription( Tests_TestConnection, suiteDescription_TestConnection, 36, "testConnect" ) {}
 void runTest() { suite_TestConnection.testConnect(); }
} testDescription_suite_TestConnection_testConnect;

#include "/home/lee/github/KQEvent/UnitTest/TestTimerTaskQueue.h"

static TestTimerTaskQueue suite_TestTimerTaskQueue;

static CxxTest::List Tests_TestTimerTaskQueue = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestTimerTaskQueue( "/home/lee/github/KQEvent/UnitTest/TestTimerTaskQueue.h", 14, "TestTimerTaskQueue", suite_TestTimerTaskQueue, Tests_TestTimerTaskQueue );

static class TestDescription_suite_TestTimerTaskQueue_testTimerTaskQueue : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestTimerTaskQueue_testTimerTaskQueue() : CxxTest::RealTestDescription( Tests_TestTimerTaskQueue, suiteDescription_TestTimerTaskQueue, 16, "testTimerTaskQueue" ) {}
 void runTest() { suite_TestTimerTaskQueue.testTimerTaskQueue(); }
} testDescription_suite_TestTimerTaskQueue_testTimerTaskQueue;

#include "/home/lee/github/KQEvent/UnitTest/TestAcceptor.h"

static TestAcceptor suite_TestAcceptor;

static CxxTest::List Tests_TestAcceptor = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestAcceptor( "/home/lee/github/KQEvent/UnitTest/TestAcceptor.h", 22, "TestAcceptor", suite_TestAcceptor, Tests_TestAcceptor );

static class TestDescription_suite_TestAcceptor_TestServer : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestAcceptor_TestServer() : CxxTest::RealTestDescription( Tests_TestAcceptor, suiteDescription_TestAcceptor, 24, "TestServer" ) {}
 void runTest() { suite_TestAcceptor.TestServer(); }
} testDescription_suite_TestAcceptor_TestServer;

static class TestDescription_suite_TestAcceptor_TestClient : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestAcceptor_TestClient() : CxxTest::RealTestDescription( Tests_TestAcceptor, suiteDescription_TestAcceptor, 62, "TestClient" ) {}
 void runTest() { suite_TestAcceptor.TestClient(); }
} testDescription_suite_TestAcceptor_TestClient;

#include "/home/lee/github/KQEvent/UnitTest/TestTCPServer.h"

static TestTCPServer suite_TestTCPServer;

static CxxTest::List Tests_TestTCPServer = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestTCPServer( "/home/lee/github/KQEvent/UnitTest/TestTCPServer.h", 14, "TestTCPServer", suite_TestTCPServer, Tests_TestTCPServer );

static class TestDescription_suite_TestTCPServer_TestServer : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestTCPServer_TestServer() : CxxTest::RealTestDescription( Tests_TestTCPServer, suiteDescription_TestTCPServer, 16, "TestServer" ) {}
 void runTest() { suite_TestTCPServer.TestServer(); }
} testDescription_suite_TestTCPServer_TestServer;

#include "/home/lee/github/KQEvent/UnitTest/TestWeb.h"

static TestWeb suite_TestWeb;

static CxxTest::List Tests_TestWeb = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestWeb( "/home/lee/github/KQEvent/UnitTest/TestWeb.h", 15, "TestWeb", suite_TestWeb, Tests_TestWeb );

static class TestDescription_suite_TestWeb_TestServer : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestWeb_TestServer() : CxxTest::RealTestDescription( Tests_TestWeb, suiteDescription_TestWeb, 17, "TestServer" ) {}
 void runTest() { suite_TestWeb.TestServer(); }
} testDescription_suite_TestWeb_TestServer;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
