#pragma once

#include <limits>
#include <string>

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
                                                                                \
  GTEST_TEST_CLASS_NAME_(test_case_name, test_name)(                            \
                       GTEST_TEST_CLASS_NAME_(test_case_name, test_name) const&)\
                                                             = delete;          \
  void operator=(GTEST_TEST_CLASS_NAME_(test_case_name, test_name) const &)     \
                                                             = delete;          \
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

inline time_t RandomAbsoluteExpiry( time_t min = 0, bool moreThanAYear = false )
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
  // In its canonical textual representation, the 16 octets of a UUID are represented as 32 hexadecimal (base-16) digits, 
  // displayed in five groups separated by hyphens, in the form 8-4-4-4-12 for a total of 36 characters (32 hexadecimal characters and 4 hyphens).
  const wchar_t hex_characters[] = { L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'a',L'b',L'c',L'd',L'e',L'f' };
  const auto lengths = { 8, 4, 4, 4, 12 };

  std::vector<std::wstring> uuids;
  for (auto& length : lengths)
  {
    std::wstring part;
    for (auto& length : lengths)
    {
      auto hex = hex_characters[std::rand() % 16];
      part += hex;
    }
    uuids.push_back( part );
  }
  std::wstring uuid;
  for (auto& uuid_part : uuids)
  {
    uuid += uuid_part;
    uuid += L'-';
  }
  return uuid;
}

inline std::wstring RandomWideString( size_t len )
{
  if (len == 0)
  {
    return L"";
  }
  std::wstring result = L"";
  const std::wstring a = L"abcdefghejklmnopqrstuvwxyz";
  const auto la = (int)a.length() - 1;

  const std::wstring b = L"abcdefghejklmnopqrstuvwxyz1234567890-";
  const auto lb = (int)b.length() - 1;

  for (auto i = 0; i < len; ++i)
  {
    result += (i == 0 || i == (len-1)) ? a[IntRandomNumber<int>(0, la)] : b[IntRandomNumber<int>(0, lb)];
  }
  return result;
}

inline std::string RandomString(size_t len)
{
  if (len == 0)
  {
    return "";
  }
  std::string result = "";
  const std::string a = "abcdefghejklmnopqrstuvwxyz1234567890-";
  const auto la = (int)a.length() - 1;

  for (auto i = 0; i < len; ++i)
  {
    result += a[IntRandomNumber<int>(0, la)];
  }
  return result;
}

inline std::wstring XmlElementName()
{
  const size_t len = 10;
  std::wstring result = L"";
  const std::wstring a = L"abcdefghejklmnopqrstuvwxyz";
  const auto la = (int)a.length() - 1;

  const std::wstring b = L"abcdefghejklmnopqrstuvwxyz1234567890-";
  const auto lb = (int)b.length() - 1;

  for (auto i = 0; i < len; ++i)
  {
    result += (i == 0 || i == (len - 1)) ? a[IntRandomNumber<int>(0, la)] : b[IntRandomNumber<int>(0, lb)];
  }
  return result;
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