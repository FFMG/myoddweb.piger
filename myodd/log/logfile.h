#pragma once
#ifndef _LogFile_h
#define _LogFile_h

#include "../string/string.h"
#include "../threads/threads.h"

namespace myodd{ namespace log{
  class LogFile
  {
  public:
    LogFile();
    virtual ~LogFile();

    bool Initialise(const std::wstring& lpPath, const std::wstring& lpPrefix, const std::wstring& lpExtention, size_t maxFileSize );
    bool IsOpen() const;
    bool LogToFile( unsigned int uiType, const MYODD_CHAR* pszLine );
    const MYODD_STRING& GetCurrentLogFile() const{
      return m_sCurrentFile;
    }

    /** 
     * Get when this log file was started.
     */
    const struct tm& GetStartedDate() const {
      return _tStarted;
    }

    const size_t GetMaxFileSizeInMegabytes() const {
      return _maxFileSizeInMegabytes;
    }
  protected:
    bool Close();
    bool Open();
    bool Create();
    void ValidateDateAndSize();

    MYODD_STRING m_sPrefix;
    MYODD_STRING m_sExtention;
    MYODD_STRING m_sDirectory;
    MYODD_STRING m_sCurrentFile;
    bool m_bInOpenCall;
    FILE* m_fp;
    __int64 m_uCurrentSize;
    struct tm _tStarted;
    size_t _maxFileSizeInMegabytes;
  };
} //  log
} //  myodd
#endif // _LogFile_h