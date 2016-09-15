#include "cache/memorycache.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

class MyCache : public myodd::cache::MemoryCache
{
public:
  MyCache(const wchar_t* name) : myodd::cache::MemoryCache(name) {}

  void Expire(const wchar_t* key) {
    CacheItemsAndPolicies& ci = GetCacheItemAndPolicy();
    auto it = ci.find(key);
    if (it == ci.end())
    {
      return;
    }

    time_t now;
    time(&now);
#pragma warning(push)
#pragma warning( disable : 4996)
    auto inThePast = *localtime(&now);
#pragma warning( pop )  

    auto days = IntRandomNumber<int>(1, 365);
    inThePast.tm_mday -= days;
    auto thePast = mktime(&inThePast);

    myodd::cache::CacheItemPolicy& policy = it->second._policy;
    policy.SetAbsoluteExpiration(thePast);
  }
};

TEST(ExpiredCacheItem, CheckThatExpiredItemReturnsNull)
{
  auto key = Uuid();
  MyCache mc(key.c_str());

  // add an item
  auto keyCacheItem = Uuid();
  auto value = IntRandomNumber<int>();
  mc.Add(keyCacheItem.c_str(), value, myodd::cache::CacheItemPolicy());

  {
    auto item = mc.Get(keyCacheItem.c_str(), nullptr);
    ASSERT_EQ(value, item);
  }

  // expire it.
  mc.Expire(keyCacheItem.c_str());

  // we should no longer be able to get it.
  {
    auto item = mc.Get(keyCacheItem.c_str(), nullptr);
    ASSERT_EQ(nullptr, item);
  }
}

TEST_MEM_LOOP(BasicMemoryTests, GetCountOfExpiredItems, NUMBER_OF_TESTS)
{
  auto key = Uuid();
  MyCache mc(key.c_str());

  //  add a few items.
  typedef std::map< std::wstring, int > Values;
  Values values;
  unsigned int expired = 0;
  int count = IntRandomNumber<int>(50, 100);
  for (int i = 0; i < count; ++i)
  {
    auto keyCacheItem = Uuid();
    auto value = IntRandomNumber<int>();
    values[keyCacheItem] = value;
    mc.Add(keyCacheItem.c_str(), value, myodd::cache::CacheItemPolicy());

    if (BoolRandomNumber() == true)
    {
      mc.Expire(keyCacheItem.c_str());
      ++expired;
    }
  }

  // the size must be coun - expired items.
  ASSERT_EQ((count - expired), mc.GetCount());
}
