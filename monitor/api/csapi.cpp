#include "StdAfx.h"
#include "csapi.h"

/**
 * \copydoc
 */
CsApi::CsApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messagesHandler) :
  PowershellApi(uuid, action, application, messagesHandler )
{
}
