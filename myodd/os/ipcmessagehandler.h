#pragma once
#include "ipcdata.h"

namespace myodd {
  namespace os {
    class IpcMessageHandler
    {
    public:
      virtual ~IpcMessageHandler()
      {
      }

      //  handle the message.
      virtual bool HandleIpcMessage(const IpcData& ipcRequest, IpcData& ipcResponse ) = 0;

      //  handle the send message.
      virtual bool HandleIpcSendMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam) = 0;

      //  handle the post message.
      virtual bool HandleIpcPostMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam ) = 0;
    };
  }
}
