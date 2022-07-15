#include "common/commandline_variables.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicCommandLineVariables, NoArgumentsAtAllNotEvenExeName)
{
  const int argc = 0;
  myodd::commandline_variables cmd( argc, nullptr );
  ASSERT_FALSE(cmd.contains(L"d"));
}

TEST(BasicCommandLineVariables, NoArguments)
{
  const int argc = 1;
  wchar_t* args[] = { L"example.exe" };
  myodd::commandline_variables cmd(argc, args);
  ASSERT_FALSE(cmd.contains(L"d"));
}

TEST(BasicCommandLineVariables, OneKeyWithNoValue)
{
  const int argc = 2;
  wchar_t* args[] = { L"example.exe", L"-d"};
  myodd::commandline_variables cmd(argc, args);
  
  // it exists
  ASSERT_TRUE(cmd.contains(L"d"));

  ASSERT_EQ( (const wchar_t*)nullptr, cmd.get(L"d"));
}

TEST(BasicCommandLineVariables, OneKeyWithNoValueIgnoreDefault)
{
  const int argc = 2;
  wchar_t* args[] = { L"example.exe", L"-d" };
  myodd::commandline_variables cmd(argc, args);

  // it exists
  ASSERT_TRUE(cmd.contains(L"d"));

  // we return null as the value does exist but it is not set.
  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"d", L"Default" ));
}

TEST(BasicCommandLineVariables, MoreThanOneKeyNoneHaveAValue)
{
  const int argc = 4;
  wchar_t* args[] = { L"example.exe", L"-a", L"-b", L"-c" };
  myodd::commandline_variables cmd(argc, args);

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));
  ASSERT_TRUE(cmd.contains(L"c"));

  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"a"));
  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"b"));
  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"c"));
}

TEST(BasicCommandLineVariables, GetValuesWillIgnoreDefault)
{
  const int argc = 5;
  wchar_t* args[] = { L"example.exe", L"-a", L"Foo", L"-b", L"Bar" };
  myodd::commandline_variables cmd(argc, args);

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));

  ASSERT_STREQ(L"Foo", cmd.get(L"a", L"Default"));
  ASSERT_STREQ(L"Bar", cmd.get(L"b", L"Default"));
}

TEST(BasicCommandLineVariables, HasValueGetAnotherWithNoDefault)
{
  const int argc = 5;
  wchar_t* args[] = { L"example.exe", L"-a", L"Foo", L"-b", L"Bar" };
  myodd::commandline_variables cmd(argc, args);

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));

  // does not
  ASSERT_FALSE(cmd.contains(L"c"));

  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"c"));
}

TEST(BasicCommandLineVariables, HasValueGetAnotherWithDefault)
{
  const int argc = 5;
  wchar_t* args[] = { L"example.exe", L"-a", L"Foo", L"-b", L"Bar" };
  myodd::commandline_variables cmd(argc, args);

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));

  // does not
  ASSERT_FALSE(cmd.contains(L"c"));

  ASSERT_STREQ(L"Default", cmd.get(L"c", L"Default"));
}

TEST(BasicCommandLineVariables, UseNonDefaultPrefixGetValuesWillIgnoreDefault)
{
  const int argc = 5;
  wchar_t* args[] = { L"example.exe", L"--a", L"Foo", L"--b", L"Bar" };
  myodd::commandline_variables cmd(argc, args, L"--");

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));

  ASSERT_STREQ(L"Foo", cmd.get(L"a", L"Default"));
  ASSERT_STREQ(L"Bar", cmd.get(L"b", L"Default"));
}

TEST(BasicCommandLineVariables, NonDefaultPrefixMoreThanOneKeyNoneHaveAValue)
{
  const int argc = 4;
  wchar_t* args[] = { L"example.exe", L"--a", L"--b", L"--c" };
  myodd::commandline_variables cmd(argc, args, L"--");

  // it exists
  ASSERT_TRUE(cmd.contains(L"a"));
  ASSERT_TRUE(cmd.contains(L"b"));
  ASSERT_TRUE(cmd.contains(L"c"));

  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"a"));
  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"b"));
  ASSERT_EQ((const wchar_t*)nullptr, cmd.get(L"c"));
}
