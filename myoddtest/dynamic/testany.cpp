#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTest* 
TEST_MEM(AnyTest, CheckTheDataType)
{
  auto x = myodd::dynamic::Any();
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}

TEST_MEM(AnyTest, CanCreateTheValueWithNullPointer)
{
  auto x = myodd::dynamic::Any( nullptr );
  ASSERT_EQ(myodd::dynamic::Type::type_null, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButIntTypeIsRespected)
{
  int* i = nullptr;
  auto x = myodd::dynamic::Any(i);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstIntTypeIsRespected)
{
  const int* i = nullptr;
  auto x = myodd::dynamic::Any(i);
  ASSERT_EQ(myodd::dynamic::Type::Integer_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButDoubleTypeIsRespected)
{
  double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstDoubleTypeIsRespected)
{
  const double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButLongDoubleTypeIsRespected)
{
  long double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstLongDoubleTypeIsRespected)
{
  const long double* d = nullptr;
  auto x = myodd::dynamic::Any(d);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_long_double, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButLongIntTypeIsRespected)
{
  long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstLongIntTypeIsRespected)
{
  const long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButLongLongIntTypeIsRespected)
{
  long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstLongLongIntTypeIsRespected)
{
  const long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_long_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButShortIntTypeIsRespected)
{
  short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstShortIntTypeIsRespected)
{
  const short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButUnsignedIntTypeIsRespected)
{
  unsigned int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstUnsignedIntTypeIsRespected)
{
  const unsigned int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButUnsignedLongIntTypeIsRespected)
{
  unsigned long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstUnsignedLongIntTypeIsRespected)
{
  const unsigned long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButUnsignedLongLongIntTypeIsRespected)
{
  unsigned long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstUnsignedLongLongIntTypeIsRespected)
{
  const unsigned long long int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_long_long_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButUnsignedShortIntTypeIsRespected)
{
  unsigned short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstUnsignedShortIntTypeIsRespected)
{
  const unsigned short int* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Integer_unsigned_short_int, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButUnsignedCharTypeIsRespected)
{
  unsigned char* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstUnsignedCharTypeIsRespected)
{
  const unsigned char* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_unsigned_char, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButSignedCharTypeIsRespected)
{
  signed char* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstSignedCharTypeIsRespected)
{
  const signed char* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_signed_char, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButWideCharTypeIsRespected)
{
  wchar_t* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButWideConstCharTypeIsRespected)
{
  const wchar_t* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Character_wchar_t, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButBoolTypeIsRespected)
{
  bool* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstBoolTypeIsRespected)
{
  const bool* b = nullptr;
  auto x = myodd::dynamic::Any(b);
  ASSERT_EQ(myodd::dynamic::Type::Boolean_bool, x.Type());
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButCharTypeIsRespected)
{
  char* c = nullptr;
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ('\0', x);
}

TEST_MEM(AnyTest, AddressOfPointerIsNullButConstCharTypeIsRespected)
{
  const char* c = nullptr;
  auto x = myodd::dynamic::Any(c);
  ASSERT_EQ('\0', x);
}

TEST_MEM(AnyTest, CanCreateWithAPointerValueInt)
{
  auto value = IntRandomNumber<short>();
  auto x = myodd::dynamic::Any( &value );
  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, x.Type());
  ASSERT_EQ(value, x );
}

TEST_MEM(AnyTest, CanCreateWithAPointerValueRealDouble)
{
  auto value = RealRandomNumber<double>();
  auto x = myodd::dynamic::Any(&value);
  ASSERT_EQ(myodd::dynamic::Type::Floating_point_double, x.Type());
  ASSERT_EQ(value, x);
}

TEST_MEM(AnyTest, CompareTwoAnyThatAreNotTheSameValue)
{
  auto valueX = IntRandomNumber<short>();
  auto valueY = 0;
  while (valueX == valueY)
  {
    valueY = IntRandomNumber<short>();
  }

  auto x = myodd::dynamic::Any(valueX);
  auto y = myodd::dynamic::Any(valueY);

  ASSERT_NE(x, y);
}

TEST_MEM(AnyTest, EqualOperatorFromNothing)
{
  auto value = IntRandomNumber<short>();
  auto x = myodd::dynamic::Any( value );
  auto y = x;

  ASSERT_EQ(myodd::dynamic::Type::Integer_short_int, y.Type());
  ASSERT_EQ(value, y);
}

TEST_MEM(AnyTest, SetAnyNullValueToNewValue)
{
  //  all the underlying values are null
  auto x = myodd::dynamic::Any(nullptr);

  // so that should not throw.
  auto y = x;

  // and the type should be valid.
  ASSERT_EQ(myodd::dynamic::Type::type_null, y.Type());
}

TEST_MEM(AnyTest, SetAnyNullValueToOldValue)
{
  //  all the underlying values are null
  auto x = myodd::dynamic::Any(nullptr);

  //  y is an existing value
  auto y = myodd::dynamic::Any( IntRandomNumber<int>() );

  // setting x to y should not throw.
  y = x;

  // and the type should be valid.
  ASSERT_EQ(myodd::dynamic::Type::type_null, y.Type());

  // and the value should be zero
  // we have to cast to int as we compare types.
  ASSERT_EQ(0, (int)y );
}