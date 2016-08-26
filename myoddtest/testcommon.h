#pragma once

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <limits>
#include <string>
#include <boost/lexical_cast.hpp>

#include <chrono>
#include <random>

#ifndef UNUSED_ALWAYS
# define UNUSED_ALWAYS(x) x
#endif

#undef max
#undef min

# define MEMORY_GUARD_START \
_CrtMemState memState_; \
_CrtMemCheckpoint(&memState_);\
{

# define MEMORY_GUARD_END \
}\
_CrtMemState stateNow, stateDiff;\
_CrtMemCheckpoint(&stateNow);\
int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);\
UNUSED_ALWAYS(stateNow);\
if (diffResult)\
{\
  FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";\
}

#define GTEST_TEST_MEM_(test_case_name, test_name, parent_class, parent_id)\
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class {\
 public:\
  GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() { \
    _CrtMemCheckpoint(&memState_);\
  }\
  \
  virtual ~GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() { \
    _CrtMemState stateNow, stateDiff;\
    _CrtMemCheckpoint(&stateNow);\
    int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);\
    UNUSED_ALWAYS(stateNow);\
    if (diffResult)\
    {\
      ADD_FAILURE() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";\
    }\
  }\
 private:\
  _CrtMemState memState_; \
  virtual void TestBody();\
  static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN_(\
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name));\
};\
\
::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)\
  ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
        #test_case_name, #test_name, NULL, NULL, \
        (parent_id), \
        parent_class::SetUpTestCase, \
        parent_class::TearDownTestCase, \
        new ::testing::internal::TestFactoryImpl<\
            GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);\
void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()

# define TEST_MEM(test_case_name, test_name) \
  GTEST_TEST_MEM_(test_case_name, test_name,\
    ::testing::Test, ::testing::internal::GetTestTypeId())

template<typename T>
T RealRandomNumber(bool canBeZero = true)
{
  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  std::uniform_real_distribution<T> unif(lower_bound, upper_bound);
  auto value = unif(re);
  if (canBeZero == false && value == 0)
  {
    return RealRandomNumber<T>(canBeZero);
  }
  return value;
}

template<typename T>
T IntRandomNumber( bool canBeZero = true )
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  
  std::uniform_int_distribution<T> unif(lower_bound, upper_bound);
  auto value =  unif(re);
  if (canBeZero == false && value == 0 )
  {
    return IntRandomNumber<T>(canBeZero);
  }
  return value;
}

template<typename T>
T CharRandom()
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  T lower_bound = std::numeric_limits<T>::min();
  T upper_bound = std::numeric_limits<T>::max();
  std::uniform_int_distribution<int> unif(lower_bound, upper_bound);
  return T(unif(re));
}

inline bool BoolRandomNumber()
{

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  std::uniform_int_distribution<int> unif(0, 1);
  return (unif(re) != 0);
}

inline std::wstring Uuid()
{
  return boost::lexical_cast<std::wstring>(boost::uuids::random_generator()());
}

enum ExpectedIpcDataType : char
{
  None = 0,
  Guid = 1,
  Int32 = 2,
  Int64 = 3,
  String = 4,
  StringAscii = 5,
};