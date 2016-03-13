#include "stdafx.h"
#include "workers.h"

namespace myodd {
  namespace threads {
    Workers::Workers()
    {
    }

    Workers::~Workers()
    {
      //  make sure we wait for all our workers.
      WaitForAllWorkers();
    }

    /**
     * Add this thread to the queue.
     */
    void Workers::QueueWorker(std::thread* threadToQueue)
    {
      _workers.push_back(threadToQueue);
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
      for (vWorkers::iterator it = _workers.begin(); it != _workers.end(); ++it)
      {
        //  is it still busy?
        if ((*it)->joinable())
        {
          (*it)->join();
        }

        // now that it is done, we can free the memory
        delete (*it);
      }

      //  we can now get rid of all the workers.
      _workers.clear();
    }
  }
}