#include "stdafx.h"
#include "IVirtualMachine.h"

IVirtualMachine::IVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  _actions(actions),
  _messagesHandler( messagesHandler ),
  _ipcListener( iIpcListener )
{
}

IVirtualMachine::~IVirtualMachine()
{
}
