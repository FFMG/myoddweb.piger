#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_CS_PLUGIN
#include "powershellapi.h"
#include <string>

// this is the version number for that particular API
static const std::wstring ACTIONMONITOR_CS_PLUGIN_VERSION = L"0.1";

class CsApi : public PowershellApi
{
public:
  CsApi(const ActiveAction& action, IMessages& messages);
  virtual ~CsApi() = default;
};

#endif
