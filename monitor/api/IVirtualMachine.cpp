#include "stdafx.h"
#include "IVirtualMachine.h"

IVirtualMachine::IVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  _application(application),
  _messagesHandler( messagesHandler ),
  _ipcListener( iIpcListener )
{
}

IVirtualMachine::~IVirtualMachine()
{
}
