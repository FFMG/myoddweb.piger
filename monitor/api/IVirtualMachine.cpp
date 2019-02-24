#include "stdafx.h"
#include "IVirtualMachine.h"

IVirtualMachine::IVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler) :
  _actions(actions),
  _messagesHandler( messagesHandler )
{
}

IVirtualMachine::~IVirtualMachine()
{
}
