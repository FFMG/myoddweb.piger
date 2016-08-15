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

TEST(BasicMemoryTests, AddItemsAndCheckNumbers )
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
    auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);

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
  EXPECT_THROW(mc.GetCacheItem<int>(nullptr), std::invalid_argument);
}

TEST(BasicMemoryTests, GetCacheItemRegionNameMustBeNull)
{
  auto keyCacheItem = Uuid();
  auto regionName = Uuid();
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());
  EXPECT_THROW(mc.GetCacheItem<int>(keyCacheItem.c_str(), regionName.c_str()), std::invalid_argument);
}

TEST(BasicMemoryTests, GetACacheItemThatDoesNotExist)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  //  add an item
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);

  auto anotherKeyCacheItem = Uuid();
  EXPECT_THROW(mc.GetCacheItem<int>(anotherKeyCacheItem.c_str() ), std::out_of_range );
}

TEST(BasicMemoryTests, GetGetCacheItemAndCheckValues)
{
  auto key = Uuid();
  myodd::cache::MemoryCache mc(key.c_str());

  //  add an item
  myodd::cache::CacheItemPolicy policy;
  auto keyCacheItem = Uuid();
  auto ci = myodd::cache::CacheItem<int>(keyCacheItem.c_str(), 10);
  mc.Add(ci, policy);

  auto x = mc.GetCacheItem<float>(keyCacheItem.c_str());
  //ASSERT_EQ( 10, mc.GetCacheItem<int>(keyCacheItem.c_str()) );
  // ASSERT_EQ( 10.f, mc.GetCacheItem<float>(keyCacheItem.c_str()));
}