#include "memorycache.h"
#include <algorithm>  //  std::transform
#include <cwctype>    //  std::towlower

namespace myodd {
  namespace cache {

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
  }
}