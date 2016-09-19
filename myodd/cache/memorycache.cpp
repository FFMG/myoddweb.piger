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
#include "memorycache.h"
#include <algorithm>  //  std::transform
#include <cwctype>    //  std::towlower

#include <functional>
#include <chrono>

#define MEMORYCACHE_EXPIRED_CHECK_SEC 1

namespace myodd {
  namespace cache {
    MemoryCache::Lock::Lock(std::mutex &mutex) :
      _mutex(mutex),
      _guard(nullptr)
    {
      try
      {
        _guard = new std::lock_guard<std::mutex>(_mutex);
      }
      catch (const std::system_error&) {
      }
    }

    /**
    * Free memory, release the lock.
    */
    MemoryCache::Lock::~Lock()
    {
      Release();
    }

    /**
    * Release the lock straightaway.
    */
    void MemoryCache::Lock::Release()
    {
      delete _guard;
      _guard = nullptr;
    }

    /**
     * Initializes a new instance of the MemoryCache class.
     * The value can have spaces but cannot be null and/or empty.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.memorycache(v=vs.110).aspx
     * @param const std::wstring& name The name to use to look up configuration information.
     */
    MemoryCache::MemoryCache(const wchar_t* name) : _name(name ? name : L"")
    {
      ValidateName();
    }

    /**
     * The destructor.
     */
    MemoryCache::~MemoryCache()
    {
      //  reset/clean everthing
      Dispose();
    }

    /**
     * Releases all resources that are used by the current instance of the MemoryCache class.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.dispose(v=vs.110).aspx
     */
    void MemoryCache::Dispose()
    {
      // lock us in in case another thread tries to access that data.
      {
        MemoryCache::Lock guard(_mutex);

        // clear all the cache itmes.
        _cacheItems.clear();
      }// remove the lock

      // wait for the dispose thread to end.
      //  check if the thread is still running.
      for (;;)
      {
        // is the thread running still or is it even valid?
        // if the thread was never started then it will never be valid.
        auto status = _absoluteExpirationTimerThread.valid() ? _absoluteExpirationTimerThread.wait_for(std::chrono::milliseconds(0)) : std::future_status::ready;
        if (status == std::future_status::ready)
        {
          break;
        }
        
        // wait a little for the thread to end.
        std::this_thread::sleep_for(std::chrono::microseconds(100));
      }
    }

    /**
     * Remove all the expired items.
     */
    void MemoryCache::RemoveExpired()
    {
      // lock us in in case another thread tries to access that data.
      MemoryCache::Lock guard(_mutex);

      // start the look
      auto it = _cacheItems.begin();

      // go around and remove all the expired items. 
      while (it != _cacheItems.end())
      {
        if (it->second._policy.HasExpired())
        {
          it = _cacheItems.erase(it);
        }
        else
        {
          ++it;
        }
      }
    }

    /**
     * Make sure that the key value is valid.
     * @param const wchar_t* givenKey the key we want to validate.
     */
    void MemoryCache::ValidateKey(const wchar_t* givenKey) const
    {
      //  the key cannot be null
      if (nullptr == givenKey)
      {
        throw std::invalid_argument("The key name cannot be null!");
      }
    }

    /**
     * Start a thread to check for the expired keys.
     */
    void MemoryCache::AbsoluteExpirationTimer()
    {
      // check if the thread is still running.
      // the default is 'std::future_status::ready' so we can create the thread.
      auto status = _absoluteExpirationTimerThread.valid() ? _absoluteExpirationTimerThread.wait_for(std::chrono::milliseconds(0)) : std::future_status::ready;
      if (status != std::future_status::ready)
      {
        return;
      }

      time_t now;
      time(&now);

      auto _this = this;
      _absoluteExpirationTimerThread = std::async(std::launch::async, [_this]
      {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        for (;;)
        {
          // wait a little.
          std::this_thread::sleep_for(std::chrono::milliseconds(100));

          {
            // lock us in in case another thread tries to access that data.
            MemoryCache::Lock guard(_this->_mutex);

            // if we have nothing left in the thread, then we can bail out.
            // another thread will be started if need be.
            if (_this->_cacheItems.size() == 0)
            {
              break;
            }
          }

          end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsed_seconds = end - start;
          if (elapsed_seconds.count() < MEMORYCACHE_EXPIRED_CHECK_SEC)
          {
            continue;
          }

          // remove the exipred items
          _this->RemoveExpired();

          // this is the new start time
          start = std::chrono::system_clock::now();
        }
      });
    }

