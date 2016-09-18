// ***********************************************************************
// Copyright (c) 2016 Florent Guelfucci
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// @see https://opensource.org/licenses/MIT
// ***********************************************************************
#pragma once

#include <string>
#include "cacheitem.h"
#include "cacheitempolicy.h"

#include <mutex>  // locks
#include <map>    // all the items.
#include <memory> // unique_ptr
#include <time.h> // time_t
#include <future>

namespace myodd {
  namespace cache {
    /**
     * C++ implementation of .NET MemoryCache
     * The objective(s) of this class are
     *   - Be thread safe
     *   - Be fast
     *   - 'Look' similar to the .NET implementatation as much as posible.
     *   - be c++14 compliant and require no other libraries.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache(v=vs.110).aspx
     */ 
    class MemoryCache
    {
    public:
      MemoryCache( const wchar_t* name );
      virtual ~MemoryCache();

    public:
      // Methods

      /**
       * inserts a cache entry into the cache, specifying information about how the entry will be evicted.
       * @see https://msdn.microsoft.com/en-us/library/hh138343(v=vs.110).aspx
       * @param const CacheItem& item the item to add.
       * @param CacheItemPolicy policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
       * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
       */
      virtual bool Add(const CacheItem& item, const CacheItemPolicy& policy);

      /**
       * inserts a cache entry into the cache, specifying information about how the entry will be evicted.
       * @see https://msdn.microsoft.com/en-us/library/ee395902(v=vs.110).aspx
       * @param const wchar_t* key A unique identifier for the cache entry.
       * @param const ::myodd::dynamic::Any& value the value we want to add
       * @param const CacheItemPolicy& policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
       * @param const wchar_t* regionName = nullptr Optional. A named region in the cache to which the cache entry can be added, if regions are implemented. The default value for the optional parameter is null.
       * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
       */
      virtual bool Add(const wchar_t* key, const ::myodd::dynamic::Any& value, const CacheItemPolicy& policy, const wchar_t* regionName = nullptr);

      /**
       * Adds a cache entry into the cache using the specified key and a value and an absolute expiration value
       * @see https://msdn.microsoft.com/en-us/library/dd780614(v=vs.110).aspx
       * @param const wchar_t* key A unique identifier for the cache entry.
       * @param const ::myodd::dynamic::Any& value the object to insert
       * @param time_t absoluteExpiration The fixed date and time at which the cache entry will expire. This parameter is required when the Add method is called.
       * @param const wchar_t* regionName A named region in the cache to which the cache entry can be added,
       * @return boolean true if insertion succeeded, or false if there is an already an entry in the cache that has the same key as key. 
       */
      virtual bool Add(const wchar_t* key, const ::myodd::dynamic::Any& value, time_t absoluteExpiration, const wchar_t* regionName = nullptr );

      /**
       * @param const CacheItem& item the object to add
       * @param const CacheItemPolicy& policy
       * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
       */
      virtual const CacheItem* AddOrGetExisting( const CacheItem& item, const CacheItemPolicy& policy);

      /**
       * Inserts a cache entry into the cache using the specified key and value and the specified details for how it is to be evicted.
       * @param const wchar_t* key A unique identifier for the cache entry to add or get. 
       * @param const ::myodd::dynamic::Any& value The data for the cache entry.
       * @param const CacheItemPolicy& policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
       * @param const wchar_t* regionName A named region in the cache to which a cache entry can be added. Do not pass a value for this parameter. By default, this parameter is null
       * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
       */
      virtual const CacheItem* AddOrGetExisting(const wchar_t* key, const ::myodd::dynamic::Any& value, const CacheItemPolicy& policy, const wchar_t* regionName = nullptr);

      /**
       * Adds a cache entry into the cache using the specified key and a value and an absolute expiration value.
       * @param const wchar_t* key A unique identifier for the cache entry to add or get.
       * @param const ::myodd::dynamic::Any& value The data for the cache entry.
       * @param time_t absoluteExpiration The fixed date and time at which the cache entry will expire
       * @param const wchar_t* regionName A named region in the cache to which a cache entry can be added. Do not pass a value for this parameter. By default, this parameter is null
       * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
       */
      virtual const CacheItem* AddOrGetExisting(const wchar_t* key, const ::myodd::dynamic::Any& value, time_t absoluteExpiration, const wchar_t* regionName = nullptr);

      /**
       * Determines whether a cache entry exists in the cache.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.contains(v=vs.110).aspx
       */
      bool Contains(const wchar_t* key, const wchar_t* regionName = nullptr) const;

      /**
       * Returns the total number of cache entries in the cache.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.getcount(v=vs.110).aspx
       */
      size_t GetCount(const wchar_t* regionName = nullptr) const;

      /**
       * Returns an entry from the cache or null. 
       * @param const wchar_t* key
       * @param const wchar_t* regionName
       * @return ::myodd::dynamic::Any* the value or null.
       */
      virtual const ::myodd::dynamic::Any Get(const wchar_t* key, const wchar_t* regionName = nullptr) const;

      /**
       * Returns the specified entry from the cache as a CacheItem instance.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.getcacheitem(v=vs.110).aspx
       * @throw std::out_of_range if the item is not found.
       * @throw std::invalid_argument if the key is null or if regionname is not null.
       * @param A unique identifier for the cache entry to get.
       * @param A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
       * @return const CacheItem& the cache item, we will throw if the item does not exist.
       */
      virtual const CacheItem* GetCacheItem(const wchar_t* key, const wchar_t* regionName = nullptr) const;

      /**
       * Releases all resources that are used by the current instance of the MemoryCache class.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.dispose(v=vs.110).aspx
       */
      void Dispose();

    protected:
      struct CacheItemAndPolicy
      {
        CacheItem _item;
        CacheItemPolicy _policy;
      };

      // all our items and policies.
      typedef std::map< std::wstring, CacheItemAndPolicy > CacheItemsAndPolicies;

      /** 
       * Get all the cache items and policies. 
       */
      inline CacheItemsAndPolicies& GetCacheItemAndPolicy() { return _cacheItems; }
      inline const CacheItemsAndPolicies& GetCacheItemAndPolicy() const { return _cacheItems; }

    private:
      CacheItemsAndPolicies _cacheItems;

      // the mutex to protect this data
      // it is mutable so we can use it inside const functions.
      mutable std::mutex _mutex;

      // the name of this collection
      const std::wstring _name;

      // nake sure that the name is valid
      // we throw if there is an error.
      void ValidateName() const;

      // make sure that the given key is valid
      void ValidateKey(const wchar_t* givenKey) const;

      // make sure that the region name is valid
      void ValidateRegionName(const wchar_t* givenRegionName) const;

      // make sure that the given key is valid
      void ValidateValue( const ::myodd::dynamic::Any& givenValue) const;

      // make sure that the validation is not in the past and more than 12 months in the future.
      void ValidateAbsoluteExpiration(time_t absoluteExpiration) const;

      // remove all the expired items.
      void RemoveExpired();

      // Lock that we use all the time to guard our data.
      class Lock
      {
      public:
        Lock(std::mutex& mutex);
        virtual ~Lock();

        void Release();

      private:
        std::mutex& _mutex;
        std::lock_guard<std::mutex>* _guard;
      };

      // the timer thread
      std::future<void> _absoluteExpirationTimerThread;

      // call the timer
      void AbsoluteExpirationTimer();
    };
  }
}