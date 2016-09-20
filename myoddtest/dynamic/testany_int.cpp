#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestInt* 
TEST_MEM(AnyTestInt, NullWillReturnAnInt)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(0, (int)x);
}

TEST_MEM(AnyTestInt, CheckTheDataTypeInt)
{
  int value = IntRandomNumber<int>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ( myodd::dynamic::Type::Integer_int, x.Type() );
}

TEST_MEM(AnyTestInt, CheckTheDataTypeSigned)
{
  signed value = IntRandomNumber<signed>();
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeSignedInt)
{
  signed int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST_MEM(AnyTestInt, DefaultConstructorConstInt)
{
  auto x = myodd::dynamic::Any( (int)10);
  ASSERT_EQ(10, (int)x);
}

TEST_MEM(AnyTestInt, DefaultConstructorNonConstInt)
{
  int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(value, (int)x);
}

TEST_MEM(AnyTestInt, AValidLongToInt)
{
  long l = 10;
  auto x = myodd::dynamic::Any(l);
  ASSERT_EQ((int)l, (int)x);
}

TEST_MEM(AnyTestInt, CheckTheDataTypeShort )
{
  short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeShortInt)
{
  short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeSignedShort)
{
  signed short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeSignedShortInt)
{
  signed short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeUnsignedShort)
{
  unsigned short value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeUnsignedShortInt)
{
  unsigned short int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeUnsignedUnsigned)
{
  unsigned value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckTheDataTypeUnsignedUnsignedInt)
{
  unsigned int value = 10;
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST_MEM(AnyTestInt, CheckThatWeCanCastAShortIntToAnInt)
{
  auto value = IntRandomNumber<short>(false);
  auto x = myodd::dynamic::Any(value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());

  auto y = myodd::dynamic::Any( (int)x );
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, y.Type());
  ASSERT_EQ((int)value, y);
}
