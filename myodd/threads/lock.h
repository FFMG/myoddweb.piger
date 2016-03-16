#pragma once

#include <mutex>

namespace myodd {
  namespace threads {
    class Lock
    {
    public:
      Lock( std::mutex& mutex );
      virtual ~Lock();

    private:
      bool _mustUnlock;
      std::mutex& _mutex;
      std::lock_guard<std::mutex>* _guard;
    };
  }
}