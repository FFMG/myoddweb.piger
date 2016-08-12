#include "cache/memorycache.h"
#include <gtest/gtest.h>

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