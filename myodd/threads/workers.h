#pragma once

#include <thread>
#include <vector>
#include "lock.h"

namespace myodd {
  namespace threads {
    class Workers
    {
    public:
      Workers();
      virtual ~Workers();

    protected:
      void WaitForAllWorkers();
      std::thread& QueueWorker(std::thread* threadToQueue);

      template< class Function, class... Args >
      std::thread& QueueWorker(Function&& f, Args&&... args)
      {
        return QueueWorker(new std::thread(f, std::forward<Args>(args) ... ));
      }

    private:

      // all the messages we are waiting to complete.
      typedef std::vector<std::thread*> vWorkers;
      vWorkers _workers;
      std::mutex _mutex;
    };
  }
}
