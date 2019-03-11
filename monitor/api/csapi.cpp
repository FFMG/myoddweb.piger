#include "StdAfx.h"
#include "csapi.h"

/**
 * \copydoc
 */
CsApi::CsApi(const std::wstring& uuid, const ActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler) :
  PowershellApi(uuid, action, actions, messagesHandler )
{
}
