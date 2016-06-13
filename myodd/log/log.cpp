#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "log.h"

// this is the maximum number of messages we will keep in memory
// those messages are kept when new apps log in so they can have the last handful of messages.
static unsigned short MAX_NUM_LOG_MESSAGES = 25;

namespace myodd{ namespace log{
  void LogDebug( LogEvent::LogType uiType, LPCTSTR pszFmt, ...)
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
	  LogEvent::Instance().Log( LogEvent::LogType_None, pszFmt, argp);
	  va_end(argp);
  }

  void LogSuccess(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log( LogEvent::LogType_Success, pszFmt, argp);
	  va_end(argp);
  }

  void LogError(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log( LogEvent::LogType_Error, pszFmt, argp);
	  va_end(argp);
  }

  void LogWarning(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log( LogEvent::LogType_Warning, pszFmt, argp);
	  va_end(argp);
  }

  void LogMessage(LPCTSTR pszFmt, ...)
  {
	  va_list argp;
	  va_start(argp, pszFmt);
	  LogEvent::Instance().Log( LogEvent::LogType_Message, pszFmt, argp);
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
    LogEvent::Instance().Log( LogEvent::LogType_System, pszFmt, argp);
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

  LogEvent& LogEvent::Instance()
  {
    static LogEvent logEvent;
    return logEvent;
  }

  LogEvent::~LogEvent(void)
  {
    //  the log will close itself.
  }

  LogEvent::LogEvent(void)
  {
  }

  /**
   * Remove a LPARAM from the list. If the list is empty then we will remove it.
   * @param T the typename of the function
   * @param LPARAM the param we want to remove
   * @return bool if something was removed we will return true.
   */
  bool LogEvent::RemoveNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam )
  {
    return __super::RemoveNotif( fnNotif, lParam );
  }

  /**
   * Add an LPARAM/Function to the list of items
   * @param T the typename of the function
   * @param LPVOID the param we want to add
   * @param size_t the number of last notifications we want to receive.
   * @return bool true if the item was added, false if it already exists.
   */
  bool LogEvent::AddNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast )
  {
    // Lock the thread
    myodd::threads::AutoLock autoLock( *this );

    // send the last few messages
    int iSend = static_cast<int>(min( m_logMessages.size()-1, iSendLast ));
    while( iSend >= 0 )
    {
      _LogMessage& lm = m_logMessages[ iSend-- ];
      fnNotif( lm.GetType(), lm.GetMessage(), (void*)lParam );
    }

    // add it to the list
    return __super::AddNotif( fnNotif, lParam );
  }

  /**
   * Log an event
   * @param LogType the type of the event been logged
   * @param LPCTSTR the unformatted buffer.
   * @param va_list the list of arguments we will add to the buffer.
   * @return 
   */
  void LogEvent::Log(LogType uiType, LPCTSTR pszLine, va_list argp)
  {
    // try and Lock the thread
    myodd::threads::AutoLockTry autoLockTry( *this );
    if( !autoLockTry.TryLock( 20 ) )
    {
      return;
    }

    ASSERT(pszLine != NULL);

    TCHAR* buffer = NULL;
    int len = _vsctprintf( pszLine, argp );
    if( len >0 )
    {
      // _vscprintf doesn't count + 1; terminating '\0'
      ++len;
      buffer = new TCHAR[len];
      _vsntprintf_s(buffer, len, len, pszLine, argp);
    }

    // we still have the lock!
    LogInLockedThread( uiType, buffer );

    if( NULL != buffer )
    {
      delete [] buffer;
    }
  }

  /**
   * Log a message and send notifications.
   * This assumes that we have the thread lock.
   * @param LogType the message been logged.
   * @param LPCTSTR the text been logged.
   * @return none
   */
  void LogEvent::LogInLockedThread( LogType uiType, LPCTSTR pszLine )
  {
    //  then we need to notify all this new message.
    LPARAM lParam;
    NOTIFY_LOG fnNotif;
    unsigned idx = 0;
    while( GetNotif( idx++, fnNotif, lParam ) )
    {
      (*fnNotif)( uiType, pszLine ? pszLine : _T(""), (void*)lParam );
    }

    // log it to the file
    LogToFile( uiType, pszLine );

    //  and add it to the list, in case we have future registrations.
    _LogMessage lm( uiType, pszLine );
    m_logMessages.push_back( lm );
    while( m_logMessages.size() > MAX_NUM_LOG_MESSAGES )
    {
      m_logMessages.erase( m_logMessages.begin() );
    }
  }

  /**
  * Set the log path directory.
  * If we pass a NULL argument then we want to stop/disable the logging.
  * @param LPCTSTR the log path we will be using.
  * @param LPCTSTR the prefix of the filename we will create, (default is blank).
  * @param LPCTSTR the file extension.
  * @return bool success or not
  */
  bool LogEvent::Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention)
  {
    if (!m_logFile.Initialise(wPath, wPrefix, wExtention))
    {
      return false;
    }

    //  of the path is zero then we are not really creating anything
    // this just mean that we are not logging anything.
    if (wPath.length() == 0)
    {
      return true;
    }

    // Lock the thread
    // if we cannot lock those message then they are lost forever
    // but at least we don't have a deadlock
    myodd::threads::AutoLockTry autoLockTry( *this );
    if( autoLockTry.HasLock() )
    {
      // send the last few messages
      for( std::vector<_LogMessage>::const_iterator it = m_logMessages.begin();
            it != m_logMessages.end();
            ++it
          )
      {
        const _LogMessage& lm = (*it);
        LogToFile( lm.GetType(), lm.GetMessage() );
      }// each messages.
    }// try to lock
    return true;
  }

  /**
  * Log an entry to the file.
  * @param unsigned int uiType the log type
  * @param LPCTSTR the line we are adding.
  * @return 
  */
  bool LogEvent::LogToFile( LogType uiType, LPCTSTR pszLine )
  {
    // Lock the thread
    myodd::threads::AutoLock autoLock( *this );

    return m_logFile.LogToFile( uiType, pszLine );
  }
} //  log
} //  myodd