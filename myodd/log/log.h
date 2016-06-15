// log.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../common/includes.h"
#include "../threads/threads.h"

#include "logtype.h"

namespace myodd{ namespace log{
  // the helper functions
  void Log(LPCTSTR pszLine, ...);
  void LogSuccess(LPCTSTR pszLine, ...);
  void LogError(LPCTSTR pszLine, ...);
  void LogWarning(LPCTSTR pszLine, ...);
  void LogMessage(LPCTSTR pszLine, ...);
  void LogSystem(LPCTSTR pszLine, ...);

  //  add the debug log message
  void LogDebug(LogType uiType, LPCTSTR pszFmt, ...);  //  log in _DEBUG messages only

  
  bool AddNotif( const LogEventCallback& fnNotif, LPARAM lParam, size_t iSendLast = 100 );
  bool RemoveNotif( const LogEventCallback& fnNotif, LPARAM lParam );

  const STD_TSTRING& GetCurrentLogFile();

  //  initialise the various events.
  bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention);
} //  log
} //  myodd
