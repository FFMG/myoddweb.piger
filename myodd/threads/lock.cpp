#include "stdafx.h"
#include "lock.h"

namespace myodd {
  namespace threads {
    Lock::Lock(std::mutex &mutex) :
      _mustUnlock(false),
      _mutex(mutex),
      _guard(NULL)
    {
      try
      {
        _guard = new std::lock_guard<std::mutex>(_mutex);
      }
      catch (const std::system_error& e) {

      }
    }

    Lock::~Lock()
    {
      delete _guard;
    }
  }
}