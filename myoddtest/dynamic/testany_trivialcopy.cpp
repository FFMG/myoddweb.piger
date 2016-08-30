#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestTrivialCopy* 

struct TrivialStruct
{
  int a;
  int b;
};

class TrivialClass
{
public:
  int a;
  int b;
};

class TrivialClass2
{
public:
  TrivialClass2(int aa, int bb) : a(aa), b(bb) {}
  TrivialClass2()  : TrivialClass2(0,0) { }

  int A() { return a; }
  int B() { return b; }

protected:
  int a;
  int b;
};

TEST_MEM_LOOP(AnyTestTrivialCopy, PassStructureAsToConstructor, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());  
}

TEST_MEM_LOOP(AnyTestTrivialCopy, PassClassAsToConstructor, NUMBER_OF_TESTS)
{
  TrivialClass tc;
  tc.a = IntRandomNumber<int>(false);
  tc.b = IntRandomNumber<int>(false);

  ::myodd::dynamic::Any any(tc);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, PassClass2AsToConstructor, NUMBER_OF_TESTS)
{
  TrivialClass2 tc( IntRandomNumber<int>(false), IntRandomNumber<int>(false) );

  ::myodd::dynamic::Any any(tc);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, AssignmentOperator, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any;
  any = ts;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, AssignmentOperatorTrivialClass, NUMBER_OF_TESTS)
{
  TrivialClass tc;
  tc.a = IntRandomNumber<int>(false);
  tc.b = IntRandomNumber<int>(false);

  ::myodd::dynamic::Any any;
  any = tc;

  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, CopyConstructor, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);

  auto any2 = any;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any2.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, CheckForMemoryLeaks, NUMBER_OF_TESTS)
{
  ::myodd::dynamic::Any any;
  TrivialStruct ts1 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts1;

  TrivialStruct ts2 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts2;

  TrivialStruct ts3 = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  any = ts3;
  
  auto any2 = any;
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any2.Type());

  // comapare the copied value.
  ASSERT_NE(ts1, any2);
  ASSERT_NE(ts2, any2);
  ASSERT_EQ(ts3, any2 );

  // compare the current value
  ASSERT_NE(ts1, any );
  ASSERT_NE(ts2, any );
  ASSERT_EQ(ts3, any );
}

TEST_MEM_LOOP(AnyTestTrivialCopy, GetValue, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  TrivialStruct ts2 = any;
  ASSERT_EQ(ts2.a, ts.a);
  ASSERT_EQ(ts2.b, ts.b);
}

TEST_MEM_LOOP(AnyTestTrivialCopy, GetValueDerivedStruct, NUMBER_OF_TESTS)
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  // create a derived struct
  struct DerivedTrivialStruct : TrivialStruct { };

  DerivedTrivialStruct ts2 = any;
  ASSERT_EQ(ts2.a, ts.a);
  ASSERT_EQ(ts2.b, ts.b);
}

TEST_MEM(AnyTestTrivialCopy, CannotCompareTrivialAndIntFundamentals )
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  EXPECT_THROW( any == (int)12, std::bad_cast );
}

TEST_MEM(AnyTestTrivialCopy, CannotCompareTrivialAndFloatFundamentals)
{
  TrivialStruct ts = { IntRandomNumber<int>(), IntRandomNumber<int>() };
  ::myodd::dynamic::Any any(ts);

  EXPECT_THROW(any == (float)12, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, FromTrivialToInt )
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());

  any = IntRandomNumber<int>(false);
  ASSERT_EQ(::myodd::dynamic::Integer_int, any.Type());
}

TEST_MEM(AnyTestTrivialCopy, FromTrivialToDouble)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());

  any = RealRandomNumber<double>();
  ASSERT_EQ(::myodd::dynamic::Floating_point_double, any.Type());
}

TEST_MEM(AnyTestTrivialCopy, FromTrivialToString)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());

  any = "Hello World";
  ASSERT_EQ(::myodd::dynamic::Character_char, any.Type());
}

TEST_MEM(AnyTestTrivialCopy, FromTrivialToWideString)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());

  any = L"Hello World";
  ASSERT_EQ(::myodd::dynamic::Character_wchar_t, any.Type());
}

TEST_MEM_LOOP(AnyTestTrivialCopy, CheckMoreComplextTrivialClassCopiesData, NUMBER_OF_TESTS)
{
  TrivialClass2 tc(IntRandomNumber<int>(false), IntRandomNumber<int>(false));

  ::myodd::dynamic::Any any(tc);
  TrivialClass2 tc2 = any;

  ASSERT_EQ(::myodd::dynamic::Misc_trivial, any.Type());
  ASSERT_EQ(tc.A(), tc2.A() );
  ASSERT_EQ(tc.B(), tc2.B());
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToInt )
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((int)any != (int)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToFloat)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((float)any != (float)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToChar)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((char)any != (char)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToWideChar)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((wchar_t)any != (wchar_t)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToUnsignedChar)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((unsigned char)any != (unsigned char)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToString)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW(std::string( (const char*)any) != std::string((const char*)any), std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToWideString)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW(std::wstring((const wchar_t*)any) != std::wstring((const wchar_t*)any), std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToBoolean)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((bool)any != (bool)any, std::bad_cast);
}

TEST_MEM(AnyTestTrivialCopy, CannotCastTrivialToDouble)
{
  TrivialStruct ts = { IntRandomNumber<int>(false), IntRandomNumber<int>(false) };
  ::myodd::dynamic::Any any(ts);
  EXPECT_THROW((double)any != (double)any, std::bad_cast);
}





