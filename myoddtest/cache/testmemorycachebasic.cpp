#include "cache/memorycache.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicMemoryTests, InvalidNameLowerCase)
{
  EXPECT_THROW( new myodd::cache::MemoryCache( L"default"), std::exception);
}

TEST(BasicMemoryTests, InvalidNameUpperCase)
{
  EXPECT_THROW(new myodd::cache::MemoryCache(L"default"), std::exception);
}

TEST(BasicMemoryTests, InvalidNameAnyCase)
{
  EXPECT_THROW(new myodd::cache::MemoryCache(L"DefAuLt"), std::exception);
}

TEST(BasicMemoryTests, NameCannotBeEmpty)
{
  EXPECT_THROW(new myodd::cache::MemoryCache(L""), std::exception);
}

TEST(BasicMemoryTests, NameCannotBeNull)
{
  EXPECT_THROW(new myodd::cache::MemoryCache( nullptr ), std::exception);
}

TEST(BasicMemoryTests, ContainsKeyCannotBeNull)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.Contains( nullptr ), std::invalid_argument);
}

TEST(BasicMemoryTests, ContainsRegionNameMustBeNull)
{
  auto keyCacheItem = Uuid();
  auto regionName = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.Contains(keyCacheItem.c_str(), regionName.c_str() ), std::invalid_argument);
}

TEST(BasicMemoryTests, CannotPassARegionWithGetCount)
{
  auto regionName = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.GetCount( regionName.c_str()), std::invalid_argument);
}

TEST(BasicMemoryTests, DoesNotContain )
{
  auto keyCacheItem = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_FALSE(mc.Contains(keyCacheItem.c_str() ));
}

TEST(BasicMemoryTests, DoesNotContainThenAddThenContains )
{
  auto keyCacheItem = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_FALSE(mc.Contains(keyCacheItem.c_str()));

  auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), 10);
  myodd::cache::CacheItemPolicy policy;
  mc.Add(ci, policy);
  ASSERT_TRUE(mc.Contains(keyCacheItem.c_str()));
}

TEST(BasicMemoryTests, AddingTwiceDoesNotRemoveIt)
{
  auto keyCacheItem = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_FALSE(mc.Contains(keyCacheItem.c_str()));

  auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), 10);
  myodd::cache::CacheItemPolicy policy;
  mc.Add(ci, policy);
  ASSERT_TRUE(mc.Contains(keyCacheItem.c_str()));

  // add it again
  mc.Add(ci, policy);

  //  should still be here.
  ASSERT_TRUE(mc.Contains(keyCacheItem.c_str()));
}

TEST_MEM_LOOP(BasicMemoryTests, JustAddOne, NUMBER_OF_TESTS)
{
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), IntRandomNumber<int>() );
  myodd::cache::CacheItemPolicy policy;

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(ci, policy ));
}

TEST_MEM_LOOP(BasicMemoryTests, JustAddItemMoreThanOnce, NUMBER_OF_TESTS)
{
  myodd::cache::CacheItemPolicy policy;

  auto keyCacheItem = Uuid();
  auto ci1 = myodd::cache::CacheItem(keyCacheItem.c_str(), IntRandomNumber<int>());
  auto ci2 = myodd::cache::CacheItem(keyCacheItem.c_str(), IntRandomNumber<int>());

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(ci1, policy));

  // we cannot add another item with the same key name
  ASSERT_FALSE(mc.Add(ci2, policy));
}

TEST_MEM_LOOP(BasicMemoryTests, JustAddItemMoreThanOnceUsingKeyName, NUMBER_OF_TESTS)
{
  myodd::cache::CacheItemPolicy policy;

  auto keyCacheItem = Uuid();

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(keyCacheItem.c_str(), IntRandomNumber<int>(), policy));

  // we cannot add another item with the same key name
  ASSERT_FALSE(mc.Add(keyCacheItem.c_str(), IntRandomNumber<int>(), policy));
}

TEST_MEM(BasicMemoryTests, CannotAddWithNullKey )
{
  myodd::cache::CacheItemPolicy policy;

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_THROW(mc.Add(nullptr, IntRandomNumber<int>(), policy), std::invalid_argument );
}

TEST_MEM(BasicMemoryTests, CannotAddWithNullValue )
{
  myodd::cache::CacheItemPolicy policy;

  auto key = Uuid();
  auto keyCacheItem = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), nullptr, policy), std::invalid_argument);
}

TEST_MEM(BasicMemoryTests, AddItemsAndCheckNumbers )
{
  myodd::cache::CacheItemPolicy policy;

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_EQ(0, mc.GetCount());

  for (auto i = 0; i < 10; ++i)
  {
    // we have not added it yet.
    ASSERT_EQ(i, mc.GetCount());

    auto keyCacheItem = Uuid();
    auto value = IntRandomNumber<int>();
    auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), value);

    //  add it and check the numbers.
    ASSERT_TRUE(mc.Add(ci, policy));
    ASSERT_EQ(i+1, mc.GetCount());

    // adding it again does not change the count.
    ASSERT_FALSE(mc.Add(ci, policy));
    ASSERT_EQ(i+1, mc.GetCount());
  }
}

TEST(BasicMemoryTests, GetCacheItemKeyCannotBeNull)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.GetCacheItem(nullptr), std::invalid_argument);
}

