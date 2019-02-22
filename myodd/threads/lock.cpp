#include "lock.h"
#include <ostream>
#include <iostream>

namespace myodd {
  namespace threads {
    Lock::Lock(std::mutex &mutex) :
      _mutex(mutex),
      _guard(nullptr)
    {
      try
      {
        _guard = new std::lock_guard<std::mutex>(_mutex);
      }
      catch (const std::system_error&) 
      {
        // and error is thrown if we are trying re-entry.
      }
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
    }
  }
}
