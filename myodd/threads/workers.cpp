#include "workers.h"
#include <future>
#include <thread>

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
        const auto status = future.wait_for(std::chrono::seconds(0));
        if( status != std::future_status::ready)
        {
          return true;
        }
      }
      // if we are here, nothing is running.
      return false;
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
      for (auto& future : _workers)
      {
        future.wait();

        // give threads a chance to run...
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      //  we can now get rid of all the workers.
      _workers.clear();
    }
  }
}