#pragma once

// add the APLI libs
#ifdef ACTIONMONITOR_API_PLUGIN
#include "helperapi.h"

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PLUGIN_VERSION = 0.1f;

class PluginApi final : public HelperApi
{
public:
  PluginApi(const IActiveAction& action, IApplication& application, IMessagesHandler& messagesHandler);
  virtual ~PluginApi();

public:
  double Version ();
  size_t GetCommandCount();

  bool Say         (const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const override;
  bool Execute     (const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const override;
  int GetString    (unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const;
  size_t GetCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer ) const;
  int GetAction    (unsigned int nBufferLength, wchar_t* lpBuffer ) const;
  int GetFile      (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const;
  int GetFolder    (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const;
  int GetUrl       (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer, bool bQuote) const;
  bool AddAction   (const wchar_t* szText, const wchar_t* szPath ) const;
  bool RemoveAction(const wchar_t* szText, const wchar_t* szPath ) const;
  bool GetVersion  (unsigned int nBufferLength, wchar_t* lpBuffer) const;
  bool FindAction  (unsigned int idx, const wchar_t* lpCommand, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  void Log( unsigned int logType, const wchar_t* lpText ) const;
  
  HWND GetForegroundWindow() const override;
};

#endif
