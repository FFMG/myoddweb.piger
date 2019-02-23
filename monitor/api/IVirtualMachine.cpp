#include "stdafx.h"
#include "IVirtualMachine.h"

IVirtualMachine::IVirtualMachine(IMessagesHandler& messagesHandler) :
  _messagesHandler( messagesHandler )
{
}

IVirtualMachine::~IVirtualMachine()
{
}
