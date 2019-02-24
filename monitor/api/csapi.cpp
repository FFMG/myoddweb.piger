#include "StdAfx.h"
#include "csapi.h"
#include "pluginVirtualMachine.h"

/**
 * \copydoc
 */
CsApi::CsApi(const ActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler) :
  PowershellApi(action, actions, messagesHandler )
{
}
