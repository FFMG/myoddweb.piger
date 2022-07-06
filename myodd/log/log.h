// log.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../common/includes.h"
#include "../threads/threads.h"

#include "logtype.h"

namespace myodd{ namespace log{
  // the helper functions
  void Log(const MYODD_CHAR* pszLine, ...);
  void LogSuccess(const MYODD_CHAR* pszLine, ...);
  void LogError(const MYODD_CHAR* pszLine, ...);
  void LogWarning(const MYODD_CHAR* pszLine, ...);
  void LogMessage(const MYODD_CHAR* pszLine, ...);
  void LogSystem(const MYODD_CHAR* pszLine, ...);

  //  add the debug log message
  void LogDebug(LogType uiType, const MYODD_CHAR* pszFmt, ...);  //  log in _DEBUG messages only

  
  bool AddNotif( const LogEventCallback& fnNotif, MYODD_LPARAM lParam, size_t iSendLast = 100 );
  bool RemoveNotif( const LogEventCallback& fnNotif, MYODD_LPARAM lParam );

  const std::wstring& GetCurrentLogFile();

  //  initialise the various events.
  bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention, size_t maxFileSize);
} //  log
} //  myodd
