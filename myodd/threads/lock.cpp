#include "stdafx.h"
#include "lock.h"

namespace myodd {
  namespace threads {
    Lock::Lock(std::mutex &mutex) :
      _mutex(mutex),
      _guard(NULL)
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
      _guard = NULL;
    }
  }
}