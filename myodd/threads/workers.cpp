#include "workers.h"
#include <future>
#include <thread>

namespace myodd {
  namespace threads {
    Workers::Workers() : _mutex(L"Workers")
    {
    }

    Workers::~Workers()
    {
      //  anything to do?
      if (_workers.size() != 0)
      {
        //  we _might_ have some workers waiting to finish.
        Lock guard(_mutex);
        if (_workers.size() != 0)
        {
          //  make sure we wait for all our workers.
          WaitForAllWorkers();
        }
      }
    }

    /**
     * \brief check if any threads are running
     * \return if anything is still running, (joinable)
     */
    bool Workers::Running() const
    {
      // if anybody is busy ... then we are not ready.
      for (auto& future : _workers)
      {
        const auto status = future.wait_for(std::chrono::microseconds(10));
        if (status != std::future_status::ready)
        {
          return true;
        }
      }
      // if we are here, nothing is running.
      return false;
    }

    void Workers::WaitForAllWorkers()
    {
      WaitForAllWorkers(nullptr);
    }

    /**
     * \brief Make sure all the workers are finished doing their work.
     *        We call the base class once this is done.
     * \param onInterval called at regular intervals while we are waiting.
     *        if the function returns false, we will break out of the loop
     */
    void Workers::WaitForAllWorkers(std::function<bool()> onInterval)
    {
      //  anything to do?
      if (_workers.size() == 0)
      {
        return;
      }

      //  we _might_ have some workers waiting to finish.
      const auto sleepForMilliseconds = std::chrono::milliseconds(10);
      auto lastUsedIndex = 0;
      for(;;)
      {
        // try and remove that one worker.
        // return -1 if no more work is possible.
        lastUsedIndex = WaitForOneWorkerAndCompleteIfNeeded(lastUsedIndex);

        // do we have anything left?
        if(lastUsedIndex == -1 )
        {
          break;
        }

        // move forward.
        ++lastUsedIndex;

        if( onInterval != nullptr )
        {
          try
          {
            if( !onInterval() )
            {
              break;
            }
          }
          catch(...)
          {
          }
        }

        // give threads a chance to run...
        std::this_thread::yield();
        std::this_thread::sleep_for(sleepForMilliseconds);
      }
    }

    /**
     * \brief Wait for a little while for a worker to finish
     *        If it is not finished then we will move on to the next item
     *        If it is finished we will remove it from the list
     * \param index the index we would like to wait a little bit for.
     * \return the actual index we ran
     *         return -1 if there are no more indexes to run, (the collection is empty).
     */
    int Workers::WaitForOneWorkerAndCompleteIfNeeded(int index)
    {
      const auto waitForMilliseconds = std::chrono::milliseconds(10);

      Lock guard(_mutex);

      // now that we have the lock, make sure that the value is within range
      const auto numberOfWorkers = _workers.size();
      if (numberOfWorkers == 0)
      {
        return -1;
      }

      index = index >= numberOfWorkers ? 0 : index;

      // get a future and move the indexes forward.
      auto& future = _workers[index];
      if (future.valid())
      {
        const auto status = future.wait_for(waitForMilliseconds);
        if (status != std::future_status::ready)
        {
          return index;
        }
      }

      // remove that worker as it is complete.
      _workers.erase(_workers.begin() + index);
      return index;
    }
  }
}
