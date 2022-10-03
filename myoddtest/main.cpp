// myoddtest64.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <crtdbg.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	printf("Running main() from main");
#ifdef _WIN64
  printf(" on x64\n");
#else
  printf(" on x86\n");
#endif

  testing::InitGoogleTest(&argc, argv);
  auto result = RUN_ALL_TESTS();
  return result;
}