    /**
     * Make sure that the date is not in the past and more than 12 month in the future.
     * @param time_t absoluteExpirationthe date we are checking.
     * @return double after how many seconds will this expire.
     */
    void MemoryCache::ValidateAbsoluteExpiration(time_t absoluteExpiration) const
    {
      //  default time means never expires.
      if (absoluteExpiration == std::numeric_limits<time_t>::max())
      {
        return;
      }

      // absolute expiration cannot be negative.
      if (absoluteExpiration < 0)
      {
        throw std::invalid_argument("The absolute time cannot be negative.");
      }

      // get the current time
      time_t now;
      time(&now);

      // is the time more than one year?
      auto seconds = difftime(absoluteExpiration, now);
      if (seconds < 0)
      {
        throw std::invalid_argument( "The expiration time cannot be in the past." );
      }

      // One astronomical year of a single rotation around the sun, has 365.25 days:
      // (365.25 days/year) × (24 hours/day) × (3600 seconds/hour) = 31557600 seconds/year
      static const unsigned int oneAstronomicalYear = 31557600;
      if (seconds > oneAstronomicalYear )
      {
        throw std::invalid_argument("The expiration cannot be set to a value greater than one year.");
      }
    }

    /**
     * Make sure that the region name is null.
     * @param const wchar_t* givenRegionName the given region name.
     */
    void MemoryCache::ValidateRegionName(const wchar_t* givenRegionName) const
    {
      if (nullptr != givenRegionName )
      {
        throw std::invalid_argument("The regionName must be null!");
      }
    }

    /**
     * Make sure that the value is not null
     * const ::myodd::dynamic::Any& givenValue the value to check
     */
    void MemoryCache::ValidateValue(const ::myodd::dynamic::Any& givenValue) const
    {
      // check that the value is not null
      if (nullptr == givenValue )
      {
        throw std::invalid_argument("The value cannot be null!");
      }
    }

