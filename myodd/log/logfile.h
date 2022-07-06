#pragma once
#ifndef _LogFile_h
#define _LogFile_h

#include "../string/string.h"

#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace myodd{ namespace log{
  class LogFile
  {
  public:
    LogFile();
    virtual ~LogFile();

    bool Initialise(const std::wstring& lpPath, const std::wstring& lpPrefix, const std::wstring& lpExtention, size_t maxFileSize );
    bool IsOpen() const;
    bool LogToFile( unsigned int uiType, const MYODD_CHAR* pszLine );
    const std::wstring& GetCurrentLogFile() const{
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

    std::wstring m_sPrefix;
    std::wstring m_sExtention;
    std::wstring m_sDirectory;
    std::wstring m_sCurrentFile;
    bool m_bInOpenCall;
    FILE* m_fp;
    __int64 m_uCurrentSize;
    struct tm _tStarted;
    size_t _maxFileSizeInMegabytes;

    void UpdateStartedTime();
    struct tm GetCurrentTimeStruct() const;
  };
} //  log
} //  myodd
#endif // _LogFile_h