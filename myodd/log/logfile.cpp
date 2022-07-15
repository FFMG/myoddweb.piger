#include "stdafx.h"
#include "log.h"
#include "logfile.h"
#include <io.h>

#include <chrono>

#include "../math/math.h"
#include "../files/files.h"

// the maxfile size we want to use.
static size_t MAX_FILE_SIZE_IN_MEGABYTES = 5;

static const wchar_t* LogFile_default_Prefix = L"myodd";
static const wchar_t* LogFile_default_Extention = L"log";

namespace myodd{ namespace log{
  LogFile::LogFile() : 
    m_fp( NULL ),
    m_uCurrentSize( 0 ),
    m_bInOpenCall( false ),
    _maxFileSizeInMegabytes(MAX_FILE_SIZE_IN_MEGABYTES)
  {
  }

  LogFile::~LogFile()
  {
    //  close the log file.
    Close();
  }

  /**
   * Check if the log file is open or not.
   * @param none
   * @return bool if the log file is open or not.
   */
  bool LogFile::IsOpen() const
  {
    return ( m_fp != NULL && !m_bInOpenCall );
  }

  struct tm LogFile::GetCurrentTimeStruct() const
  {
    // the current time.
    auto timeNow = std::chrono::system_clock::now();

    // to a time_t struct
    auto tTime = std::chrono::system_clock::to_time_t(timeNow);

    // to a tm structure.
    return *localtime(&tTime);
  }

  /**
   * Updated the started time of the log file.
   * this will set the _tStarted structure.
   */
  void LogFile::UpdateStartedTime()
  {
    // update the started time with the current structure.
    _tStarted = GetCurrentTimeStruct();
  }

  /**
   * Create the log file given the prefix and extension
   * We will check that the file size if within limit
   * And the date/time is up to date.
   */
  bool LogFile::Create()
  {
    //  is the file open already.
    if (IsOpen())
    {
      return true;
    }

    // lock the current
    if( m_bInOpenCall )
    {
      return false;
    }

    //  we are in open call
    // that way we can log things ourselves.
    m_bInOpenCall = true;

    bool bResult = false;
    try
    {
      // get the time now.
      UpdateStartedTime();

      // look for a file that matches
      for ( unsigned int i = 0;; ++i )
      {
        //  get the log file.
        wchar_t szDateLogStarted[40] = {};
        _tcsftime(szDateLogStarted, _countof(szDateLogStarted), L"%Y-%m-%d", &_tStarted );

        // add a number if the file number is > 0
        // so we will create more than one file for today if need be.
        wchar_t szFileCount[10] = {};
        if (i > 0) 
        {
          swprintf(szFileCount, 10, L"-%u", i);
        }

        //  we have already added a back slash at the end of the directory.
        // the file could exist already.
        std::wstring proposedCurrentFile = m_sDirectory + m_sPrefix + szDateLogStarted + szFileCount + L"." + m_sExtention;

        double fileSizeInMegabytes = myodd::math::BytesToMegabytes(myodd::files::GetFileSizeInBytes(proposedCurrentFile));
        if (fileSizeInMegabytes < GetMaxFileSizeInMegabytes())
        {
          m_sCurrentFile = proposedCurrentFile;
          break;
        }
      }

      // try and open the new file.
      m_fp = _tfsopen(m_sCurrentFile.c_str(), L"a+b", _SH_DENYWR);

      // did the file open?
      if( NULL == m_fp )
      {
        myodd::log::LogError( L"Could not open log file : \"%s\".", m_sCurrentFile.c_str() );
        bResult = false;
      }
      else
      {
        myodd::log::LogSuccess( L"Log file, \"%s\", opened.", m_sCurrentFile.c_str() );

        //  get the size of the file.
        m_uCurrentSize = _filelengthi64(_fileno(m_fp));

        // is it a brand new file?
        if( m_uCurrentSize == 0 )
        {
          // add the unicode byte order format.
          fputwc(0xFEFF, m_fp);
        }
#ifdef _DEBUG
        // just make sure that we have the right format.
        // if the file was changed by the user, then something will go wrong at some stage.
        // but we cannot go around and police user misbehaving
        //
        // so the debug version will make sure that the app is not misbehaving.
        else if (m_uCurrentSize >= sizeof(WORD))
        {
          WORD wBOM;
          if (fread(&wBOM, sizeof(wBOM), 1, m_fp) == 1)
          {
            assert( wBOM == 0xFEFF );
          }
        }
#endif // Check for the UNICODE param

        // go to the end of the file.
        (void)fseek(m_fp, 0, SEEK_END);
      }

      //  if we made it here, then it worked.
      bResult = true;
    }
    catch ( ... )
    {
      bResult = false;
    }

    //  we are no longer in open call.
    m_bInOpenCall = false;

    // return if this worked.
    return bResult;
  }

