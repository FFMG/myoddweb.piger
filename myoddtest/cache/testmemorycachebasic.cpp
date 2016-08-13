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

TEST(BasicMemoryTests, CannotBeEmpty)
{
  EXPECT_THROW(new myodd::cache::MemoryCache(L""), std::exception);
}

TEST(BasicMemoryTests, CannotBeNull)
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

  auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);
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

  auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);
  myodd::cache::CacheItemPolicy policy;
  mc.Add(ci, policy);
  ASSERT_TRUE(mc.Contains(keyCacheItem.c_str()));

  // add it again
  mc.Add(ci, policy);

  //  should still be here.
  ASSERT_TRUE(mc.Contains(keyCacheItem.c_str()));
}

TEST(BasicMemoryTests, JustAddOne)
{
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10 );
  myodd::cache::CacheItemPolicy policy;

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(ci, policy ));
}

TEST(BasicMemoryTests, JustAddItemMoreThanOnce)
{
  myodd::cache::CacheItemPolicy policy;

  auto keyCacheItem = Uuid();
  auto ci1 = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);
  auto ci2 = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 20);

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(ci1, policy));

  // we cannot add another item with the same key name
  ASSERT_FALSE(mc.Add(ci2, policy));
}

TEST(BasicMemoryTests, JustAddItemMoreThanOnceUsingKeyName)
{
  myodd::cache::CacheItemPolicy policy;

  auto keyCacheItem = Uuid();

  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  ASSERT_TRUE(mc.Add(keyCacheItem.c_str(), 10, policy));

  // we cannot add another item with the same key name
  ASSERT_FALSE(mc.Add(keyCacheItem.c_str(), 40, policy));
}