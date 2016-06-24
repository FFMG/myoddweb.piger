#pragma once

namespace myodd {
  namespace os {
    class Ipc
    {
    public:
      Ipc( const wchar_t* serverName );
      virtual ~Ipc();

    protected:
      void CreateServer(const wchar_t* serverName);

      void* _pServer;
    };
  }
}