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

  enum class LogType : unsigned int
  {
    None = 0,
    Success,
    Error,
    Warning,
    Message,
    System,
  };

  // the helper functions
  void Log(LPCTSTR pszLine, ...);
  void LogSuccess(LPCTSTR pszLine, ...);
  void LogError(LPCTSTR pszLine, ...);
  void LogWarning(LPCTSTR pszLine, ...);
  void LogMessage(LPCTSTR pszLine, ...);
  void LogSystem(LPCTSTR pszLine, ...);

  //  add the debug log message
  void LogDebug(LogType uiType, LPCTSTR pszFmt, ...);  //  log in _DEBUG messages only

  
  bool AddNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast = 100 );
  bool RemoveNotif( const NOTIFY_LOG& fnNotif, LPARAM lParam );

  const STD_TSTRING& GetCurrentLogFile();

  // the actual log class
  class LogEvent : public myodd::notif::Notifications<NOTIFY_LOG>
  {
  public:
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
      _LogMessage( LogType lt = LogType::None, LPCTSTR lpMessage = NULL ) :
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

  //  initialise the various events.
  bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention);
} //  log
} //  myodd
#endif // _Log_h
