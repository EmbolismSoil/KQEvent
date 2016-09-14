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
CxxTest::StaticSuiteDescription suiteDescription_KQEventTest( "/home/lee/github/KQEvent/UnitTest/TestSuit.h", 23, "KQEventTest", suite_KQEventTest, Tests_KQEventTest );

static class TestDescription_suite_KQEventTest_testComponent : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_KQEventTest_testComponent() : CxxTest::RealTestDescription( Tests_KQEventTest, suiteDescription_KQEventTest, 25, "testComponent" ) {}
 void runTest() { suite_KQEventTest.testComponent(); }
} testDescription_suite_KQEventTest_testComponent;

static class TestDescription_suite_KQEventTest_testEventLoop : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_KQEventTest_testEventLoop() : CxxTest::RealTestDescription( Tests_KQEventTest, suiteDescription_KQEventTest, 44, "testEventLoop" ) {}
 void runTest() { suite_KQEventTest.testEventLoop(); }
} testDescription_suite_KQEventTest_testEventLoop;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
