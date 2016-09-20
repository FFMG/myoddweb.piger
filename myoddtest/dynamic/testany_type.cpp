#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestType* 
TEST_MEM(AnyTestType, ShortInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, myodd::dynamic::get_type<short int>::value );
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, myodd::dynamic::get_type<const short int>::value);
}

TEST_MEM(AnyTestType, UnsignedShortInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, myodd::dynamic::get_type<unsigned short int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, myodd::dynamic::get_type<const unsigned short int>::value);
}

TEST_MEM(AnyTestType, Int)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, myodd::dynamic::get_type<int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, myodd::dynamic::get_type<const int>::value);
}

TEST_MEM(AnyTestType, UnsignedInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, myodd::dynamic::get_type<unsigned int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, myodd::dynamic::get_type<const unsigned int>::value);
}

TEST_MEM(AnyTestType, LongInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, myodd::dynamic::get_type<long int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, myodd::dynamic::get_type<const long int>::value);
}

TEST_MEM(AnyTestType, UnsignedLongInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, myodd::dynamic::get_type<unsigned long int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, myodd::dynamic::get_type<const unsigned long int>::value);
}

TEST_MEM(AnyTestType, LongLongInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, myodd::dynamic::get_type<long long int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, myodd::dynamic::get_type<const long long int>::value);
}

TEST_MEM(AnyTestType, UnsignedLongLongInt)
{
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, myodd::dynamic::get_type<unsigned long long int>::value);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, myodd::dynamic::get_type<const unsigned long long int>::value);
}

TEST_MEM(AnyTestType, Double)
{
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, myodd::dynamic::get_type<double>::value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, myodd::dynamic::get_type<const double>::value);
}

TEST_MEM(AnyTestType, Float)
{
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_float, myodd::dynamic::get_type<float>::value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_float, myodd::dynamic::get_type<const float>::value);
}

TEST_MEM(AnyTestType, LongDouble)
{
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, myodd::dynamic::get_type<long double>::value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, myodd::dynamic::get_type<const long double>::value);
}

TEST_MEM(AnyTestType, Bool)
{
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, myodd::dynamic::get_type<bool>::value);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, myodd::dynamic::get_type<const bool>::value);
}

TEST_MEM(AnyTestType, UnsignedChar)
{
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, myodd::dynamic::get_type<unsigned char>::value);
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, myodd::dynamic::get_type<const unsigned char>::value);
}

TEST_MEM(AnyTestType, SignedChar)
{
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, myodd::dynamic::get_type<signed char>::value);
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, myodd::dynamic::get_type<const signed char>::value);
}

TEST_MEM(AnyTestType, Char)
{
  ASSERT_EQ(myodd::dynamic::Type::Character_char, myodd::dynamic::get_type<char>::value);
  ASSERT_EQ(myodd::dynamic::Type::Character_char, myodd::dynamic::get_type<const char>::value);
}

TEST_MEM(AnyTestType, WideChar)
{
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, myodd::dynamic::get_type<wchar_t>::value);
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, myodd::dynamic::get_type<const wchar_t>::value);
}
