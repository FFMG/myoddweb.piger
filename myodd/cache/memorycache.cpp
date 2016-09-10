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
    MemoryCache::MemoryCache(const wchar_t* name) : _name( name ? name : L"" )
    {
      ValidateName();
    }

    MemoryCache::~MemoryCache()
    {
    }

    /**
     * Make sure that the given name is valid.
     * It cannot be empty + null. Spaces are alowed, (and should be retained).
     */
    void MemoryCache::ValidateName() const
    {
      if (_name.length() == 0)
      {
        throw std::exception( "The length of name cannot be 0" );
      }

      // this might not work for all the locals but we are only looking for 'default'.
      auto lowerName = _name;
      std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), std::towlower );
      if (lowerName == L"default" )
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
      if (nullptr == key)
      {
        throw std::invalid_argument("The key name cannot be null!");
      }

      //  the region nust be null
      if (nullptr != regionName)
      {
        throw std::invalid_argument("The regionName must be null!");
      }

      // lock us in so we don't find data while deleting it, (or something like that).
      MemoryCache::Lock guard( _mutex );

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
    size_t MemoryCache::GetCount(const wchar_t* regionName ) const
    {
      //  the region nust be null
      if (nullptr != regionName)
      {
        throw std::invalid_argument("The regionName must be null!");
      }

      // we must lock so it does not change mid-flight.
      MemoryCache::Lock guard(_mutex);

      //  return the size
      return _cacheItems.size();
    }
  }
}