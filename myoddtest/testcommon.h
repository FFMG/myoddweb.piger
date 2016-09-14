#pragma once

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <limits>
#include <string>
#include <boost/lexical_cast.hpp>

#include <chrono>
#include <random>
#include <time.h>

#ifndef UNUSED_ALWAYS
# define UNUSED_ALWAYS(x) x
#endif

// number of time we want to run the loops.
#ifdef _DEBUG
# define NUMBER_OF_TESTS 30
#else
# define NUMBER_OF_TESTS 100
#endif

#undef max
#undef min

# define MEMORY_GUARD_START                                                     \
_CrtMemState memState_;                                                         \
_CrtMemCheckpoint(&memState_);                                                  \
{

# define MEMORY_GUARD_END                                                       \
}                                                                               \
_CrtMemState stateNow, stateDiff;                                               \
_CrtMemCheckpoint(&stateNow);                                                   \
int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);          \
UNUSED_ALWAYS(stateNow);                                                        \
if (diffResult)                                                                 \
{                                                                               \
  FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";   \
}

#define GTEST_TEST_MEM_LOOP_(test_case_name, test_name, test_number_of,         \
                             parent_class, parent_id)                           \
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class   \
{                                                                               \
 public:                                                                        \
  GTEST_TEST_CLASS_NAME_(test_case_name, test_name)()                           \
  {                                                                             \
    _CrtMemCheckpoint(&memState_);                                              \
  }                                                                             \
                                                                                \
  virtual ~GTEST_TEST_CLASS_NAME_(test_case_name, test_name)()                  \
  {                                                                             \
    if( !::testing::Test::HasFailure() )                                        \
    {                                                                           \
      _CrtMemState stateNow, stateDiff;                                         \
      _CrtMemCheckpoint(&stateNow);                                             \
      int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);    \
      UNUSED_ALWAYS(stateNow);                                                  \
      if (diffResult)                                                           \
      {                                                                         \
        ADD_FAILURE() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";  \
      }                                                                         \
    }                                                                           \
  }                                                                             \
                                                                                \
 private:                                                                       \
  _CrtMemState memState_;                                                       \
  virtual void TestBody();                                                      \
  void TestBodySingle();                                                        \
  static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;         \
  GTEST_DISALLOW_COPY_AND_ASSIGN_(                                              \
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name));                       \
};                                                                              \
                                                                                \
::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)    \
  ::test_info_ =                                                                \
    ::testing::internal::MakeAndRegisterTestInfo(                               \
        #test_case_name, #test_name, NULL, NULL,                                \
        ::testing::internal::CodeLocation(__FILE__, __LINE__),                  \
        (parent_id),                                                            \
        parent_class::SetUpTestCase,                                            \
        parent_class::TearDownTestCase,                                         \
        new ::testing::internal::TestFactoryImpl<                               \
            GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);                \
                                                                                \
void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()              \
{                                                                               \
  for( auto i = 0; i < test_number_of; ++i )                                    \
  {                                                                             \
    TestBodySingle();                                                           \
    if(test_number_of > 1 )                                                     \
        printf( "    [%s : %d]\n", #test_case_name, i);                         \
    if( ::testing::Test::HasFailure() )                                         \
    {                                                                           \
      break;                                                                    \
    }                                                                           \
  }                                                                             \
}                                                                               \
                                                                                \
void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBodySingle()

# define TEST_MEM(test_case_name, test_name)                                    \
  GTEST_TEST_MEM_LOOP_(test_case_name, test_name, 1,                            \
                       ::testing::Test, ::testing::internal::GetTestTypeId())

# define TEST_MEM_LOOP(test_case_name, test_name, test_number_of)               \
  GTEST_TEST_MEM_LOOP_(test_case_name, test_name, test_number_of,               \
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
T IntRandomNumber( T lower_bound, T upper_bound, bool canBeZero = true)
{
  if (lower_bound >= upper_bound)
  {
    throw std::runtime_error("Nonsensical values passed");
  }

  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine generator(seed);
  std::default_random_engine re(seed);

  std::uniform_int_distribution<T> unif(lower_bound, upper_bound);
  auto value = unif(re);
  if (canBeZero == false && value == 0)
  {
    return IntRandomNumber<T>(lower_bound, upper_bound, canBeZero);
  }
  return value;
}

inline time_t RandomAbsoluteExpiry( time_t min = -1, bool moreThanAYear = false )
{
  // One astronomical year of a single rotation around the sun, has 365.25 days:
  // (365.25 days/year) × (24 hours/day) × (3600 seconds/hour) = 31557600 seconds/year
  static const unsigned int oneAstronomicalYear = 31557600;

  time_t now;
  time(&now);

#pragma warning(push)
#pragma warning( disable : 4996)
  auto newyear = *localtime(&now);
#pragma warning( pop )  

  newyear.tm_year += 1;
  auto oneYear = mktime(&newyear);

  for (;;)
  {
    auto absoluteExpiration = IntRandomNumber<time_t>( min, oneYear );

    // is the time more than one year?
    auto seconds = difftime(absoluteExpiration, now);
    if (seconds > oneAstronomicalYear && moreThanAYear)
    {
      return absoluteExpiration;
    }
    if (seconds <= oneAstronomicalYear && !moreThanAYear)
    {
      return absoluteExpiration;
    }
  }
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