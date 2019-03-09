#pragma once

#include <mutex>
#include <vector>
#include "../threads/lock.h"
#include "ipcmessagehandler.h"

namespace myodd {
  namespace os {
    class IpcListener : IpcMessageHandler
    {
    public:
      /**
       * Create the server.
       */
      IpcListener(const wchar_t* serverName);
      virtual ~IpcListener();

      //  add a message handler
      void AddMessageHandler(IpcMessageHandler& handler);

      // remove a message handler.
      void RemoveMessageHandler(IpcMessageHandler& handler);

      // the message handler
      bool HandleIpcMessage(const IpcData& ipcRequest, IpcData& ipcResponse) override;

      //  handle the send message.
      bool HandleIpcSendMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam) override;

      //  handle the post message.
      bool HandleIpcPostMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam) override;

    protected:
      // create the window that will receive all the messages.
      void Create(const wchar_t* serverName);

      // the server window.
      void* _pServer;

      // all the messages handler.
      std::vector<IpcMessageHandler*> _messageHandlers;

      // find a specific message handler.
      std::vector<IpcMessageHandler*>::const_iterator FindMessageHandler(IpcMessageHandler& handler) const;

      // the mutex lock
      myodd::threads::Key _mutex;
    };
  }
}
