#include "stdafx.h"
#include "log.h"

namespace myodd{ namespace log{
  void LogDebug( LogType uiType, LPCTSTR pszFmt, ...)
  {
#ifdef _DEBUG
    va_list argp;
    va_start(argp, pszFmt);
    LogEvent::Instance().Log( uiType, pszFmt, argp);
    va_end(argp);
#else
    UNUSED_ALWAYS( uiType );
    UNUSED_ALWAYS( pszFmt );
    __noop;
#endif
  }

  void Log(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log(LogType::None, pszFmt, argp);
	  va_end(argp);
  }

  void LogSuccess(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log(LogType::Success, pszFmt, argp);
	  va_end(argp);
  }

  void LogError(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log(LogType::Error, pszFmt, argp);
	  va_end(argp);
  }

  void LogWarning(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log(LogType::Warning, pszFmt, argp);
	  va_end(argp);
  }

  void LogMessage(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log( LogType::Message, pszFmt, argp);
	  va_end(argp);
  }

  /**
   * Log a system message.
   * @param LPCTSTR the string message we are logging.
   * @param ... params in the message.
   * @return none
   */
  void LogSystem(LPCTSTR pszFmt, ...)
  {
    va_list argp;
    va_start(argp, pszFmt);
    LogEvent::Instance().Log( LogType::System, pszFmt, argp);
    va_end(argp);
  }

  /**
   * Set the log path directory.
   * If we pass a NULL argument then we want to stop/disable the logging.
   * @param const std::wstring& wPath the log path we will be using.
   * @param const std::wstring& wPrefix the prefix of the filename we will create, (default is blank).
   * @param const std::wstring& wExtention the file extension.
   * @return bool success or not
   */
  bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention )
  {
    return LogEvent::Instance().Initialise( wPath.c_str(), wPrefix.c_str(), wExtention.c_str() );
  }

  /**
   * Get the current logfile.
   * @param none
   * @return STD_TSTRING get the current log file.
   */
  const STD_TSTRING& GetCurrentLogFile()
  {
    return LogEvent::Instance().GetCurrentLogFile();
  }

  /**
   * Remove a LPARAM from the list. If the list is empty then we will remove it.
   * @param T the typename of the function
   * @param LPARAM the param we want to remove
   * @return bool if something was removed we will return true.
   */
  bool RemoveNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam )
  {
    return LogEvent::Instance().RemoveNotif( fnNotif, lParam );
  }

  /**
   * Add an LPARAM/Function to the list of items
   * @param T the typename of the function
   * @param LPVOID the param we want to add
   * @param size_t the number of last notifications we want to receive.
   * @return bool true if the item was added, false if it already exists.
   */
  bool AddNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast /*= 100*/  )
  {
    return LogEvent::Instance().AddNotif( fnNotif, lParam, iSendLast );
  }
} //  log
} //  myodd