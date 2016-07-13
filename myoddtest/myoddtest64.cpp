// myoddtest64.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	printf("Running main() from main\n");

	printf("###\n");
#ifdef _WIN64
	printf("    x64\n");
#else
	printf("#    x86\n");
#endif
	printf("###\n");

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}