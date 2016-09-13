#include "cache/cacheitempolicy.h"
#include <gtest/gtest.h>

#include <limits.h>

#include "../testcommon.h"

TEST_MEM(CacheItemPolicy, CreateWithDefaultValues )
{
  auto policy = ::myodd::cache::CacheItemPolicy();
  ASSERT_EQ(std::numeric_limits<__int64>::max(), policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, CreateWithGivenAbsoluteExipration, NUMBER_OF_TESTS )
{
  auto absoluteExipration = IntRandomNumber<__int64>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<__int64>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());
}

TEST_MEM_LOOP(CacheItemPolicy, NegativeAbsoluteExpirationThrowsError, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<__int64>();
  while (absoluteExipration >= 0)
  {
    absoluteExipration = IntRandomNumber<__int64>();
  }
  EXPECT_THROW( ::myodd::cache::CacheItemPolicy(absoluteExipration), std::runtime_error );
}

TEST_MEM_LOOP(CacheItemPolicy, CannotSetNegativeAbsoluteExpirationThrowsError, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<__int64>();
  while (absoluteExipration >= 0)
  {
    absoluteExipration = IntRandomNumber<__int64>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy();
  EXPECT_THROW(policy.SetAbsoluteExpiration(absoluteExipration), std::runtime_error);
}

TEST_MEM_LOOP(CacheItemPolicy, ChangeAbsoluteExpiration, NUMBER_OF_TESTS)
{
  auto absoluteExipration = IntRandomNumber<__int64>();
  while (absoluteExipration < 0)
  {
    absoluteExipration = IntRandomNumber<__int64>();
  }
  auto policy = ::myodd::cache::CacheItemPolicy();
  policy.SetAbsoluteExpiration(absoluteExipration);
  ASSERT_EQ(absoluteExipration, policy.GetAbsoluteExpiration());
}