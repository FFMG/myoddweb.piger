// myoddtest64.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <crtdbg.h>
#include <gtest/gtest.h>

namespace testing
{
  class MemoryLeakDetector : public EmptyTestEventListener
  {
#ifdef _DEBUG
  public:
    virtual void OnTestStart(const TestInfo&)
    {
      _CrtMemCheckpoint(&memState_);
    }

    virtual void OnTestEnd(const TestInfo& test_info) {
      if (test_info.result()->Passed())
      {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
        {
          FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";
        }
      }
    }

  private:
    _CrtMemState memState_;
#endif // _DEBUG
  };
}

int main(int argc, char** argv)
{
	printf("Running main() from main");
#ifdef _WIN64
  printf(" on x64\n");
#else
  printf(" on x86\n");
#endif

  testing::InitGoogleTest(&argc, argv);

  // we cannot use the memory leaks tests as it returns false positives
  // with stuff like boost.
  // but you can use it every so often, ... just to make sure.
  // @see https://svn.boost.org/trac/boost/ticket/5856
  //
  testing::UnitTest::GetInstance()->listeners().Append(new testing::MemoryLeakDetector());
  return RUN_ALL_TESTS();
}