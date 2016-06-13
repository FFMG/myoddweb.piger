// log.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _Log_h
#define _Log_h

#include "../common/includes.h"
#include "../notif/notif.h"

#include "../threads/threads.h"
#include "logfile.h"

typedef void ( *NOTIFY_LOG )(unsigned int uiType, LPCTSTR pszLine, void* pContext);

namespace myodd{ namespace log{
  // the helper functions
  void Log(LPCTSTR pszLine, ...);
  void LogSuccess(LPCTSTR pszLine, ...);
  void LogError(LPCTSTR pszLine, ...);
  void LogWarning(LPCTSTR pszLine, ...);
  void LogMessage(LPCTSTR pszLine, ...);
  void LogSystem(LPCTSTR pszLine, ...);
  
  bool AddNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast = 100 );
  bool RemoveNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam );

  const STD_TSTRING& GetCurrentLogFile();

  // the actual log class
  class LogEvent : public myodd::notif::Notifications<NOTIFY_LOG>
  {
  public:
    enum LogType
    {
      LogType_None    = 0,
      LogType_Success ,
      LogType_Error,
      LogType_Warning,
      LogType_Message,
      LogType_System,
    };
    virtual ~LogEvent(void);

    const STD_TSTRING& GetCurrentLogFile() const{
      return m_logFile.GetCurrentLogFile();
    }

    bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention);

  protected:
    LogEvent(void);
    const LogEvent& operator=(const LogEvent&);      // Prevent assignment

    //  all the messages.
    class _LogMessage
    {
    public:
      _LogMessage( LogType lt = LogType_None, LPCTSTR lpMessage = NULL ) : 
          _uiType(lt),
          _szLogLine( lpMessage )
          {
          }
      _LogMessage( const _LogMessage& lm){ Copy(lm); }
      const _LogMessage& operator=( const _LogMessage& lm ){
        Copy( lm );
        return lm;
      }

      LPCTSTR GetMessage() const{ return _szLogLine.c_str(); }
      LogType GetType() const { return _uiType; }
    protected:
      void Copy( const _LogMessage& lm )
      {
        if( this == &lm )
          return;

        _szLogLine = lm._szLogLine;
        _uiType = lm._uiType;
      }
      STD_TSTRING _szLogLine;
      LogType _uiType;
    };

    // the last few messages.
    std::vector<_LogMessage> m_logMessages;
    
  protected:
    myodd::log::LogFile m_logFile;
    bool LogToFile( LogType uiType, LPCTSTR pszLine );
    void LogInLockedThread( LogType uiType, LPCTSTR pszLine );

  public:
    // notification messages
    bool AddNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast );
    bool RemoveNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam );

    // the actual log messages.
    void Log(LogType uFlags, LPCTSTR pszFmt, va_list argp);
    static LogEvent& Instance();
  };

  //  add the debug log message
  void LogDebug( LogEvent::LogType uiType, LPCTSTR pszFmt, ...);  //  log in _DEBUG messages only
} //  log
} //  myodd
#endif // _Log_h
