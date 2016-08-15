#pragma once

#include <string>
#include "cacheitem.h"
#include "cacheitempolicy.h"

#include <mutex>  //  locks
#include <map>    //  all the items.
#include <memory> // unique_ptr

namespace myodd {
  namespace cache {
    /**
     * C++ implementation of .NET MemoryCache
     * The objective(s) of this class are
     *   - Be thread safe
     *   - Be fast
     *   - 'Look' similar to the .NET implementatation as much as posible.
     *   - be c++11 compliant and require no other libraries.
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
       * @param const CacheItem<T>& item the item to add.
       * @param CacheItemPolicy policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
       * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
       */
      bool Add(const CacheItem& item, const CacheItemPolicy& policy)
      {
        // lock the threads so we don't have a race condition.
        MemoryCache::Lock guard(_mutex);

        // if we already have it, then we cannot add it.
        if (Contains(item.Key()))
        {
          // we cannot add it again, release the lock on the way out.
          return false;
        }

        // copy this item
        auto copyOfItem = new CacheItem(item);

        // we can now add it to our list.
        _cacheItems.emplace(
          std::make_pair( std::wstring(item.Key()),
            CacheItemPtr( copyOfItem , 
              //  lambda delete function
              [](void *ptr)
              {
                delete static_cast<CacheItem*>(ptr); 
              },
              // lamdbad get function
              []()
              { 
                return (CacheItem*)nullptr;
              }
            ))
        );

        // success.
        return true;
      }

      /**
       * inserts a cache entry into the cache, specifying information about how the entry will be evicted.
       * @see https://msdn.microsoft.com/en-us/library/ee395902(v=vs.110).aspx
       * @param const wchar_t* key A unique identifier for the cache entry.
       * @param T value the value we want to add
       * @param CacheItemPolicy policy An object that contains eviction details for the cache entry. This object provides more options for eviction than a simple absolute expiration.
       * @param const wchar_t* regionName = nullptr Optional. A named region in the cache to which the cache entry can be added, if regions are implemented. The default value for the optional parameter is null.
       * @return bool  if the insertion try succeeds, or false if there is an already an entry in the cache with the same key as key.
       */
      bool Add(const wchar_t* key, const boost::any& value, CacheItemPolicy policy, const wchar_t* regionName = nullptr )
      {
        //  just pass the value to the CacheItem function.
        return Add( CacheItem(key, value, regionName), policy);
      }

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
       * Returns the specified entry from the cache as a CacheItem instance.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.getcacheitem(v=vs.110).aspx
       * @throw std::out_of_range if the item is not found.
       * @throw std::invalid_argument if the key is null or if regionname is not null.
       * @param A unique identifier for the cache entry to get.
       * @param A named region in the cache to which a cache entry was added. Do not pass a value for this parameter. This parameter is null by default, because the MemoryCache class does not implement regions.
       * @return const CacheItem<T>& the cache item, we will throw if the item does not exist.
       */
      const CacheItem& GetCacheItem(const wchar_t* key, const wchar_t* regionName = nullptr) const
      {
        //  the region nust be null
        if (nullptr != regionName)
        {
          throw std::invalid_argument("The regionName must be null!");
        }

        // the key cannot be null
        if (nullptr == key)
        {
          throw std::invalid_argument("The key name cannot be null!");
        }

        // we now need to lock as this might/could change.
        MemoryCache::Lock guard(_mutex);

        // now look for the value.
        const auto it = _cacheItems.find(std::wstring(key));
        if (it == _cacheItems.end())
        {
          throw std::out_of_range("Could not locate this key");
        }

        auto xx = it->second.getValue<int>();
      }

    private:
      // the mutex to protect this data
      // it is mutable so we can use it inside const functions.
      mutable std::mutex _mutex;

      // all our items.
      typedef std::map< std::wstring, CacheItemPtr > CachItems;
      CachItems _cacheItems;

      // the name of this collection
      const std::wstring _name;

      // nake sure that the name is valid
      // we throw if there is an error.
      void ValidateName() const;

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
    };
  }
}