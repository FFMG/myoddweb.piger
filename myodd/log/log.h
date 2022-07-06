// log.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../common/includes.h"
#include "../threads/threads.h"

#include "logtype.h"

namespace myodd{ namespace log{
  // the helper functions
  void Log(const wchar_t* pszLine, ...);
  void LogSuccess(const wchar_t* pszLine, ...);
  void LogError(const wchar_t* pszLine, ...);
  void LogWarning(const wchar_t* pszLine, ...);
  void LogMessage(const wchar_t* pszLine, ...);
  void LogSystem(const wchar_t* pszLine, ...);

  //  add the debug log message
  void LogDebug(LogType uiType, const wchar_t* pszFmt, ...);  //  log in _DEBUG messages only

  
  bool AddNotif( const LogEventCallback& fnNotif, MYODD_LPARAM lParam, size_t iSendLast = 100 );
  bool RemoveNotif( const LogEventCallback& fnNotif, MYODD_LPARAM lParam );

  const std::wstring& GetCurrentLogFile();

  //  initialise the various events.
  bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention, size_t maxFileSize);
} //  log
} //  myodd
