#include "lock.h"
#include <ostream>
#include <iostream>

#ifdef _DEBUG
#include <Afx.h>
#endif

namespace myodd {
  namespace threads {
    #ifdef _DEBUG
    #define __OUT TRACE
    #else
    #define __OUT
    #endif

    Lock::Lock(Key &key ) :
      _key(key),
      _guard(nullptr)
    {
      try
      {
        __OUT( L"---------------- Getting lock %s\n", _key._name.c_str() );
        _guard = new std::lock_guard<std::mutex>(_key);
      }
      catch (const std::system_error&) 
      {
        // and error is thrown if we are trying re-entry.
      }
      __OUT(L"---------------- Got lock %s\n", _key._name.c_str());
    }

    /**
     * Free memory, release the lock.
     */
    Lock::~Lock()
    {
      Release();
    }

    /**
     * Release the lock straightaway.
     */
    void Lock::Release()
    {
      delete _guard;
      _guard = nullptr;
      __OUT(L"---------------- Released lock %s\n", _key._name.c_str());
    }
  }
}
