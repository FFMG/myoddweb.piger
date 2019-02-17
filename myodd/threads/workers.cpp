#include "workers.h"

namespace myodd {
  namespace threads {
    Workers::Workers()
    {
    }

    Workers::~Workers()
    {
      //  anything to do?
      if (_workers.size() != 0)
      {
        //  we _might_ have some workers waiting to finish.
        myodd::threads::Lock guard(_mutex);
        if (_workers.size() != 0)
        {
          //  make sure we wait for all our workers.
          WaitForAllWorkers();
        }
      }
    }

    /**
     * \brief Add this thread to the queue.
     * \param threadToQueue the thread we want to queue.
     * \return std::thread& the thread that we just added.
     */
    std::thread& Workers::QueueWorker(std::thread* threadToQueue)
    {
      Lock guard(_mutex);
      _workers.push_back(threadToQueue);
      return *threadToQueue;
    }

    /**
     * Make sure all the workers are finished doing their work.
     * We call the base class once this is done.
     */
    void Workers::WaitForAllWorkers()
    {
      //  anything to do?
      if (_workers.size() == 0)
      {
        return;
      }

      //  we _might_ have some workers waiting to finish.
      Lock guard(_mutex);
      for (auto& th : _workers)
      {
        th->join();

        // give threads a chance to run...
        std::this_thread::yield();
      }

      // then delete the threads.
      for (auto& th : _workers)
      {
        // now that it is done, we can free the memory
        delete th;

        // give threads a chance to run...
        std::this_thread::yield();
      }

      //  we can now get rid of all the workers.
      _workers.clear();
    }
  }
}