#include "cache/cacheitempolicy.h"
#include <gtest/gtest.h>

#include <limits.h>

#include "../testcommon.h"

TEST_MEM(CacheItemPolicy, CreateWithDefaultValues )
{
  auto policy = ::myodd::cache::CacheItemPolicy();
  ASSERT_EQ(std::numeric_limits<time_t>::max(), policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, CreateWithGivenAbsoluteExipration, NUMBER_OF_TESTS )
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, NegativeAbsoluteExpirationThrowsError, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration >= 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  ASSERT_THROW(auto policy = ::myodd::cache::CacheItemPolicy(absoluteExipration), std::runtime_error );
}

TEST_MEM_LOOP(CacheItemPolicy, CannotSetNegativeAbsoluteExpirationThrowsError, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration >= 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy();
  ASSERT_THROW(policy.SetAbsoluteExpiration(absoluteExipration), std::runtime_error);
}

TEST_MEM_LOOP(CacheItemPolicy, ChangeAbsoluteExpiration, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy();
  policy.SetAbsoluteExpiration(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, CreateThenChangeAbsoluteExipration, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());

  // change it now
  absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  policy.SetAbsoluteExpiration(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, CreateThenChangeToNegativeAbsoluteExipration, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());

  // change it now
  absoluteExipration = IntRandomNumber<time_t>();
  while (absoluteExipration >= 0)
  {
    absoluteExipration = IntRandomNumber<time_t>();
  }
  ASSERT_THROW(policy.SetAbsoluteExpiration(absoluteExipration), std::runtime_error);
}
