#pragma once

#include <string>
#include "cacheitem.h"
#include "cacheitempolicy.h"

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
      bool Add( const CacheItem& item, const CacheItemPolicy& policy);

    private:
      const std::wstring _name;

      void ValidateName() const;
    };
  }
}