    /**
     * Make sure that the given name is valid.
     * It cannot be empty + null. Spaces are alowed, (and should be retained).
     */
    void MemoryCache::ValidateName() const
    {
      if (_name.length() == 0)
      {
        throw std::exception("The length of name cannot be 0");
      }

      // this might not work for all the locals but we are only looking for 'default'.
      auto lowerName = _name;
      std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), std::towlower);
      if (lowerName == L"default")
      {
        //  The string value "default" (case insensitive) is assigned to name.The value "default" cannot be assigned to a new MemoryCache instance, 
        //  because the value is reserved for use by the Default property.
        throw std::exception("The string value \"default\" (case insensitive) is assigned to name.");
      }
    }

    /**
     * Determines whether a cache entry exists in the cache.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.contains(v=vs.110).aspx
     * @param const wchar_t* key A unique identifier for the cache entry to search for.
     * @param const wchar_t* regionName = nullptr A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
     * @return boolean if the cache contains a cache entry whose key matches key; otherwise, false.
     */
    bool MemoryCache::Contains(const wchar_t* key, const wchar_t* regionName /*= nullptr*/) const
    {
      // the key cannot be null
      ValidateKey(key);

      //  the region nust be null
      ValidateRegionName(regionName);

      // lock us in so we don't find data while deleting it, (or something like that).
      MemoryCache::Lock guard(_mutex);

      // now look for the value.
      const auto it = _cacheItems.find(std::wstring(key));

      // have we found anything?
      return (it != _cacheItems.end());
    }

    /**
     * Returns the total number of cache entries in the cache.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.getcount(v=vs.110).aspx
     * @param const wchar_t* regionName A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
     * @return size_t The number of entries in the cache.
     */
    size_t MemoryCache::GetCount(const wchar_t* regionName) const
    {
      //  the region nust be null
      ValidateRegionName(regionName);

      // we must lock so it does not change mid-flight.
      MemoryCache::Lock guard(_mutex);

      // count the number of items that are not expired.
      size_t count = 0;
      for (auto it = _cacheItems.begin(); it != _cacheItems.end(); ++it)
      {
        if (!it->second._policy.HasExpired())
        {
          // not expired.
          ++count;
        }
      }

      //  return the size
      return count;
    }

    /**
    * Returns an entry from the cache or null.
    * @param const wchar_t* key
    * @param const wchar_t* regionName
    * @return ::myodd::dynamic::Any* the value or null.
    */
    const ::myodd::dynamic::Any MemoryCache::Get(const wchar_t* key, const wchar_t* regionName ) const
    {
      // get the lock now so we can hold it until we return the value.
      MemoryCache::Lock guard(_mutex);

      // look for the complete cache item value
      auto cacheItem = GetCacheItem(key, regionName);
      if (nullptr == cacheItem )
      {
        // could not find it.
        return nullptr;
      }

      // return the value.
      return cacheItem->Value();
    }

    /**
     * Returns the specified entry from the cache as a CacheItem instance.
     * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.getcacheitem(v=vs.110).aspx
     * @throw std::out_of_range if the item is not found.
     * @throw std::invalid_argument if the key is null or if regionname is not null.
     * @param A unique identifier for the cache entry to get.
     * @param A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
     * @return const CacheItem& the cache item, we will throw if the item does not exist.
     */
    const CacheItem* MemoryCache::GetCacheItem(const wchar_t* key, const wchar_t* regionName ) const
    {
      // the key cannot be null
      ValidateKey(key);

      //  the region nust be null
      ValidateRegionName(regionName);

      // we now need to lock as this might/could change.
      MemoryCache::Lock guard(_mutex);

      // now look for the value.
      const auto it = _cacheItems.find(std::wstring(key));
      if (it == _cacheItems.end())
      {
        return nullptr;
      }

      return it->second._policy.HasExpired() ? nullptr : &it->second._item;
    }

    /**
     * Adds a cache entry into the cache using the specified key and a value and an absolute expiration value
     * @see https://msdn.microsoft.com/en-us/library/dd780614(v=vs.110).aspx
     * @see AddOrGetExisting( ... )
     * @param const wchar_t* key A unique identifier for the cache entry.
     * @param const ::myodd::dynamic::Any& value the object to insert
     * @param time_t absoluteExpiration The fixed date and time at which the cache entry will expire. This parameter is required when the Add method is called.
     * @param const wchar_t* regionName A named region in the cache to which the cache entry can be added,
     * @return boolean true if insertion succeeded, or false if there is an already an entry in the cache that has the same key as key. 
     */
    bool MemoryCache::Add(const wchar_t* key, const ::myodd::dynamic::Any& value, time_t absoluteExpiration, const wchar_t* regionName )
    {
      // try and add the item or get existing, if it exists, then we return false, (as it exists already).
      return (AddOrGetExisting( key, value, absoluteExpiration, regionName) == nullptr);
    }

    /**
     * inserts a cache entry into the cache, specifying information about how the entry will be evicted.
     * @see https://msdn.microsoft.com/en-us/library/ee395902(v=vs.110).aspx
     * @see AddOrGetExisting( ... )
     * @param const wchar_t* key A unique identifier for the cache entry.
     * @param const ::myodd::dynamic::Any& value the value we want to add
     * @param const CacheItemPolicy& policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
     * @param const wchar_t* regionName = nullptr Optional. A named region in the cache to which the cache entry can be added, if regions are implemented. The default value for the optional parameter is null.
     * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
     */
    bool MemoryCache::Add(const wchar_t* key, const ::myodd::dynamic::Any& value, const CacheItemPolicy& policy, const wchar_t* regionName )
    {
      // try and add the item or get existing, if it exists, then we return false, (as it exists already).
      return (AddOrGetExisting(key, value, policy, regionName) == nullptr);
    }

    /**
     * inserts a cache entry into the cache, specifying information about how the entry will be evicted.
     * @see https://msdn.microsoft.com/en-us/library/hh138343(v=vs.110).aspx
     * @see AddOrGetExisting( ... )
     * @param const CacheItem& item the item to add.
     * @param const CacheItemPolicy& policy policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
     * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
     */
    bool MemoryCache::Add(const CacheItem& item, const CacheItemPolicy& policy)
    {
      // try and add the item.
      // if the return value is 'null' then it means that we 
      // managed to add this item in the list properly.
      return (AddOrGetExisting(item, policy) == nullptr);
    }

    /**
     * @param const CacheItem& item the object to add
     * @param const CacheItemPolicy& policy
     * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
     */
    const CacheItem* MemoryCache::AddOrGetExisting(const CacheItem& item, const CacheItemPolicy& policy)
    {
      // lock the threads so we don't have a race condition.
      MemoryCache::Lock guard(_mutex);

      // look to see if it exists already
      auto it = _cacheItems.find(std::wstring(item.Key()));

      // if we already have it, then we cannot add it.
      if ( _cacheItems.end() != it )
      {
        // we cannot add it again, release the lock on the way out.
        return &it->second._item;
      }

      // we can now add it to our list.
      _cacheItems.emplace(std::make_pair(std::wstring(item.Key()), CacheItemAndPolicy{ item, policy }));

      // if we have max time, then it will never expire.
      if (policy.GetAbsoluteExpiration() != std::numeric_limits<time_t>::max())
      {
        // start the timer if it has not yet started.
        AbsoluteExpirationTimer();
      }

      // success, as we added the item, we return 'null'
      return nullptr;
    }

    /**
     * Inserts a cache entry into the cache using the specified key and value and the specified details for how it is to be evicted.
     * @param const wchar_t* key A unique identifier for the cache entry to add or get.
     * @param const ::myodd::dynamic::Any& value The data for the cache entry.
     * @param const CacheItemPolicy& policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
     * @param const wchar_t* regionName A named region in the cache to which a cache entry can be added. Do not pass a value for this parameter. By default, this parameter is null
     * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
     */
    const CacheItem* MemoryCache::AddOrGetExisting(const wchar_t* key, const ::myodd::dynamic::Any& value, const CacheItemPolicy& policy, const wchar_t* regionName)
    {
      // the key cannot be null
      ValidateKey(key);

      // make sure that the region name is null
      ValidateRegionName(regionName);

      // check that the value is not null
      ValidateValue(value);

      // validate the time
      ValidateAbsoluteExpiration(policy.GetAbsoluteExpiration());

      //  just pass the value to the CacheItem function.
      return AddOrGetExisting(CacheItem(key, value, regionName), policy);
    }

    /**
    * Adds a cache entry into the cache using the specified key and a value and an absolute expiration value.
    * @param const wchar_t* key A unique identifier for the cache entry to add or get.
    * @param const ::myodd::dynamic::Any& value The data for the cache entry.
    * @param time_t absoluteExpiration The fixed date and time at which the cache entry will expire
    * @param const wchar_t* regionName A named region in the cache to which a cache entry can be added. Do not pass a value for this parameter. By default, this parameter is null
    * @return CacheItem If a cache entry with the same key exists, the existing cache entry; otherwise, null.
    */
    const CacheItem* MemoryCache::AddOrGetExisting(const wchar_t* key, const ::myodd::dynamic::Any& value, time_t absoluteExpiration, const wchar_t* regionName)
    {
      // the key cannot be null
      ValidateKey(key);

      // check that the value is not null
      ValidateValue(value);

      // make sure that the date is valid
      ValidateAbsoluteExpiration(absoluteExpiration);

      // the region mame must be null
      ValidateRegionName(regionName);

      return AddOrGetExisting(CacheItem(key, value, regionName), CacheItemPolicy(absoluteExpiration) );
    }

    /**
    * Removes a cache entry from the cache.
    * @param const wchar_t* key A unique identifier for the cache entry to add or get.
    * @param const wchar_t* regionName A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
    * @return ::myodd::dynamic::Any If the entry is found in the cache, the removed cache entry; otherwise, null.
    */
    const ::myodd::dynamic::Any MemoryCache::Remove(const wchar_t* key, const wchar_t* regionName )
    {
      // the key cannot be null
      ValidateKey(key);

      // the region mame must be null
      ValidateRegionName(regionName);

      // get the lock now so we can hold it until we return the value.
      MemoryCache::Lock guard(_mutex);

      // look for the complete cache item value
      const auto it = _cacheItems.find(std::wstring(key));
      if (it == _cacheItems.end())
      {
        return nullptr;
      }

      // get the value
      auto value = it->second._item.Value();

      // did the value expire?
      // if it has then we have to return null.
      if (it->second._policy.HasExpired())
      {
        value = nullptr;
      }

      // remove it.
      _cacheItems.erase(it);

      // return the value.
      return value;
    }
  }
}