#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_CS_PLUGIN
#include "powershellapi.h"
#include <string>

// this is the version number for that particular API
static const std::wstring ACTIONMONITOR_CS_PLUGIN_VERSION = L"0.1";

class CsApi final : public PowershellApi
{
public:
  CsApi(const std::wstring& uuid, const IActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler );
  virtual ~CsApi() = default;
};

#endif
