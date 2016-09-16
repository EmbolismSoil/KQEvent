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

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
