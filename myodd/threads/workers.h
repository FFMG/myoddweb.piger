#pragma once

#include <thread>
#include <vector>

namespace myodd {
  namespace threads {
    class Workers
    {
    public:
      Workers();
      virtual ~Workers();

    protected:
      void WaitForAllWorkers();
      void QueueWorker(std::thread* threadToQueue);

      template< class Function, class... Args >
      void QueueWorker(Function&& f, Args&&... args)
      {
        QueueWorker(new std::thread(f, std::forward<Args>(args) ... ));
      }

    private:

      // all the messages we are waiting to complete.
      typedef std::vector<std::thread*> vWorkers;
      vWorkers _workers;
    };
  }
}
