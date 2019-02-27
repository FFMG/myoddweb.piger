#pragma once

#include <vector>
#include "lock.h"
#include <future>

namespace myodd {
  namespace threads {
    class Workers
    {
    public:
      Workers();
      virtual ~Workers();

      // no copies.
      Workers(const Workers&) = delete;
      const Workers& operator=(const Workers&) = delete;

    public:
      void WaitForAllWorkers();

      /**
       * \brief Add this thread to the queue.
       * \param function the thread we want to queue.
       * \param args the arguments to pass to the function
       * \return std::thread& the thread that we just added.
       */
      template< class Function, class... Args >
      void QueueWorker(Function&& function, Args&&... args)
      {
        // the thread we are creating.
        auto future = std::async(std::launch::async, function, std::forward<Args>(args)...);

        // add it to the list
        {
          Lock guard(_mutex);
          _workers.push_back(std::move(future));
        }
      }

      bool Running() const;

    private:
      // wait for one of the worker to complete and remove it if needed.
      int WaitForOneWorkerAndCompleteIfNeeded( int index );
      
      // all the messages we are waiting to complete.
      typedef std::vector<std::future<void>> VWorkers;
      VWorkers _workers;
      Key _mutex;
    };
  }
}
