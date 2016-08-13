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
      template<class T>
      bool Add(const CacheItem<T>& item, const CacheItemPolicy& policy)
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
        auto copyOfItem = new CacheItem<T>(item);

        // we can now add it to our list.
        _cacheItems.emplace(
          std::make_pair( std::wstring(item.Key()),
            CachItemsPtr( copyOfItem , [](void *ptr) 
            {
              delete static_cast<CacheItem<T>*>(ptr); 
            }))
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
      template<class T>
      bool Add(const wchar_t* key, T value, CacheItemPolicy policy, const wchar_t* regionName = nullptr )
      {
        //  just pass the value to the CacheItem function.
        return Add( CacheItem<T>(key, value, regionName), policy);
      }

      /**
       * Determines whether a cache entry exists in the cache.
       * @see https://msdn.microsoft.com/en-us/library/system.runtime.caching.memorycache.contains(v=vs.110).aspx
       */
      bool Contains(const wchar_t* key, const wchar_t* regionName = nullptr) const;
    private:
      // the mutex to protect this data
      // it is mutable so we can use it inside const functions.
      mutable std::mutex _mutex;

      // all our items.
      typedef std::unique_ptr<void, void(*)(void*)> CachItemsPtr;
      typedef std::map< std::wstring, CachItemsPtr > CachItems;
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