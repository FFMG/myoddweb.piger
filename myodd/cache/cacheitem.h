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

#include <wchar.h>
#include "../dynamic/any.h"

namespace myodd {
  namespace cache {
    /**
     * Implementation of CacheItem
     * https://msdn.microsoft.com/en-us/library/system.runtime.caching.cacheitem(v=vs.110).aspx
     */
    class CacheItem
    {
    public:
      /** 
       * Copy contructor. 
       * @param const CacheItem& rhs the item we are copying.
       */
      CacheItem(const CacheItem& rhs ) :
        _key(nullptr),
        _regionName(nullptr)
      {
        Copy(rhs);
      }

      /**
       * Initializes a new CacheItem instance using the specified key of a cache entry.
       * @param const wchar_t* key A unique identifier for a CacheItem entry.
       */
      CacheItem(const wchar_t* key) :
        CacheItem(key, ::myodd::dynamic::Any(), nullptr)
      {
      }

      /**
       * Initializes a new CacheItem instance using the specified key of a cache entry.
       * @param const wchar_t* key A unique identifier for a CacheItem entry.
       * @param T value the value we want to set this item as.
       * @param const wchar_t* regionName the region name.
       */
      CacheItem(const wchar_t* key, const ::myodd::dynamic::Any& value, const wchar_t* regionName = nullptr) :
        _key(nullptr),
        _regionName(nullptr),
        _value(value)
      {
        // set the value
        Value(value);

        // set the key
        Key(key);

        // set the regionnae
        RegionName(regionName);
      }

      /**
       * =operator.
       * @param const CacheItem& rhs the item we are copying.
       */
      const CacheItem& operator=(const CacheItem& rhs)
      {
        Copy(rhs);
        return *this;
      }

      virtual ~CacheItem()
      {
        // clean everything
        Clean();
      }

      /**
       * Set the key value
       * const wchar_t* key the key value.
       */
      void Key(const wchar_t* key)
      {
        // clean the key
        CleanKey();

        if (nullptr != key)
        {
          auto l = wcslen(key);
          _key = new wchar_t[l + 1];
          wmemset(_key, 0, l + 1);
          _valid_wcsncpy(_key, key, l);
        }
      }

      /**
       * Get the Key value if we have one.
       * @return const wchar_t* the key value
       */
      const wchar_t* Key() const
      {
        return _key;
      }

      /**
       * Get the region name
       * @return const wchar_t* the name.
       */
      const wchar_t* RegionName() const
      {
        return _regionName;
      }

      /**
       * Set the region name value
       * const wchar_t* regionName the key value.
       */
      void RegionName(const wchar_t* regionName)
      {
        // clean the regionName
        CleanRegionName();

        if (nullptr != regionName)
        {
          auto l = wcslen(regionName);
          _regionName = new wchar_t[l + 1];
          wmemset(_regionName, 0, l + 1);
          _valid_wcsncpy(_regionName, regionName, l);
        }
      }

      /**
       * Set the value 
       * @param const ::myodd::dynamic::Any& value, the value we want to set.
       */
      void Value(const ::myodd::dynamic::Any& value)
      {
        // clean the value
        CleanValue();

        // set it.
        _value = value;
      }
 
      /**
       * Get the current value
       */
      const ::myodd::dynamic::Any& Value() const
      {
        return _value;
      }
    private:
      /**
       * @param const CacheItem& rhs the item we are copying.
       */
      void Copy(const CacheItem& rhs)
      {
        if (this != &rhs)
        {
          // clean everything
          Clean();

          // set the value
          Value( rhs._value );

          // set the key
          Key( rhs._key );

          // set the regionnae
          RegionName( rhs._regionName );
        }
      }

      /**
       * Clean all the values
       */
      void Clean()
      {
        // the key
        CleanKey();

        // the region
        CleanRegionName();

        // the value.
        CleanValue();
      }

      /**
       * Clear the key value.
       */
      void CleanKey()
      {
        delete[] _key;
        _key = nullptr;
      }
 
      /**
       * Clean the region name.
       */
      void CleanRegionName()
      {
        delete[] _regionName;
        _regionName = nullptr;
      }

      /**
       * Clean the actual value.
       */
      void CleanValue()
      {
        // not much we can do here...
      }


      wchar_t* _key;
      wchar_t* _regionName;

      ::myodd::dynamic::Any _value;

      /**
      * same as wcsncpy but we have to go around the MS warnings...
      */
      static void _valid_wcsncpy(
        wchar_t *strDest,
        const wchar_t *strSource,
        size_t count
      )
      {
#if (defined(_WIN32) || defined(WIN32))
#pragma warning( push )
#pragma warning(disable:4996)
#endif
        wcsncpy(strDest, strSource, count);
#if (defined(_WIN32) || defined(WIN32))
#pragma warning( pop )
#endif
      }
    };
  }
}
