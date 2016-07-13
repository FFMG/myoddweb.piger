// myoddtest64.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	printf("Running main() from main\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}