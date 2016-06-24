#pragma once

namespace myodd {
  namespace os {
    class IpcListener
    {
    public:
      /**
       * Create the server.
       */
      IpcListener( const wchar_t* serverName );
      virtual ~IpcListener();

    protected:
      void Create(const wchar_t* serverName);

      void* _pServer;
    };
  }
}