  /**
   * Ensure that the file is not too big and not out of date.
   */
  void LogFile::ValidateDateAndSize()
  {
    if (!IsOpen())
    {
      return;
    }

    // recreate?
    bool bRecreate = false;

    // check the size.
    double fileSizeInMegabytes = myodd::math::BytesToMegabytes(myodd::files::GetFileSizeInBytes( GetCurrentLogFile()));
    if (fileSizeInMegabytes > GetMaxFileSizeInMegabytes())
    {
      bRecreate = true;
    }
    else
    {
      // get the current time and check if the file is out of date.
      auto tStarted = GetCurrentTimeStruct();

      //  compare it to the started time.
      auto tm = GetStartedDate();
      if (tm.tm_yday != tStarted.tm_yday)
      {
        //  close the old one and force a re-open
        // this will force a check to be done.
        bRecreate = true;
      }
    }

    if (bRecreate)
    {
      // close the old file
      Close();

      // create a new one.
      Create();
    }
  }

  /**
   * Open the file for login.
   * @param none
   * @return none
   */
  bool LogFile::Open()
  {
    //  is the file open already.
    if (IsOpen())
    {
      return true;
    }

    // lock the current
    if( m_bInOpenCall )
    {
      return false;
    }

    //  we are in open call
    // that way we can log things ourselves.
    m_bInOpenCall = true;

    //  assume an error.
    bool bResult = false;
    try
    {
      // do we already know the name of the file?
      if( m_sCurrentFile.empty() )
      {
        if( !Create() )
        {
          //  there was a problem.
          m_bInOpenCall = true;
          return false;
        }
      }

      // try and open the new file.
      m_fp = _tfsopen(m_sCurrentFile.c_str(), L"a+b", _SH_DENYWR);

      //  get the size of the file.
      m_uCurrentSize = _filelengthi64(_fileno(m_fp));

      // go to the end of the file.
      (void)fseek(m_fp, 0, SEEK_END);

      //  if we made it here, then it worked.
      bResult = true;
    }
    catch ( ... )
    {
      bResult = false;
    }

    //  we are no longer in open call.
    m_bInOpenCall = false;

    return bResult;
  }

  /**
   * Close a log file.
   * @param none
   * @return bool if the file was closed or not.
   */
  bool LogFile::Close()
  {
    // is it open?
    if (!IsOpen())
    {
      return true;
    }

    // close it.
    bool bResult = (fclose(m_fp) == 0);
    m_fp = NULL;
    m_uCurrentSize = 0;
    return bResult;
  }

  /**
   * Log an entry to the file.
   * @param unsigned int uiType the log type
   * @param const wchar_t* the line we are adding.
   * @return bool success or not.
   */
  bool LogFile::LogToFile( unsigned int uiType, const wchar_t* pszLine )
  {
    //  are we logging?
    if (m_sCurrentFile.empty())
    {
      return false;
    }

    if (!Open())
    {
      return false;
    }

    // check the size and date
    ValidateDateAndSize();

    bool bResult = false;
    try
    {
      // get the current date so we can add it to the log.
      auto tStarted = GetCurrentTimeStruct();

      wchar_t szDateLogStarted[40];
      _tcsftime(szDateLogStarted, _countof(szDateLogStarted), L"%Y/%m/%d %H:%M:%S", &tStarted );

      //  build the return message.
      std::wstring stdMsg = szDateLogStarted;
      stdMsg += L" " + myodd::strings::ToString( uiType, L"%04d");
      stdMsg += L" ";
      stdMsg += (pszLine ? pszLine : L"");
      stdMsg += L"\r\n";

      const wchar_t* pszMsg = stdMsg.c_str();

      // get the total size of what we are about to write..
      size_t uToWrite = _tcslen(pszMsg) * sizeof(wchar_t);

      //  then we can write to the file.
      size_t uWritten = fwrite(pszMsg, 1, uToWrite, m_fp);

      //  did it work?
      bResult = !ferror(m_fp);

      //  get the file size.
      m_uCurrentSize += uWritten;
    }
    catch( ... )
    {
    }
    Close();
    return bResult;
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
  bool LogFile::Initialise(const std::wstring& wPath, const std::wstring& wPrefix, const std::wstring& wExtention, size_t maxFileSize)
  {
    //  close the file
    Close();

    // is the new, given path valid?
    // or is the user telling us that they want to close it?
    if( wPath.length() == 0 )
    {
      m_sPrefix = m_sExtention = m_sDirectory = L"";
      return true;
    }

    //  set the prefix and extension.
    // if the user is giving us some silly values then there is nothing we can do about really.
    m_sPrefix     = wPrefix.length() > 0    ? wPrefix    : LogFile_default_Prefix;
    m_sExtention  = wExtention.length() > 0 ? wExtention : LogFile_default_Extention;

    // It seems to be valid, so we can set the path here.
    m_sDirectory = wPath;

    //  expand the file name
    if (!myodd::files::ExpandEnvironment(m_sDirectory, m_sDirectory))
    {
      return false;
    }

    //  add a trailing backslash
    myodd::files::AddTrailingBackSlash( m_sDirectory );

    //  expand the path, in case the user changes their path or something.
    if( !myodd::files::GetAbsolutePath( m_sDirectory, m_sDirectory, L"." ))
    {
      return false;
    }

    // expand the path, in case the user changes their path or something.
    if( !myodd::files::CreateFullDirectory( m_sDirectory.c_str(), false ) )
    {
      return false;
    }

    // simply open the next file
    // if there is a problem then nothing will log.
    return Create();
  }
} //  log
} //  myodd