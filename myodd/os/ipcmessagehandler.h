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
    };
  }
}
