#pragma once

// remove MS warnings.
#ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
# define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

#include <typeindex>  //  typeindex

namespace myodd {
  namespace cache {
    /**
     * Implementation of CacheItem
     * https://msdn.microsoft.com/en-us/library/system.runtime.caching.cacheitem(v=vs.110).aspx
     */
    class CacheItem
    {
    public:
      CacheItem(const wchar_t* key);

      template<class T>
      CacheItem(const wchar_t* key, const T& value );

      template<class T>
      CacheItem(const wchar_t* key, const T& value, const wchar_t* regionName );

      virtual ~CacheItem();

      const wchar_t* Key() const;
      const wchar_t* RegionName() const;

      template<class T>
      const T& Value() const;

      void Key(const wchar_t* key);
      void RegionName(const wchar_t* regionName);

      template<class T>
      void Value(const T& value );

    private:
      void Clean();
      void CleanKey();
      void CleanRegionName();
      void CleanValue();

      wchar_t* _key;
      wchar_t* _regionName;

      char* _value;
      std::type_index _value_type_index;
    };
  }
}
