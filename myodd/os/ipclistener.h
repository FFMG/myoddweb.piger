#pragma once

#include <mutex>

namespace myodd {
  namespace os {
    class IpcListener
    {
    public:
      /**
       * Create the server.
       */
      IpcListener( const wchar_t* serverName, void* pParent );
      virtual ~IpcListener();

    protected:
      void Create(const wchar_t* serverName, void* pParent);

      // the server window.
      void* _pServer;

      // the mutex lock
      std::mutex _mutex;
    };
  }
}