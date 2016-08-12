#include "cache/cacheitem.h"
#include <gtest/gtest.h>

#include "../testcommon.h"

TEST(BasicCacheItem, SetKey)
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str() );
  ASSERT_EQ(key, ci.Key());

  auto newKey = Uuid();
  ci.Key(newKey.c_str() );

  ASSERT_EQ(newKey, ci.Key());
}

TEST(BasicCacheItem, SetKeyAndRegionName)
{
  auto key = Uuid();
  auto regionName = Uuid();
  myodd::cache::CacheItem ci(key.c_str(), nullptr, regionName.c_str() );
  ASSERT_EQ(key, ci.Key());
  ASSERT_EQ(regionName, ci.RegionName());

  auto newKey = Uuid();
  ci.Key(newKey.c_str());

  auto newRegionName = Uuid();
  ci.RegionName(newRegionName.c_str());

  ASSERT_EQ(newKey, ci.Key());
  ASSERT_EQ(newRegionName, ci.RegionName());
}

TEST(BasicCacheItem, RegionNameIsNullByDefault)
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str());
  ASSERT_EQ(nullptr, ci.RegionName());
}

TEST(BasicCacheItem, RegionNameCanBeSetToNull)
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str(), nullptr, nullptr );
  ASSERT_EQ(nullptr, ci.RegionName());
}

TEST(BasicCacheItem, KeyCanBeNullInConstructor)
{
  myodd::cache::CacheItem ci( nullptr );
  ASSERT_EQ(nullptr, ci.Key());

  auto newKey = Uuid();
  ci.Key(newKey.c_str());

  ASSERT_EQ(newKey, ci.Key());
}

TEST(BasicCacheItem, KeyCanBeSetToNull)
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str());
  ASSERT_EQ(key, ci.Key());

  auto newKey = nullptr;
  ci.Key( newKey );

  ASSERT_EQ(newKey, ci.Key());
}

TEST(BasicCacheItem, DefaultValueIsNull )
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str());
  ASSERT_EQ(nullptr, ci.Value<const char *>());
}

TEST(BasicCacheItem, DefaultValueIsNotNull)
{
  auto key = Uuid();
  myodd::cache::CacheItem ci(key.c_str());
  ASSERT_EQ(nullptr, ci.Value<const char *>());
}
