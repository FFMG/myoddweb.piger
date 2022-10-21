#include "stdafx.h"
#include "logevent.h"

// this is the maximum number of messages we will keep in memory
// those messages are kept when new apps log in so they can have the last handful of messages.
static size_t MAX_NUM_LOG_MESSAGES = 25;

namespace myodd { namespace log {
  LogEvent& LogEvent::Instance()
  {
    static LogEvent logEvent;
    return logEvent;
  }

  LogEvent::~LogEvent(void)
  {
    //  the log will close itself.
    _initialised = false;
  }

  LogEvent::LogEvent(void) : _initialised(false)
  {
  }

  bool LogEvent::Initialised() const
  {
    myodd::threads::AutoLock al(*this);
    return _initialised;
  }

  /**
  * Remove a MYODD_LPARAM from the list. If the list is empty then we will remove it.
  * @param T the typename of the function
  * @param MYODD_LPARAM the param we want to remove
  * @return bool if something was removed we will return true.
  */
  bool LogEvent::RemoveNotif(const LogEventCallback& fnNotif, MYODD_LPARAM lParam)
  {
    myodd::threads::AutoLock autoLock(*this);
    return __super::RemoveNotif(fnNotif, lParam);
  }

  /**
  * Add an long/Function to the list of items
  * @param T the typename of the function
  * @param MYODD_LPARAM lParam the param we want to add
  * @param size_t the number of last notifications we want to receive.
  * @return bool true if the item was added, false if it already exists.
  */
  bool LogEvent::AddNotif(const LogEventCallback& fnNotif, MYODD_LPARAM lParam, size_t iSendLast)
  {
    // Lock the thread
    myodd::threads::AutoLock autoLock(*this);

    // send the last few messages
    int iSend = static_cast<int>(min(m_logMessages.size() - 1, iSendLast));
    while (iSend >= 0)
    {
      _LogMessage& lm = m_logMessages[iSend--];
      fnNotif(lm.GetType(), lm.GetMessage(), (void*)lParam);
    }

    // add it to the list
    return __super::AddNotif(fnNotif, lParam);
  }

  /**
  * Log an event
  * @param LogType the type of the event been logged
  * @param const wchar_t* the unformatted buffer.
  * @param va_list the list of arguments we will add to the buffer.
  * @return
  */
  void LogEvent::Log(LogType uiType, const wchar_t* pszLine, va_list argp)
  {
    ASSERT(pszLine != NULL);

    wchar_t* buffer = NULL;
    int len = _vsctprintf(pszLine, argp);
    if (len >0)
    {
      // _vscprintf doesn't count + 1; terminating '\0'
      ++len;
      buffer = new wchar_t[len];
      _vsntprintf_s(buffer, len, len, pszLine, argp);
    }

    Log(uiType, buffer);
    if (NULL != buffer)
    {
      delete[] buffer;
    }
  }

  /**
  * Log an event
  * @param LogType the type of the event been logged
  * @param const wchar_t* the unformatted buffer.
  * @return
  */
  void LogEvent::Log(LogType uiType, const wchar_t* pszLine )
  {
    ASSERT(pszLine != NULL);

    // try and Lock the thread
    myodd::threads::AutoLockTry autoLockTry(*this);
    if (autoLockTry.TryLock(20))
    {
      LogInLockedThread(uiType, pszLine);
    }
  }

  /**
  * Log a message and send notifications.
  * This assumes that we have the thread lock.
  * @param LogType the message been logged.
  * @param const wchar_t* the text been logged.
  * @return none
  */
  void LogEvent::LogInLockedThread(LogType uiType, const wchar_t* pszLine)
  {
    //  then we need to notify all this new message.
    MYODD_LPARAM lParam;
    LogEventCallback fnNotif;
    unsigned idx = 0;
    while (GetNotif(idx++, fnNotif, lParam))
    {
      (*fnNotif)(uiType, pszLine ? pszLine : L"", (void*)lParam);
    }

    // log it to the file
    LogToFile(uiType, pszLine);

    //  and add it to the list, in case we have future registrations.
    _LogMessage lm(uiType, pszLine);
    m_logMessages.push_back(lm);

    //  remove if we have too many.
    while (m_logMessages.size() > MAX_NUM_LOG_MESSAGES)
    {
      m_logMessages.erase( m_logMessages.begin() );
    }
  }

  /**
   * Set the log path directory.
   * If we pass a NULL argument then we want to stop/disable the logging.
   * @param const std::wstring& wPath the log path we will be using.
   * @param const std::wstring& wPrefix the prefix of the filename we will create, (default is blank).
   * @param const std::wstring& wExtention the file extension.
   * @param size_t maxFileSize the max file size in megabytes.
   * @return bool success or not
   */
  bool LogEvent::Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention, size_t maxFileSize )
  {
    // we cannot do that more than once.
    if (Initialised())
    {
      return false;
    }

    // try and get the lock
    myodd::threads::AutoLock autoLock(*this);

    // check again if we are initialised.
    if (Initialised())
    {
      return false;
    }

    // initialise the log file.
    if (!m_logFile.Initialise(wPath, wPrefix, wExtention, maxFileSize ))
    {
      return false;
    }

    // we are now initialised
    _initialised = true;

    //  of the path is zero then we are not really creating anything
    // this just mean that we are not logging anything.
    if (wPath.length() == 0)
    {
      return true;
    }

    // send the last few messages
    for (auto it = m_logMessages.begin();
      it != m_logMessages.end();
      ++it
      )
    {
      const _LogMessage& lm = (*it);
      LogToFile(lm.GetType(), lm.GetMessage());
    }// each messages.

    return true;
  }

  /**
  * Log an entry to the file.
  * @param unsigned int uiType the log type
  * @param const wchar_t* the line we are adding.
  * @return
  */
  bool LogEvent::LogToFile(LogType uiType, const wchar_t* pszLine)
  {
    // Lock the thread
    myodd::threads::AutoLock autoLock(*this);

    //
    return m_logFile.LogToFile((unsigned int)uiType, pszLine);
  }
} //  logevent
} //  myodd
