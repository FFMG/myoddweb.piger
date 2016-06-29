#pragma once

#include <mutex>
#include <vector>
#include "ipcmessagehandler.h"

namespace myodd {
  namespace os {
    class IpcListener : IpcMessageHandler
    {
    public:
      /**
       * Create the server.
       */
      IpcListener( const wchar_t* serverName, void* pParent );
      virtual ~IpcListener();

      //  add a message handler
      void AddMessageHandler( IpcMessageHandler& handler);

      // remove a message handler.
      void RemoveMessageHandler(IpcMessageHandler& handler);

      // the message handler
      bool HandleIpcMessage(const IpcData& ipcRequest, IpcData& ipcResponse) override;
      
    protected:
      // create the window that will receive all the messages.
      void Create(const wchar_t* serverName, void* pParent);

      // the server window.
      void* _pServer;

      // all the messages handler.
      std::vector<IpcMessageHandler*> _messageHandlers;

      // find a specific message handler.
      std::vector<IpcMessageHandler*>::const_iterator FindMessageHandler(IpcMessageHandler& handler) const;

      // the mutex lock
      std::mutex _mutex;
    };
  }
}