TEST(BasicMemoryTests, GetCacheItemRegionNameMustBeNull)
{
  auto keyCacheItem = Uuid();
  auto regionName = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.GetCacheItem(keyCacheItem.c_str(), regionName.c_str()), std::invalid_argument);
}

TEST(BasicMemoryTests, GetACacheItemThatDoesNotExist)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  //  add an item
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), 10);

  auto anotherKeyCacheItem = Uuid();
  ASSERT_EQ( nullptr, mc.GetCacheItem(anotherKeyCacheItem.c_str() ) );
}

TEST(BasicMemoryTests, GetGetCacheItemAndCheckValues)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  //  add an item
  myodd::cache::CacheItemPolicy policy;
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem(keyCacheItem.c_str(), 10);
  mc.Add(ci, policy);

  auto cacheitem = mc.GetCacheItem(keyCacheItem.c_str());
  ASSERT_EQ( 10, cacheitem->Value() );
  ASSERT_EQ( 10.f, cacheitem->Value() );
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiry, NUMBER_OF_TESTS )
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  time_t now;
  time(&now);

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = RandomAbsoluteExpiry(now, false);
  ASSERT_TRUE(absoluteExpiration >= 0);

  auto keyCacheItem = Uuid();
  ASSERT_TRUE( mc.Add(keyCacheItem.c_str(), value, absoluteExpiration) );

  auto cacheitem = mc.GetCacheItem(keyCacheItem.c_str());
  ASSERT_EQ(value, cacheitem->Value());
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiryAndNullRegion, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  time_t now;
  time(&now);

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = RandomAbsoluteExpiry(now, false);
  ASSERT_TRUE(absoluteExpiration >= 0);

  auto keyCacheItem = Uuid();
  ASSERT_TRUE(mc.Add(keyCacheItem.c_str(), value, absoluteExpiration, nullptr ));

  auto cacheitem = mc.GetCacheItem(keyCacheItem.c_str());
  ASSERT_EQ(value, cacheitem->Value());
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiryAndNonNullRegion, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  time_t now;
  time(&now);

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = RandomAbsoluteExpiry(now, false);
  ASSERT_TRUE(absoluteExpiration >= 0);

  auto keyCacheItem = Uuid();
  auto regionCacheItem = Uuid();
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), value, absoluteExpiration, regionCacheItem.c_str()), std::invalid_argument );
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithNegativeAbsoluteExpiry, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = IntRandomNumber<__int64>();
  while (absoluteExpiration >= 0)
  {
    absoluteExpiration = IntRandomNumber<__int64>();
  }

  auto keyCacheItem = Uuid();
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), value, absoluteExpiration), std::invalid_argument);
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiryAndCheckValue, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  time_t now;
  time(&now);

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = RandomAbsoluteExpiry(now, false);
  ASSERT_TRUE(absoluteExpiration >= 0);

  auto keyCacheItem = Uuid();
  ASSERT_TRUE( mc.Add(keyCacheItem.c_str(), value, absoluteExpiration) );
  
  auto item = mc.GetCacheItem(keyCacheItem.c_str() );
  ASSERT_EQ(value, item->Value());
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiryButNullKey, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = IntRandomNumber<__int64>();
  while (absoluteExpiration < 0)
  {
    absoluteExpiration = IntRandomNumber<__int64>();
  }

  ASSERT_THROW(mc.Add(nullptr, value, absoluteExpiration), std::invalid_argument);
}

TEST_MEM_LOOP(BasicMemoryTests, AddWithAbsoluteExpiryButNullValue, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  auto value = IntRandomNumber<int>();
  auto absoluteExpiration = IntRandomNumber<__int64>();
  while (absoluteExpiration < 0)
  {
    absoluteExpiration = IntRandomNumber<__int64>();
  }

  auto keyCacheItem = Uuid();
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), nullptr, absoluteExpiration), std::invalid_argument);
}

TEST_MEM_LOOP(BasicMemoryTests, AddAbsoluteExpiryIsInThePast, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  auto value = IntRandomNumber<int>();

  time_t now;
  time(&now);
#pragma warning(push)
#pragma warning( disable : 4996)
  auto inThePast = *localtime(&now);
#pragma warning( pop )  

  auto days = IntRandomNumber<int>(1, 365);
  inThePast.tm_mday -= days;
  auto thePast = mktime(&inThePast);

  auto keyCacheItem = Uuid();
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), value, thePast), std::invalid_argument);
}

TEST_MEM_LOOP(BasicMemoryTests, AddAbsoluteExpiryIsMoreThanOneYear, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  auto value = IntRandomNumber<int>();

  time_t now;
  time(&now);
#pragma warning(push)
#pragma warning( disable : 4996)
  auto inTheFuture = *localtime(&now);
#pragma warning( pop )  

  auto days = IntRandomNumber<int>(366, 500);
  inTheFuture.tm_mday += days;
  auto theFuture = mktime(&inTheFuture);

#pragma warning(push)
#pragma warning( disable : 4996)
  inTheFuture = *localtime(&theFuture);
#pragma warning( pop ) 

  auto keyCacheItem = Uuid();
  ASSERT_THROW(mc.Add(keyCacheItem.c_str(), value, theFuture), std::invalid_argument);
}
