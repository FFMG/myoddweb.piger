#include "cacheitem.h"
#include <typeinfo> //  typeid class
#include <wchar.h>

namespace myodd {
  namespace cache {

    /**
    * Initializes a new CacheItem instance using the specified key of a cache entry.
    * @param const wchar_t* key A unique identifier for a CacheItem entry.
    */
    CacheItem::CacheItem(const wchar_t* key ) :
      CacheItem(key, nullptr, nullptr )
    {
    }

    /**
     * Initializes a new CacheItem instance using the specified key of a cache entry.
     * @param const wchar_t* key A unique identifier for a CacheItem entry.
     * @param T value the value we want to set this item as. 
     * @param const wchar_t* regionName the region name.
     */
    template<class T>
    CacheItem::CacheItem(const wchar_t* key, T value/* = nullptr*/, const wchar_t* regionName/* = nullptr*/) : 
      _value_type_index( typeid(nullptr) ),
      _key( nullptr ),
      _regionName( nullptr ),
      _value( nullptr )
    {
      // set the value
      Value(value);

      // set the key
      Key(key);

      // set the regionnae
      RegionName(regionName);
    }

    CacheItem::~CacheItem()
    {
      // clean everything
      Clean();
    }

    /**
     * Clean all the values
     */
    void CacheItem::Clean()
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
    void CacheItem::CleanKey()
    {
      delete[] _key;
      _key = nullptr;
    }

    /**
     * Clean the region name.
     */
    void CacheItem::CleanRegionName()
    {
      delete[] _regionName;
      _regionName = nullptr;
    }

    /**
     * Clean the actual value.
     */
    void CacheItem::CleanValue()
    {
      // reset the value
      delete[] _value;
      _value = nullptr;

      // and the type index.
      _value_type_index = typeid(nullptr);
    }

    /**
     * Set the key value
     * const wchar_t* key the key value.
     */
    void CacheItem::Key(const wchar_t* key)
    {
      // clean the key
      CleanKey();

      if (nullptr != key)
      {
        auto l = wcslen(key);
        _key = new wchar_t[l + 1];
        wmemset(_key, 0, l+1 );
        wcsncpy(_key, key, l);
      }
    }

    /**
     * Get the Key value if we have one.
     * @return const wchar_t* the key value
     */
    const wchar_t* CacheItem::Key() const
    {
      return _key;
    }

    /**
    * Set the region name value
    * const wchar_t* regionName the key value.
    */
    void CacheItem::RegionName(const wchar_t* regionName)
    {
      // clean the regionName
      CleanRegionName();

      if (nullptr != regionName )
      {
        auto l = wcslen(regionName);
        _regionName = new wchar_t[l + 1];
        wmemset(_regionName, 0, l + 1);
        wcsncpy(_regionName, regionName, l);
      }
    }

    /**
     * Get the current retion name
     * @return const wchar_t* the region name
     */
    const wchar_t* CacheItem::RegionName() const
    {
      return _regionName;
    }

    template<class T>
    void CacheItem::Value( const T& value )
    {
      // clean the value
      CleanValue();

      if (value != nullptr)
      {
        // set the value
        auto l = sizeof value;
        _value = new char[l];
        memset(_value, 0, l);
        memcpy(_value, value, l);
      }

      // set the type info.
      _value_type_index = std::type_index(typeid(value));
    }
  }
}