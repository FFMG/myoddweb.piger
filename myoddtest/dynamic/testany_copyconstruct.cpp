#include "dynamic/any.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

// --gtest_filter=AnyTestTrivialCopy* 
class CopyClass
{
public:
  CopyClass(int aa, const std::string& bb) : a(aa), b(bb) {}
  CopyClass(const CopyClass& other) { *this = other; };
  const CopyClass& operator=(const CopyClass& other) {
    if (this != &other)
    {
      a = other.a;
      b = other.b;
    }
    return *this;
  }
  int A() const { return a; }
  const std::string& B() const { return b; }

protected:
  int a;
  std::string b;
};

class CopyClassWithEqualOperator : CopyClass
{
public:
  CopyClassWithEqualOperator(int aa, const std::string& bb) : CopyClass(aa, bb){}

  friend bool operator==(const CopyClassWithEqualOperator& what1, const CopyClassWithEqualOperator& what2) {
    return what1.A() == what2.A() && what1.B() == what2.B();
  }
};

TEST_MEM(AnyTestCopyClass, SimpleClassCopyCheckType )
{
  CopyClass cc( IntRandomNumber<int>(), "Hello" );
  ::myodd::dynamic::Any any(cc);

  ASSERT_EQ(::myodd::dynamic::Misc_copy, any.Type());  
}

TEST_MEM(AnyTestCopyClass, SimpleClassCopyCheckValuesWithoutEqualOperator)
{
  CopyClass cc(IntRandomNumber<int>(), "Hello");
  ::myodd::dynamic::Any any(cc);

  CopyClass cc1 = any;
  EXPECT_THROW(cc1 == any, std::runtime_error);
}

TEST_MEM(AnyTestCopyClass, SimpleClassCopyCheckValues)
{
  CopyClassWithEqualOperator cc(IntRandomNumber<int>(), "Hello");
  ::myodd::dynamic::Any any(cc);

  CopyClassWithEqualOperator cc1 = any;
  ASSERT_EQ(cc1, any);
}
