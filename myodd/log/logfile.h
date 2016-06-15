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
    bool LogToFile( unsigned int uiType, LPCTSTR pszLine );
    const STD_TSTRING& GetCurrentLogFile() const{
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

    STD_TSTRING m_sPrefix;
    STD_TSTRING m_sExtention;
    STD_TSTRING m_sDirectory;
    STD_TSTRING m_sCurrentFile;
    bool m_bInOpenCall;
    FILE* m_fp;
    __int64 m_uCurrentSize;
    struct tm _tStarted;
    size_t _maxFileSizeInMegabytes;
  };
} //  log
} //  myodd
#endif // _LogFile_h