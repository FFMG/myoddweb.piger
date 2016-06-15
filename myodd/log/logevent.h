#pragma once

#include "../common/includes.h"
#include "../notif/notif.h"

#include "logfile.h"
#include <vector>

typedef void(*NOTIFY_LOG)(unsigned int uiType, LPCTSTR pszLine, void* pContext);

namespace myodd {
namespace log {

  enum class LogType : unsigned int
  {
    None = 0,
    Success,
    Error,
    Warning,
    Message,
    System,
  };
  
  // the actual log class
  class LogEvent : public myodd::notif::Notifications<NOTIFY_LOG>
  {
  public:
    virtual ~LogEvent(void);

    const STD_TSTRING& GetCurrentLogFile() const {
      return m_logFile.GetCurrentLogFile();
    }

    bool Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention);
    bool Initialised() const;

  protected:

    LogEvent(void);
    const LogEvent& operator=(const LogEvent&);      // Prevent assignment

    bool _initialised;

    //  all the messages.
    class _LogMessage
    {
    public:
      _LogMessage(LogType lt = LogType::None, LPCTSTR lpMessage = NULL) :
        _uiType(lt),
        _szLogLine(lpMessage)
      {
      }
      _LogMessage(const _LogMessage& lm) { Copy(lm); }
      const _LogMessage& operator=(const _LogMessage& lm) {
        Copy(lm);
        return lm;
      }

      LPCTSTR GetMessage() const { return _szLogLine.c_str(); }
      LogType GetType() const { return _uiType; }
    protected:
      void Copy(const _LogMessage& lm)
      {
        if (this == &lm)
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
    bool LogToFile(LogType uiType, LPCTSTR pszLine);
    void LogInLockedThread(LogType uiType, LPCTSTR pszLine);

  public:
    // notification messages
    bool AddNotif(const NOTIFY_LOG& fnNotif, LPARAM lParam, size_t iSendLast);
    bool RemoveNotif(const NOTIFY_LOG& fnNotif, LPARAM lParam);

    // the actual log messages.
    void Log(LogType uFlags, LPCTSTR pszFmt, va_list argp);
    static LogEvent& Instance();
  };

} //  logevent
} //  myodd