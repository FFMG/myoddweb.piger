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
  int A() { return a; }
  const std::string& B() { return b; }

protected:
  int a;
  std::string b;
};

TEST_MEM(AnyTestCopyClass, SimpleClassCopyCheckType )
{
  CopyClass cc( IntRandomNumber<int>(), "Hello" );
  ::myodd::dynamic::Any any(cc);

  ASSERT_EQ(::myodd::dynamic::Misc_copy, any.Type());  
}

TEST_MEM(AnyTestCopyClass, SimpleClassCopyCheckValues)
{
  CopyClass cc(IntRandomNumber<int>(), "Hello");
  ::myodd::dynamic::Any any(cc);

  CopyClass cc1 = any;
  ASSERT_EQ(cc1, any);
}
