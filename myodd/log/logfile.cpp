#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "log.h"
#include "logfile.h"
#include <io.h>
#include "../files/files.h"

static LPCTSTR LogFile_default_Prefix = _T("myodd");
static LPCTSTR LogFile_default_Extention = _T("log");

namespace myodd{ namespace log{
  LogFile::LogFile() : 
    m_fp( NULL ),
    m_uCurrentSize( 0 ),
    m_bInOpenCall( false )
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

  bool LogFile::Create()
  {
    //  is the file open already.
    if ( IsOpen() )
      return true;

    // lock the current
    myodd::threads::AutoLock lock( *this );
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
      __time64_t tTime;
      struct tm tStarted;
      _time64( &tTime ); 

      errno_t err = _localtime64_s( &tStarted, &tTime );
      if( err )
      {
        //  there was a problem.
        m_bInOpenCall = true;
        return false;
      }

      //  get the log file.
      TCHAR szDateLogStarted[40];
      _tcsftime( szDateLogStarted, _countof(szDateLogStarted), _T("%Y.%m.%d"), &tStarted );

      //  we have already added a back slash at the end of the directory.
      // the file could exist already.
      m_sCurrentFile = m_sDirectory + m_sPrefix + szDateLogStarted + _T(".") + m_sExtention;

      // try and open the new file.
      m_fp = _tfsopen(m_sCurrentFile.c_str(), _T("a+b"), _SH_DENYWR);

      // did the file open?
      if( NULL == m_fp )
      {
        myodd::log::LogError( _T("Could not open log file : \"%s\"."), m_sCurrentFile.c_str() );
        bResult = false;
      }
      else
      {
        myodd::log::LogSuccess( _T("Log file, \"%s\", opened."), m_sCurrentFile.c_str() );

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

    return bResult;
  }

  /**
   * Open the file for login.
   * @param none
   * @return none
   */
  bool LogFile::Open()
  {
    //  is the file open already.
    if ( IsOpen() )
      return true;

    // lock the current
    myodd::threads::AutoLock lock( *this );
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
      m_fp = _tfsopen(m_sCurrentFile.c_str(), _T("a+b"), _SH_DENYWR);

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
    if (!IsOpen() )
      return true;

    // lock the current
    myodd::threads::AutoLock lock( *this );

    // close it.
    bool bResult = (fclose(m_fp) == 0);
    m_fp = NULL;
    m_uCurrentSize = 0;
    return bResult;
  }

  /**
   * Log an entry to the file.
   * @param unsigned int uiType the log type
   * @param LPCTSTR the line we are adding.
   * @return bool success or not.
   */
  bool LogFile::LogToFile( unsigned int uiType, LPCTSTR pszLine )
  {
    //  are we logging?
    if( m_sCurrentFile.empty() )
      return false;

    if( !Open() )
      return false;

    // lock the current
    myodd::threads::AutoLock lock( *this );

    bool bResult = false;
    try
    {
      __time64_t tTime;
      struct tm tStarted;
      _time64( &tTime ); 

      errno_t err = _localtime64_s( &tStarted, &tTime );
      if( err )
      {
        //  there was a problem.
        return false;
      }

      TCHAR szDateLogStarted[40];
      _tcsftime(szDateLogStarted, _countof(szDateLogStarted), _T("%Y/%m/%d %H:%M:%S"), &tStarted );

      //  build the return message.
      STD_TSTRING stdMsg = szDateLogStarted;
      stdMsg += _T(" ") + myodd::strings::ToString( uiType, _T("%04d") );
      stdMsg += _T(" ");
      stdMsg += (pszLine ? pszLine : _T(""));
      stdMsg += _T("\r\n");

      LPCTSTR pszMsg = stdMsg.c_str();

      // get the total size of what we are about to write..
      size_t uToWrite = _tcslen(pszMsg) * sizeof(TCHAR);

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
  * @param LPCTSTR the log path we will be using.
  * @param LPCTSTR the prefix of the filename we will create, (default is blank).
  * @param LPCTSTR the file extension.
  * @return bool success or not
  */
  bool LogFile::SetLogDirectory
  ( 
    LPCTSTR lpPath, 
    LPCTSTR lpPrefix /*= NULL*/, 
    LPCTSTR lpExtention /*= _T("log")*/ 
  )
  {
    // lock the current
    myodd::threads::AutoLock lock( *this );

    //  close the file
    Close();

    // is the new, given path valid?
    // or is the user telling us that they want to close it?
    if( NULL == lpPath )
    {
      m_sPrefix = m_sExtention = m_sDirectory = _T("");
      return true;
    }

    //  set the prefix and extension.
    // if the user is giving us some silly values then there is nothing we can do about really.
    m_sPrefix     = lpPrefix    ? lpPrefix    : LogFile_default_Prefix;
    m_sExtention  = lpExtention ? lpExtention : LogFile_default_Extention;

    // It seems to be valid, so we can set the path here.
    m_sDirectory = lpPath;

    //  expand the file name
    myodd::files::ExpandEnvironment( m_sDirectory, m_sDirectory );

    //  add a trailing backslash
    myodd::files::AddTrailingBackSlash( m_sDirectory );

    //  expand the path, in case the user changes their path or something.
    if( !myodd::files::GetAbsolutePath( m_sDirectory, m_sDirectory ) )
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