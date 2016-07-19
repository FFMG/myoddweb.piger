#include "stdafx.h"

#include "../files/files.h"
#include "../string/string.h"
#include "../math/math.h"
#include "../log/log.h"
#include <io.h>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

// Look for version.lib
#pragma comment( lib, "version.lib" )

static const MYODD_CHAR* FILE_APPPATH = _T("%apppath%");

/**
 * TODO : Maybe move to myodd::strings
 * TODO : Why are we getting dead chars in the first place? 
 * This came from casting in the Clipboard, so maybe data is not copied properly.
 *
 * @param MYODD_STRING& the string we want to remove dead code from
 * @return none 
 */
inline void _TrimDeadChars( MYODD_STRING& s )
{
  auto l = s.length();
  while( l>1 && s[--l] == 0 )
  {
    s = s.substr(0, l );
  }
  while (l>0 && s[0] == 0)
  {
    s = s.substr(1, --l);
  }
}

// this is the separator that we will be converting everything to if we are on an MS System.
static const MYODD_CHAR* MYODD_FILE_WINSEPARATOR = _T("\\");

// the default separator
static const MYODD_CHAR* MYODD_FILE_SEPARATOR   = MYODD_FILE_WINSEPARATOR;

namespace myodd{ namespace files{
void Test()
#ifdef _DEBUG
{
  ASSERT( GetBaseFromFile( _T("c:\\a\\b\\something.txt"), false ) == _T("c:\\a\\b\\") );
  ASSERT( GetBaseFromFile( _T("c:\\a\\b\\something.txt"), false, false ) == _T("c:\\a\\b") );
  ASSERT( GetBaseFromFile( _T("C:\\test.txt" ) ) == _T("C:\\"));
  ASSERT( GetBaseFromFile( _T("test.txt" ) ) == MYODD_FILE_SEPARATOR );
  ASSERT( GetBaseFromFile( _T("C:\\dir/test.txt" ) ) == _T("C:\\dir\\"));
  ASSERT( GetBaseFromFile( _T("C:\\" ) ) == _T("C:\\"));
  
  MYODD_CHAR* lpdest = nullptr;
  ASSERT( GetAbsolutePath( lpdest, _T( "../../somefile.txt" ), _T( "c:\\dira\\dirb\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T("c:\\somefile.txt") ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;

  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\.\\Application Data\\..\\..\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;
  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\..\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\All Users\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;
  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\..\\Application Data\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;

  ASSERT( GetAbsolutePath( lpdest, _T("somepath2\\"), _T( "C:\\somepath1\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\somepath1\\somepath2\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;


  // this should not work because of the depth of the path
  ASSERT( !GetAbsolutePath( lpdest, _T( "../../../somefile.txt" ), _T( "c:\\dira\\dirb\\" ) ) );

  MYODD_STRING appPath = GetAppPath( false );
  ASSERT( ExpandEnvironment( FILE_APPPATH, lpdest ) );
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  ASSERT( ExpandEnvironment( _T("%AppPath%"), lpdest ) );   //  case insensitive
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;
  ASSERT( ExpandEnvironment( _T("%AppPath%\\somedir\\somefile.txt"), lpdest ) );   //  case insensitive
  MYODD_STRING s = appPath + _T("\\somedir\\somefile.txt" );
  ASSERT( _tcsicmp( lpdest, s.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;

  ASSERT( UnExpandEnvironment( _T("%systemdrive%"), lpdest ) );
  delete [] lpdest;

  ASSERT( UnExpandEnvironment( appPath.c_str(), lpdest ) );
  ASSERT( _tcsicmp( lpdest, FILE_APPPATH ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;

  MYODD_STRING dirtyFileName = _T("[bad]^*£");
  CleanFileName( dirtyFileName );
  ASSERT( dirtyFileName == _T("_bad___£"));

  ASSERT( GetFileName( _T("c:\\a\\b\\something.txt"), false ) == _T("something.txt") );
}
#else
{
  __noop;
}
#endif // _DEBUG

/**
 * Add an extension to a given string/filename
 * The extension does not need to have a '.', a check is already made.
 * @param const MYODD_STRING& the 'file' or string we want to add the extension to.
 * @param const MYODD_STRING& the extension we want to add to the string.
 * @param bool strip the current extension if there is one.
 * @return none
 */
void AddExtension(MYODD_STRING& filename, const MYODD_STRING& extension, bool strip_current_if_exists ) 
{
  _TrimDeadChars( filename );
  if( filename.length() == 0 || extension.length() == 0 )
  {
    // nothing to do...
    return;
  }

  if( true == strip_current_if_exists )
  {
    StripExtension(filename); //  remove the current extension.
  }

  if( *filename.rbegin() == _T('.') )
  {
    filename = filename.substr( 0, filename.length() -1 );
  }

  // do we have a dot in the extension?
  // we already know the size is not 0
  if( *extension.begin() == _T('.') )
  {
    filename += extension;
  }
  else
  {
    filename += _T('.') + extension;
  }
}

/**
 * remove the file extension
 * this is for cosmetic reasons only, it does nothing for the file itself
 *
 * We might want to remove this if we use the same file name with different extensions.
 * I added a little check in case the file name only has a '.' at the start, this is because
 * I program Apache and '.htaccess' is a valid file name, (but why would it be used as a command lord only knows).
 * But that way you can create commands starting with a '.' for what ever reason.
 *
 * @param MYODD_STRING& the string we want to remove the extension from.
 * @return none 
 */
void StripExtension( MYODD_STRING& filename)
{
  _TrimDeadChars(filename);

  auto charFilename = filename.c_str();
  auto pdest = _tcsrchr(charFilename, '.' );

  if( pdest != nullptr )
  {
    // we do not want to remove the extension from something like '.htaccess'
    // this is clearly not the extension but the filename.
    auto result = (pdest - (charFilename));
    if( result >0 && result < filename.length() )
    {
      filename = filename.substr( 0, result );
    }
  }
}

/**
 * get the file extension, if any. Return an empty string if there isn't one.
 * @param MYODD_STRING the file name
 * @return MYODD_STRING the extension, (or empty string if there are none).
 */
MYODD_STRING GetExtension( const MYODD_STRING& filename )
{
  // make a copy so we can strip the dead characters.
  auto filenameCopy(filename);
  _TrimDeadChars(filenameCopy);

  // look for the last '.'
  auto pos = filenameCopy.find_last_of( _T('.') );

  // we don't want to return extensions like 'htaccess' from a file '.htaccess'
  // this is because the file name itself is '.htaccess' with no extention.
#ifdef _UNICODE
  return (pos > 0 && pos != std::wstring::npos ? filenameCopy.substr( pos+1 ) : L"" );
#else
  return (pos > 0 && pos != std::string::npos ? filenameCopy.substr(pos + 1) : "");
#endif
}

/**
 * Check if a files extension is the one given
 * @param MYODD_STRING the file name
 * @param MYODD_STRING the extension we are checking for.
 * @return bool if the file extension is the one we wanted.
 */
bool IsExtension( const MYODD_STRING& fOriginal, const MYODD_STRING& fExt )
{
  auto f( fOriginal );
  _TrimDeadChars( f );
  strings::Trim(f);

  auto e( fExt );
  _TrimDeadChars( e );
  strings::Trim(e);
  if( e.length() > 1 && e[0] == '.' )
  {
    e = e.substr( 1 );
  }

  if( f.length() == 0 || e.length() == 0 )
  {
    return false; //  we cannot check 0 lengths
  }

  e = strings::Replace( e, _T("\\."), _T("." ));     //  in case the user escaped it already
  e = strings::Replace( e, _T("."), _T("\\." ));     //  escape it now.
  auto stdMatches = _T("^(.*)\\.(") + e + _T(")$");
#ifdef _UNICODE
  boost::wsmatch matches;
  const boost::wregex aStringregex( stdMatches.c_str(), boost::regex_constants::icase );
#else
  boost::smatch matches;
  const boost::regex aStringregex( stdMatches.c_str(), boost::regex_constants::icase );
#endif

  if (boost::regex_match(f, matches, aStringregex))
  {
    return true;
  }
  return false;
}


/**
 * Remove a leading backslash at the start of a path
 * This is normally used so we can joining 2 paths together.
 * @param MYODD_STRING the path we want to remove the backslash from
 * @return none
 */
void RemoveLeadingBackSlash( MYODD_STRING& szPath)
{
  _TrimDeadChars( szPath );

  int idx = static_cast<int>(szPath.length())-1;
  if( idx<0 )
  {
    return;
  }

  while( idx >= 0 && (szPath[ 0 ] == '\\' || szPath[ 0 ] == '/'))
  {
    szPath = szPath.substr( 1, idx-- );
  }
}

/**
 * Look for a trailing back slash at the end of the path and strip it.
 * @param MYODD_STRING& the string we want to remove the trailing back slash
 * @return none 
 */
void RemoveTrailingBackSlash
(	
 MYODD_STRING& szPath
)
{
  _TrimDeadChars( szPath );
  
  int idx = static_cast<int>(szPath.length( ))-1;
	if( idx<0 )
  {
		return;
  }

  while( idx >= 0 && (szPath[ idx ] == '\\' || szPath[ idx ] == '/'))
  {
		szPath = szPath.substr( 0, idx-- );
  }
}

/**
 * Add a trailing back slash at the end of a directory
 * @see AddTrailingBackSlash( MYODD_STRING& s )
 *
 * @param MYODD_STRING& the string we want to add a trailing back slash
 * @return none 
 */
void AddTrailingBackSlash( MYODD_STRING& subPath )
{
  //  remove the dead characters.
  _TrimDeadChars( subPath );

  //  remove the trailing backslash
  RemoveTrailingBackSlash(subPath);

  //  then just add the trailing back slash
  subPath += MYODD_FILE_SEPARATOR;
}

/**
 * UnExpand a string path into environment variables.
 * that way the user can pass a full path and get the environment variable.
 * This ensures that the user can change/rename paths and we should still be able to locate the files.
 * the calling function is responsible for cleaning the code.
 *
 * @param const MYODD_STRING the path we are editing
 * @param MYODD_STRING& the return file we are unexpanding.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest )
{
  MYODD_CHAR* unExpandEd;
  if( !files::UnExpandEnvironment( src.c_str(), unExpandEd ))
  {
    return false;
  }

  if( unExpandEd )
  {
    dest = unExpandEd;
    delete [] unExpandEd;
  }
  else
  {
    dest = _T("");
  }  
  return true;
}

/**
 * UnExpand a string path into environment variables.
 * that way the user can pass a full path and get the environment variable.
 * This ensures that the user can change/rename paths and we should still be able to locate the files.
 * the calling function is responsible for cleaning the code.
 *
 * @param const MYODD_CHAR* the path we are editing
 * @param MYODD_CHAR* the object that will contain the unexpended file, (if return true), the calling function must delete this file.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment(const MYODD_CHAR* lpSrc, MYODD_CHAR*& dest )
{
  dest = nullptr;
  if(nullptr == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  MYODD_STRING stdUnDst = _T("");
  size_t unExpandSize = 0;

  MYODD_STRING stdSrc = lpSrc;

  //  we don't want the trailing back slash
  static MYODD_STRING appPath = GetAppPath( false );
  if( MYODD_STRING::npos != strings::Find( lpSrc, appPath, 0, false ) )
  {
    // we have to expand the app path first
    // otherwise the drive letter will be replaced by %systemdrive%
    stdUnDst = strings::Replace( stdSrc, appPath, FILE_APPPATH, false );
    unExpandSize = stdUnDst.length();
  }
  else
  {
    MYODD_CHAR unDst[ T_MAX_PATH ];
    memset( unDst, 0, T_MAX_PATH );
    if( !PathUnExpandEnvStrings( stdSrc.c_str(), unDst, T_MAX_PATH ) )
    {
      // the user could have passed us an invalid folder
      // or a valid slave drive (D:\hello\) that does not get unexpanded.
      _tcscpy_s( unDst, T_MAX_PATH, stdSrc.c_str() );
    }

    unExpandSize = _tcslen( unDst );
    if( 0 == unExpandSize )
    {
      // we could not get the unexpended file for some reason.
      return false;
    }
    stdUnDst = unDst;
  }
  
  // for the null char
  ++unExpandSize;

  // Create the expanded item with the size we have.
  // and set it to 0 the terminating null should be included.
  dest = new MYODD_CHAR[ unExpandSize ];
  memset( dest, 0, unExpandSize );

  _tcscpy_s( dest, unExpandSize, stdUnDst.c_str() );

  // success the file was un-expanded.
  return true;
}

/**
 * Expand a string environment
 * that way the user can pass things like %appdata% in the API and we will expand them.
 * the calling function is responsible for cleaning the code.
 *
 * @param const MYODD_STRING& the path we are editing
 * @param MYODD_STRING& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest )
{
  MYODD_CHAR* expandEd;
  if( !files::ExpandEnvironment( src.c_str(), expandEd ))
  {
    return false;
  }

  if( expandEd )
  {
    dest = expandEd;
    delete [] expandEd;
  }
  else
  {
    dest = _T("");
  }

  return true;
}

/**
 * Expand a string environment
 * that way the user can pass things like %appdata% in the API and we will expand them.
 * the calling function is responsible for cleaning the code.
 *
 * @param const MYODD_CHAR* the path we are editing
 * @param MYODD_CHAR*& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment(const MYODD_CHAR* lpSrc, MYODD_CHAR*& dest )
{
  dest = nullptr;
  if(nullptr == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  // we might expand the value
  // so convert it to a string
  MYODD_STRING stdSrc = lpSrc;

  // look for the %apppath% in case we need to expand it.
  if( MYODD_STRING::npos != strings::Find( lpSrc, FILE_APPPATH, 0, false) )
  {
    //  we don't want the trailing back slash
    static MYODD_STRING appPath = GetAppPath( false );

    //  replace the string
    stdSrc = strings::Replace( stdSrc, FILE_APPPATH, appPath, false );
  }

  // Get the 'expanded' size of the argument.
  // We don't expand anything.
  unsigned long expandSize = (ExpandEnvironmentStrings( stdSrc.c_str(), 0, 0 ) + 1);
  if( 0 == expandSize )
  {
    // there was an error, use get last error.
    log::LogSystem( _T("ExpandEnvironment Error: %d"), ::GetLastError() );
    return false;
  }

  // Create the expanded item with the size we have.
  // and set it to 0 the terminating null should be included.
  dest = new MYODD_CHAR[ expandSize ];
  memset( dest, 0, expandSize );

  // now that we have all the needed information we can expand the string
  // but if there is an error we will return 0
  if( ExpandEnvironmentStrings( stdSrc.c_str(), dest, expandSize ) == 0 )
  {
    // there was an error, use get last error.
    log::LogSystem( _T("ExpandEnvironment Error2: %d"), ::GetLastError() );

    // simply return false as there was an error.
    // use GetLastError( ... )
    delete [] dest;
    dest = nullptr;
    return false;
  }

  // success the file was expanded.
  return true;
}

/**
* Delete a file, try and expand it first.
* @see ::DeleteFile( ... )
* @param const MYODD_STRING& the file we want to delete
* @return bool if the file was deleted or not.
*/
bool DeleteFile( const MYODD_STRING& c )
{
  return DeleteFile( c.c_str() );
}

/**
 * Delete a file, try and expand it first.
 * @see ::DeleteFile( ... )
 * @param const MYODD_CHAR* the file we want to delete
 * @return bool if the file was deleted or not.
 */
bool DeleteFile(const MYODD_CHAR* c )
{
  MYODD_CHAR* lpExpand = nullptr;
  if( !ExpandEnvironment( c, lpExpand ) )
  {
    //  could not even expand that file.
    // let alone delete it.
    return false;
  }

  // we can then try and delete that file
  BOOL bSuccess = ::DeleteFile( lpExpand );

  // remove the expanded file.
  delete [] lpExpand;

  return (bSuccess ? true : false );
}

/**
 * Copy a file to a new file.
 * @param const MYODD_STRING& the existing filename 
 * @param const MYODD_STRING& the name of the new file.
 * @param unsigned long* if not nullptr we will pass the last error code
 * @return bool success or not.
 */
bool CopyFile( const MYODD_STRING& lpExistingFileName, const MYODD_STRING& lpNewFileName, unsigned long* dwErr /*= 0*/ )
{
  return CopyFile( lpExistingFileName.c_str(), lpNewFileName.c_str(), dwErr );
}

/**
* Copy a file to a new file.
* @param const MYODD_STRING& the existing filename 
* @param const MYODD_STRING& the name of the new file.
* @param unsigned long* if not nullptr we will pass the last error code
* @return bool success or not.
*/
bool CopyFile(const MYODD_CHAR* lpExistingFileName, LPCTSTR lpNewFileName, unsigned long* dwErr /*= 0*/ )
{
  if( dwErr )
  {
    *dwErr = ERROR_SUCCESS;
  }

  MYODD_STRING stdExistingFileName( lpExistingFileName );
  if( !ExpandEnvironment( stdExistingFileName, stdExistingFileName ) )
  {
    //  cannot even expand it.
    if( dwErr )
    {
      *dwErr = ERROR_FILE_NOT_FOUND;
    }
    return false;
  }
  MYODD_STRING stdNewFileName( lpNewFileName );
  if( !ExpandEnvironment( stdNewFileName, stdNewFileName ) )
  {
    //  cannot even expand it.
    if( dwErr )
    {
      *dwErr = ERROR_FILE_NOT_FOUND;
    }
    return false;
  }

  // create the directory if need be.
  if( !CreateFullDirectory( stdNewFileName.c_str(), true ) )
  {
    if( dwErr )
    {
      *dwErr = ERROR_CANNOT_MAKE;
    }
    return false;
  }

  //  copy the file
  BOOL bSuccess = ::CopyFile( stdExistingFileName.c_str(), stdNewFileName.c_str(), false );

  if( !bSuccess && dwErr )
  {
    *dwErr = ::GetLastError();
  }

  //  success or not?
  return (bSuccess ? true : false );
}

/**
* check if a file is valid and exists on the hard drive or the network.
* @param const MYODD_STRING& the full path of the file we want to check for
* @return bool if the file exists or not.
*/
bool FileExists( const MYODD_STRING& c )
{
  return FileExists( c.c_str() );
}

/**
 * check if a file is valid and exists on the hard drive or the network.
 * @param const MYODD_CHAR* the full path of the file we want to check for
 * @return bool if the file exists or not.
 */
bool FileExists(const MYODD_CHAR* c )
{
  MYODD_CHAR* lpExpand = nullptr;
  if( !ExpandEnvironment( c, lpExpand ) )
  {
    // we could not even expand the file.
    // let alone tell if it exists.
    return false;
  }

  _tfinddata_t fdata;
  // look for the directory, if it exists then we don't need to go further.
  intptr_t ffhandle = _tfindfirst( lpExpand, &fdata );

  // remove the expanded file now
  delete [] lpExpand;

  if( ffhandle != -1 )
  {
    _findclose( ffhandle );
    return true;
  }

  //  not found
  return false;
}

/**
* check if a directory is valid and exists on the hard drive or the network.
* @param const MYODD_CHAR* the full path of the file we want to check for
* @return bool if the directory exists or not.
*/
bool DirectoryExists( const MYODD_STRING& c )
{
  return DirectoryExists( c.c_str() );
}

/**
 * check if a directory is valid and exists on the hard drive or the network.
 * @param const MYODD_CHAR* the full path of the file we want to check for
 * @return bool if the directory exists or not.
 */
bool DirectoryExists(const MYODD_CHAR* c )
{

  // look for that file
  // if it exists then there is nothing to do really.
  MYODD_STRING findFile = (c);
  AddTrailingBackSlash( findFile );
  findFile += _T("*.*"); 
  
  return FileExists( findFile.c_str() );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param const MYODD_STRING& the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory( const MYODD_STRING& c, bool bIsFile )
{
  return CreateFullDirectory( c.c_str(), bIsFile );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param const MYODD_CHAR* the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory(const MYODD_CHAR* lpPath, bool bIsFile )
{
  MYODD_CHAR* lpDest = nullptr;
  if( !files::ExpandEnvironment( lpPath, lpDest ))
  {
    return false;
  }

  if( !bIsFile )
  {
    if( DirectoryExists( lpDest ) )
    {
      delete [] lpDest;
      return true;
    }
  }
  else
  {
    if( FileExists( lpDest ) )
    {
      delete [] lpDest;
      return true;
    }
  }

  // Otherwise we need to check if it is a file or if it is a directory that we are creating.
  // first for that we need to get the length of that item.
  size_t l = lpPath ? _tcslen( lpPath ) : 0;
	if( l == 0 )
  {
    // the directory is size 0 so I cannot create it
    // I just cannot do anything with it.
    delete [] lpDest;
		return false;
  }

  MYODD_CHAR tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  _tcscpy_s( tmpDir, _countof( tmpDir ), lpPath );
  if( l >= 1 )
  {
    if( lpPath[l-1] == '\\' || lpPath[l-1] == '/' )
    {
      // we have a trialling '\' so we need to remove it.
      tmpDir[l-1] = '\0';
    }
  }

  // now go back and create the parent directory.
  int r1 = -1, r2 = -1;
  const MYODD_CHAR* p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != nullptr)
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  const MYODD_CHAR* p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != nullptr)
  {
    r2 = (int)(p2 - tmpDir + 1);
  }

  if( r1 == -1 && r2 == -1 )
  {
    // this looks like a relative path
    // so we just create the directory from where ever we are.
    return (CreateDirectory( tmpDir, nullptr)?true:false);
  }
  else if( r1 > r2 )
  {
    tmpDir[ r1 ] = '\0';
  }
  else if( r2 > r1 )
  {
    tmpDir[ r2 ] = '\0';
  }

  // re can now try and create the full directory.
  // we are no longer getting a file, whatever it is
  if( !CreateFullDirectory( tmpDir, false ) )
  {
    //  something did not work
    delete [] lpDest;
    return false;
  }

  // if we are a file then we only need to create the parents
  // if we are here then we created the parents
  // so all we need to do is return success.
  if( bIsFile )
  {
    delete [] lpDest;
    return true;
  }

  // if we are here it worked and we are a directory, (all the sub directories were created).
  // all we need to do is to create our directory
  bool bResult = (CreateDirectory( lpDest, nullptr)?true:false);
  delete [] lpDest;
  return bResult;
}

/**
 * Check if the string given is a valid existing file on the drive.
 * the string is expanded.
 *
 * @param const MYODD_STRING& stdFile the string we are checking for.
 * @return bool true|false if it is a file or not.
 */
bool IsFile(const MYODD_STRING& stdFile)
{
  return IsFile(stdFile.c_str());
}

/**
 * Check if the string given is a valid existing file on the drive.
 * the string is expanded.
 *
 * @param const MYODD_CHAR* the string we are checking for.
 * @return bool true|false if it is a file or not.
 */
bool IsFile(const MYODD_CHAR* lp )
{
  // expand the string.
  // the user could have passed environment variables.
  MYODD_CHAR* lpExpand = nullptr;
  if( !ExpandEnvironment( lp, lpExpand ) )
  {
    // something broke
    return false;
  }

  // if the value was empty we have nothing
  if(nullptr == lpExpand )
  {
    return false;
  }

  _tfinddata_t fdata;
  intptr_t ffhandle = _tfindfirst( lpExpand, &fdata );

  // we must delete the file now.
  delete [] lpExpand;

  // if the file was not found then it is not valid.
  // no need to go further.
  if( -1 == ffhandle )
  {
    return false;
  }

  // clear the handle
  _findclose( ffhandle );

  // if it is a directory then it is not a file...
  if( (fdata.attrib & _A_SUBDIR) == _A_SUBDIR )
  {
    return false;
  }

  return true;
}

/**
 * Check if the string given is a valid existing directory on the drive.
 * the string is expanded.
 *
 * @param const MYODD_CHAR* the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsDirectory( const MYODD_STRING& stdDir )
{
  return IsDirectory( stdDir.c_str() );
}

/**
 * Check if the string given is a valid existing directory on the drive.
 * the string is expanded.
 *
 * @param const MYODD_CHAR* the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsDirectory(const MYODD_CHAR* lp )
{
  // expand the string.
  // the user could have passed environment variables.
  MYODD_CHAR* lpExpand = nullptr;
  if( !ExpandEnvironment( lp, lpExpand ) )
  {
    // something broke
    // look inside the ExpandEnvironment( ... ) for details.
    return false;
  }

  // if the value was empty we have nothing
  if(nullptr == lpExpand )
  {
    return false;
  }

  _tfinddata_t fdata;
  intptr_t ffhandle = _tfindfirst( lpExpand, &fdata );
  if( -1 == ffhandle )
  {
    MYODD_STRING s = lpExpand;
    AddTrailingBackSlash( s );
    s += _T("*.*");

    // we can clear this now as we no longer need it.
    delete [] lpExpand;

    // if we find something we will not check 
    // the file attrib as know it is a directory.
    ffhandle = _tfindfirst( s.c_str(), &fdata );
    if( -1 == ffhandle )
    {
      return false;
    }

    // clear the handle
    // we found this item and it is a directory.
    _findclose( ffhandle );
    return true;
  }// find returned a -1

  // we no longer need this.
  delete [] lpExpand;

  // clear the handle
  _findclose( ffhandle );

  if( (fdata.attrib & _A_SUBDIR) != _A_SUBDIR )
  {
    return false;
  }

  return true;
}

/**
* Check if the string given LOOKS like a valid URL
* This is a very basic check, we cannot actually ping the site to enusre that it is valid.
*
* @param const MYODD_STRING& stdUrl the string we are checking for.
* @return bool true|false if it is a directory or not.
*/
bool IsURL(const MYODD_STRING& stdUrl)
{
  return IsURL(stdUrl.c_str());
}

/**
 * Check if the string given LOOKS like a valid URL
 * This is a very basic check, we cannot actually ping the site to enusre that it is valid.
 *
 * @param const MYODD_CHAR* the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsURL(const MYODD_CHAR* lp )
{
#ifdef _UNICODE
    boost::wsmatch matches;
    boost::wregex stringRegex;
    std::wstring url( lp );
#else
    boost::smatch matches;
    boost::regex stringRegex;
    std::string url( lp );
#endif

  try
  {
    //  https://regex101.com/
    // ^(([a-z|0-9]+):\/\/([\/a-z|0-9]+)([\/a-z|0-9.]+)(:[0-9]{2,})?(\/.+)?)+
    auto wild = _T("^(([a-z|0-9]+):\\/\\/([\/a-z|0-9]+)([\/a-z|0-9.]+)(:[0-9]{2,})?(.+)?)+");
    stringRegex.assign( wild, boost::regex_constants::icase);
    if (boost::regex_match(url, matches, stringRegex))
    {
      return true;
    }
  }
  catch( const std::runtime_error & e )
  {
    UNUSED_ALWAYS( e );
    // std::runtime_error if the complexity of matching the expression 
    // against an N character string begins to exceed O(N2), 
    // or if the program runs out of stack space
  }
  catch( ... )
  {
    // boost couldn't work it out.
  }
  return false;
}

/**
 * Check if the file information has changed, we pass the file name we are checking
 * as well as an old file information.
 * @see GetFileInformationByName( const MYODD_CHAR*, ... )
 * @param const MYODD_CHAR* the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged( const MYODD_STRING& file, const BY_HANDLE_FILE_INFORMATION& info )
{
  return HasFileInformationChanged( file.c_str(), info );
}

/**
 * Check if the file information has changed, we pass the file name we are checking
 * as well as an old file information.
 * @see GetFileInformationByName( const MYODD_CHAR*, ... )
 * @param const MYODD_CHAR* the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged(const MYODD_CHAR* file, const BY_HANDLE_FILE_INFORMATION& info )
{
  BY_HANDLE_FILE_INFORMATION info_now;
  if( !GetFileInformationByName( file, info_now ) )
  {
    // we cannot even get the (new?), details
    // so it must be different.
    return true;
  }

  if( memcmp( &info_now.ftCreationTime, &info.ftCreationTime, sizeof(FILETIME)) != 0 ||
    memcmp( &info_now.ftLastWriteTime, &info.ftLastWriteTime, sizeof(FILETIME)) != 0)
  {
    // the creation time or the last write time has changed.
    return true;
  }

  // the values seem to be the same
  // no real change has been noticed.
  return false;
}

/**
 * Get a file information given a file name
 * @see ::GetFileInformationByHandle ( ... )
 * @see GetFileInformationByName( const MYODD_CHAR*, ... )
 * @param const MYODD_CHAR* the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName( const MYODD_STRING& file, BY_HANDLE_FILE_INFORMATION& info )
{
  return GetFileInformationByName( file.c_str(), info );
}

/**
 * Get a file information given a file name
 * @see ::GetFileInformationByHandle ( ... )
 * @param const MYODD_CHAR* the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName(const MYODD_CHAR* file, BY_HANDLE_FILE_INFORMATION& info )
{
  // expand the file.
  MYODD_CHAR* lpDest;
  if( !files::ExpandEnvironment( file, lpDest ))
  {
    return false;
  }

  // get the original file info.
  HANDLE hSrcFile;
  if ((hSrcFile = CreateFile(lpDest,  
                             GENERIC_READ, 
                             FILE_SHARE_READ,
                             nullptr, 
                             OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE)
  {
    delete [] lpDest;
    return false;
  }

  // get the file info
  bool result = (GetFileInformationByHandle( hSrcFile, &info ) ? true : false);
  
  // cleanup
  delete [] lpDest;

  // close the handle
  CloseHandle( hSrcFile );

  // the result
  return result;
}

/**
 * Get the path of the current exe, could return an empty string if there is a problems.
 * @param bool add the trailing backslash or remove it.
 * @return MYODD_STRING the path of the current exe.
 */
MYODD_STRING GetAppPath( bool bAddtrailing /*=true*/)
{
  MYODD_STRING sReturn = _T("");
  auto lpBuffer = new MYODD_CHAR[ T_MAX_PATH ];
  if (GetModuleFileName( nullptr, lpBuffer, T_MAX_PATH ) != 0)
  {
    sReturn = GetBaseFromFile( lpBuffer, false, bAddtrailing );
  }
  else 
  {
    GetCurrentDirectory( T_MAX_PATH, lpBuffer);
    sReturn = lpBuffer;
  }
  delete [] lpBuffer;

  // make sure that the display is consistent.
  if( bAddtrailing )
  {
    AddTrailingBackSlash( sReturn );
  }
  else
  {
    RemoveTrailingBackSlash( sReturn );
  }
  return sReturn;
}

/**
 * Get the base path from the file path so "c:\somthing\whatever.txt" would return "c:\somthing\"
 * we don't check if the file exits as it might not be a real file.
 * @see GetBaseFromFile( ... )
 * @see GetFileName( ... ) for the file name
 * @param const MYODD_CHAR* the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @param bool add a trailling backslash or not.
 * @return MYODD_STRING the directory of the file given
 */
MYODD_STRING GetBaseFromFile
( 
  const MYODD_STRING& stdPath, 
  bool bExpand /*= true*/,
  bool bAddTrailling /*= true*/
)
{
  return GetBaseFromFile( stdPath.c_str(), bExpand, bAddTrailling );
}

/**
 * Get the base path from the file path so "c:\somthing\whatever.txt" would return "c:\somthing\"
 * we don't check if the file exits as it might not be a real file.
 * @see GetFileName( ... ) for the file name
 * @param const MYODD_CHAR* the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @return MYODD_STRING the directory of the file given
 */
MYODD_STRING GetBaseFromFile
( 
  const MYODD_CHAR* lpPath,
  bool bExpand /*= true*/, 
  bool bAddTrailling /*= true*/ 
)
{
  // expand the full path
  // this is in case the user uses some weird environment variable that contains the file name in it.
  // reverse look for the first '\' or '/'
  MYODD_CHAR tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  if( bExpand )
  {
    MYODD_CHAR* lpDest = nullptr;
    if( !files::ExpandEnvironment( lpPath, lpDest ))
    {
      return _T("");
    }
    _tcscpy_s( tmpDir, _countof( tmpDir ), lpDest );
    delete [] lpDest;
  }
  else
  {
    _tcscpy_s( tmpDir, _countof( tmpDir ), lpPath );
  }

  // now go back and create the parent directory.
  int r1 = -1, r2 = -1;
  const MYODD_CHAR* p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != nullptr)
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  const MYODD_CHAR* p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != nullptr)
  {
    r2 = (int)(p2 - tmpDir + 1);
  }

  if( r1 == -1 && r2 == -1 )
  {
    // nothing to do really
    // there are no backslash at all
    // so we have to assume that the entire string is a file
    //    GetBaseFromFile( "somefile.txt" ); 
    //    would return "\"
    // this is because this code assumes, (by definition), that a file was passed.
    tmpDir[0] = _T('\0');
  }
  else if( r1 > r2 )
  {
    tmpDir[ r1 ] = '\0';
  }
  else if( r2 > r1 )
  {
    tmpDir[ r2 ] = '\0';
  }

  MYODD_STRING s = ( tmpDir );
  if( bAddTrailling )
  {
    //  we have to add the trailing in case there was none to start with.
    AddTrailingBackSlash( s );
  }
  else
  {
    //  the trailing should be there already
    RemoveTrailingBackSlash( s );
  }
  return s;
}

/**
* Get the absolute path given a relative path.
* It is up to the user to delete the container value.
* @param MYODD_STRING& container for the return value, unset in case of error. 
* @param const MYODD_STRING& the relative path we would like to get the absolute path from.
* @param const MYODD_CHAR* the original path, if nullptr we will use the current directory as origin.
* @return bool if we were able to get the absolute path, false if the given path is unrealistic/impossible.
*/
bool GetAbsolutePath
( 
 MYODD_STRING& dest, 
 const MYODD_STRING& stdRelative, 
  const MYODD_CHAR* lpOrigin /*= nullptr*/
 )
{
  //  pass this to the other function.
  MYODD_CHAR* lpDest = nullptr;
  if( !files::GetAbsolutePath( lpDest, stdRelative.c_str(), lpOrigin ))
  {
    return false;
  }

  // copy the new value over.
  dest = lpDest;
  delete [] lpDest;

  return true;
}

/**
 * Get the absolute path given a relative path.
 * It is up to the user to delete the container value.
 * @param MYODD_CHAR*& container for the return value, unset in case of error. 
 * @param const MYODD_CHAR* the relative path we would like to get the absolute path from.
 * @param const MYODD_CHAR* the original path, if nullptr we will use the current directory as origin.
 * @return bool if we were able to get the absolute path, false if the given path is unrealistic/impossible.
 */
bool GetAbsolutePath
( 
  MYODD_CHAR*& dest,
  const MYODD_CHAR* lpRelative,
  const MYODD_CHAR* lpOrigin /*= nullptr*/
)
{
  dest = nullptr;
  if(nullptr == lpRelative )
  {
    //  we need a relative path to work with.
    return false;
  }

  MYODD_STRING sRelative( lpRelative );
  MYODD_STRING sOrigin( _T("") );
  if(nullptr == lpOrigin )
  {
    MYODD_CHAR lpBuffer[ T_MAX_PATH ] = {0};
    if( 0 == ::GetCurrentDirectory( T_MAX_PATH, lpBuffer ) )
    {
      //  could not get my own directory.
      return false;
    }
    sOrigin = lpBuffer;
  }
  else
  {
    sOrigin = lpOrigin;
  }

  // what we will be replacing everything with.
  static const MYODD_CHAR* SEPARATOR_REPLACE = _T("\\" );
  static MYODD_CHAR SEPARATOR_REPLACE_C = _T('\\' );
  static const MYODD_CHAR* SEPARATOR_REPLACE_FROM = _T("/");

  // now we need to replace all the '/' with '\' so we don't worry about UNC stuff.
  // don't use MYODD_FILE_SEPARATOR otherwise we might not replace anything at all.
  sOrigin = strings::Replace( sOrigin   , SEPARATOR_REPLACE_FROM, SEPARATOR_REPLACE );
  sRelative = strings::Replace( sRelative, SEPARATOR_REPLACE_FROM, SEPARATOR_REPLACE );

  //  will we need to add a trailing char?
  // remember we are not checking for MYODD_FILE_SEPARATOR as we replace all the "\"
  bool addtrailing = (sRelative[ sRelative.length() -1 ] == SEPARATOR_REPLACE_C );

  // split them all
  std::vector<MYODD_STRING> e_sRelative;
  strings::Explode( e_sRelative, sRelative, SEPARATOR_REPLACE_C, MYODD_MAX_INT32, false );

  std::vector<MYODD_STRING> e_sOrigin;
  strings::Explode( e_sOrigin, sOrigin, SEPARATOR_REPLACE_C, MYODD_MAX_INT32, false );
  // reverse from the origin path we only reverse the ../

  size_t dotdotCount = 0;
  size_t dotCount = 0;
  for( std::vector<MYODD_STRING>::const_iterator it = e_sRelative.begin();
       it != e_sRelative.end();
       ++it )
  {
    if( (*it).length() == 0 )
    {
      // maybe the user has passed a UNC path
      // either way MS does not really moan about it.
      continue;
    }
    else if( *it == _T("..") )
    {
      ++dotdotCount;
      continue;
    }
    else if( *it == _T(".") )  //  not pretty, but allowed.
    {
      ++dotCount;
      continue;
    }

    // we are done.
    // we do not want to go further into the path
    // if the user has something like ../../somepath1/somepath2/.../text.txt then we should convert it 
    // to ../../somepath1/text.txt
    break;
  }

  if( dotdotCount == 0 )
  {
    // we will not be using the original path
    // because the path that was given does not seem to be a relative path in the first place.
    e_sOrigin.erase( e_sOrigin.begin(), e_sOrigin.end() );

    // if this really look like it is not a path at all then we are relative to our path
    // relative = "example/something/"
    // origin = "c:/origin/path/"
    // return = "c:/origin/path/example/something/"
    if( sRelative.find( _T("%") ) !=  MYODD_STRING::npos )
    {
      MYODD_STRING expanded = _T("");
      if( ExpandEnvironment( sRelative, expanded ) )
      {
        if( GetAbsolutePath( dest, expanded.c_str(), sOrigin.c_str() ) )
        {
          MYODD_STRING absolute = dest;
          delete [] dest;
          dest = nullptr;

          return UnExpandEnvironment( absolute.c_str(), dest );
        }
      }
    }
    else if( sRelative.find( _T(":") ) ==  MYODD_STRING::npos )
    {
      // so what we will do is actually add the origin to the given path
      // and try and see if we can get that to work.
      AddTrailingBackSlash(sOrigin);
      MYODD_STRING correctedPath = sOrigin + sRelative;
      return GetAbsolutePath( dest, correctedPath.c_str(), sOrigin.c_str() );
    }
  }
  else if( dotdotCount >= e_sOrigin.size() )
  {
    return false; // cannot work it out from here sorry.
                  // we have more dots to reverse from than we have directories to walk.
  }

  // reverse from the origin path we only reverse the ../
  for( size_t pop_back = 0; pop_back < dotdotCount; ++pop_back )
    e_sOrigin.pop_back();

  // add the relative path at the end of it
  for( size_t push_back = (dotdotCount+dotCount); push_back < e_sRelative.size(); ++push_back )
    e_sOrigin.push_back( e_sRelative[push_back] );

  // finally clean everything up.
  std::vector<MYODD_STRING> e_sClean;
  for( std::vector<MYODD_STRING>::const_iterator it = e_sOrigin.begin();
       it != e_sOrigin.end();
       ++it
    )
  {
    if( *it == _T("..") )
    {
      //  we have to go back one step.
      // but do we have any steps to go back to?
      if( e_sClean.size() == 0 )
      {
        return false;
      }
      e_sClean.pop_back();
      continue;
    }
    else if( *it == _T(".") )  //  not pretty, but allowed.
    {
      continue;
    }
    e_sClean.push_back( *it );
  }

  // putting it all together...
  // note that we use MYODD_FILE_SEPARATOR and not SEPARATOR_REPLACE
  // this is in case both are not the same.
  MYODD_STRING imp = strings::implode( e_sClean, MYODD_FILE_SEPARATOR );
  if( addtrailing )
  {
    imp += MYODD_FILE_SEPARATOR;
  }
  size_t implodeSize = imp.length()+1;

  dest = new MYODD_CHAR[ implodeSize ];
  memset( dest, 0, implodeSize );

  _tcscpy_s( dest, implodeSize, imp.c_str() );

  return true;
}

/**
 * Remove illegal characters in a file name.
 * Do not pass a full path name as '/' will be replaced.
 * @param MYODD_STRING& the file been cleaned.
 * @return none
 */
void CleanFileName( MYODD_STRING& dirtyFileName )
{
  static MYODD_CHAR* badChars = _T("?[]/\\=+<>:;\",*|^\"");
  BOOST_FOREACH(MYODD_CHAR &tch, dirtyFileName )
  {
    BOOST_FOREACH( const MYODD_CHAR& bad, badChars )
    {
      if( tch == bad )
      {
        tch = '_';
        break;
      }
    }
  }
}

/**
 * Create a temp filename and return the FULL path
 * The file will be be unique
 * @param MYODD_STRING& the return value.
 * @param const MYODD_CHAR* the prefix or nullptr if no prefix is needed.
 * @param const MYODD_CHAR* the prefix or nullptr if no extension is needed, (if nullptr '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName( MYODD_STRING& stdFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt )
{
  // use the MYODD_STRING function.
  MYODD_CHAR* lpFileName = nullptr;
  if( !GetFullTempFileName( lpFileName, lpPrefix, lpExt ) )
  {
    return false;
  }
  // copy the value to the given string.
  stdFileName = lpFileName;

  // clean the temp buffer
  delete [] lpFileName;

  // success
  return true;
}

/**
 * Create a temp filename and return the FULL path
 * The file will be be unique
 * @param MYODD_CHAR*& the return value, the calling function will need to delete that value.
 * @param const MYODD_CHAR* the prefix or nullptr if no prefix is needed.
 * @param const MYODD_CHAR* the prefix or nullptr if no extension is needed, (if nullptr '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName(MYODD_CHAR*& lpFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt )
{
  // add a unique, random number.
  int unique_num = math::random_range( 0, 65535 );

  // create the return file
  MYODD_CHAR* lpTmpFile = new TCHAR[T_MAX_PATH];
  _stprintf_s( lpTmpFile, T_MAX_PATH, _T("%s%05d.%s"), (lpPrefix == nullptr ? _T("~myodd") : lpPrefix),
                                                        unique_num,
                                                       (lpExt == nullptr ? _T("tmp") : lpExt)
             );
  bool bResult = GetFullTempFileName( lpFileName, lpTmpFile ); 

  // clean the temp file.
  delete [] lpTmpFile;

  // this is, (suposedly), a random brand new file
  // so if it exists it is not ours.
  DeleteFile( lpFileName );

  // we are done 
  return bResult;
}

/**
 * Add a file name to the temp path
 * @param MYODD_STRING& the return buffer that will contain the file+full path.
 * @param const MYODD_CHAR* the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName( MYODD_STRING& stdFullPathFileName, const MYODD_CHAR* lpFileName )
{
  MYODD_CHAR* lpFullPathFileName = nullptr;
  if( !GetFullTempFileName( lpFullPathFileName, lpFileName ) )
  {
    return false;
  }

  // copy the given value.
  stdFullPathFileName = lpFullPathFileName;

  // clean the temp buffer
  delete [] lpFullPathFileName;

  // success
  return true;
}

/**
 * Add a file name to the temp path
 * @param MYODD_CHAR*& the return buffer that will contain the file+full path.
 * @param const MYODD_CHAR* the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName(MYODD_CHAR*& lpFullPathFileName, const MYODD_CHAR* lpFileName )
{
  // get the temp path
  MYODD_CHAR* lpTmpBuffer = new TCHAR[T_MAX_PATH];
  unsigned long dwLen = GetTempPath( T_MAX_PATH, lpTmpBuffer );
  if( 0 == dwLen )
  {
    // clean the temp path.
    delete [] lpTmpBuffer;

    //  could not create the temp path.
    return false;
  }

  // create the return buffer.
  lpFullPathFileName = new MYODD_CHAR[ T_MAX_PATH + 1];
  memset( lpFullPathFileName, 0, T_MAX_PATH );

  // join the path and the given filename
  _stprintf_s( lpFullPathFileName, T_MAX_PATH, _T("%s%s"), lpTmpBuffer, 
                                                           lpFileName
             );

  // clean the temp path.
  delete [] lpTmpBuffer;

  // it is not up to us to make sure that the file exists
  // or not, we do not want to delete it, in case the user needs it.
  return true;
}

/**
 * Check if a given string is a dot/dotdot directory
 * @param const MYODD_CHAR* the file we are checking.
 * @return bool if it is a directory or not
 */
bool IsDot( const MYODD_STRING& stdFile )
{
  return IsDot( stdFile.c_str() );
}

/**
 * Check if a given string is a dot/dotdot directory
 * @param const MYODD_CHAR* the file we are checking.
 * @return bool if it is a directory or not
 */
bool IsDot(const MYODD_CHAR* lpFile )
{
  // now we need to replace all the '/' with '\' so we don't worry about UNC stuff.
  MYODD_STRING sOrigin = lpFile;
  sOrigin = strings::Replace( sOrigin, _T("/"), _T("\\") );
  
  std::vector<MYODD_STRING> e_sOrigin;
  auto size = strings::Explode( e_sOrigin, sOrigin, _T('\\'), MYODD_MAX_INT32, false );
  if( size == 0 )
  {
    return false;
  }
  if( e_sOrigin[size-1] == _T("..") )
  {
    return true;
  }
  if( e_sOrigin[size-1] == _T(".") )
  {
    return true;
  }
  return false;
}

/**
* Get the filename from a full path, so "c:\somthing\whatever.txt" would return "whatever.txt"
* we don't check if the file exits as it might not be a real file.
* @see GetBaseFromFile( ... )
* @param const MYODD_CHAR* the full path we are getting.
* @param bool if we want to expand the string or not, used to prevent recursive calls.
* @return MYODD_STRING the filename of the file given without the directory
*/
MYODD_STRING GetFileName( const MYODD_STRING& stdPath, bool bExpand /*= true*/ )
{
  return GetFileName( stdPath.c_str(), bExpand );
}

/**
 * Get the filename from a full path, so "c:\somthing\whatever.txt" would return "whatever.txt"
 * we don't check if the file exits as it might not be a real file.
 * @see GetBaseFromFile( ... )
 * @param const MYODD_CHAR* the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @return MYODD_STRING the filename of the file given without the directory
 */
MYODD_STRING GetFileName(const MYODD_CHAR* lpPath, bool bExpand /*= true*/ )
{
  // expand the full path
  // this is in case the user uses some weird environment variable that contains the file name in it.
  // reverse look for the first '\' or '/'
  MYODD_CHAR tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  if( bExpand )
  {
    MYODD_CHAR* lpDest = nullptr;
    if( !files::ExpandEnvironment( lpPath, lpDest ))
    {
      return _T("");
    }
    _tcscpy_s( tmpDir, _countof( tmpDir ), lpDest );
    delete [] lpDest;
  }
  else
  {
    _tcscpy_s( tmpDir, _countof( tmpDir ), lpPath );
  }

  // now go back and create the parent directory.
  int r1 = -1, r2 = -1;
  const MYODD_CHAR* p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != nullptr)
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  const MYODD_CHAR* p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != nullptr)
  {
    r2 = (int)(p2 - tmpDir + 1);
  }

  if( r1 == -1 && r2 == r2 -1 )
  {
    return tmpDir;  // return was was passed.
  }

  MYODD_STRING s = _T("");
  if( r1 > r2 )
  {
    s = tmpDir + r1;
  }
  else if( r2 > r1 )
  {
    s = tmpDir + r2;
  }  
  return s;
}

/**
 * Get the file encoding
 * @param const MYODD_CHAR* the file we are checking.
 * @param int* if not nullptr we will set the number of characters to skip
 * @return FileEncode the file encoding.
 */
FileEncode DetermineEncoding(const MYODD_CHAR* file, int* npSkip )
{
  if( npSkip )
    *npSkip = 0;
  if(nullptr == file )
  {
    assert( 0 ); //  what are we trying to do?
    return uniError;
  }

  FILE* fp = 0;
  errno_t err = _tfopen_s( &fp, file, _T("rb") );
  if ( err != 0 || !fp )
  {
    assert( 0 );
    return uniError;
  }

  static const unsigned int blockSize = 5;
  BYTE b[ blockSize ];
  size_t nLen = fread( &b, sizeof(BYTE), blockSize, fp );
  
  FileEncode encoding = uni8Bit;
  int nSkip = 0;

  static const size_t MYFILE_UTF_8_LEN_ = 3;
  static const unsigned char MYFILE_UTF_8_[ MYFILE_UTF_8_LEN_ ] = {0xef, 0xbb, 0xbf};

  static const size_t MYFILE_UTF_16_BE_LEN_ = 2;
  static const unsigned char MYFILE_UTF_16_BE[ MYFILE_UTF_16_BE_LEN_ ] = {0xFE, 0xFF};

  static const size_t MYFILE_UTF_16_LE_LEN_ = 2;
  static const unsigned char MYFILE_UTF_16_LE[ MYFILE_UTF_16_LE_LEN_ ] = {0xFF, 0xFE};

  if( nLen >= MYFILE_UTF_16_BE_LEN_ && b[0] == MYFILE_UTF_16_BE[0] && b[1] == MYFILE_UTF_16_BE[1])
  {
    encoding = uni16BE;
    nSkip = 2;
  }
  else if( nLen >= MYFILE_UTF_16_LE_LEN_ && b[0] == MYFILE_UTF_16_LE[0] && b[1] == MYFILE_UTF_16_LE[1])
  {
    encoding = uni16LE;
    nSkip = 2;
  }
  else if( nLen >= MYFILE_UTF_8_LEN_ && b[0] == MYFILE_UTF_8_[0] && b[1] == MYFILE_UTF_8_[1] && b[2] == MYFILE_UTF_8_[2])
  {
    encoding = uniUTF8;
    nSkip = 3;
  }
  else if (nLen >= 2 && b[0] != NULL && b[1] == NULL && IsTextUnicode(b, static_cast<int>(nLen), nullptr))
  {
    encoding = uni16LE_NoBOM;
    nSkip = 0;
  }
  else if (nLen >= 2 && IsTextUnicode(b, (int)nLen, nullptr))
  {
    encoding = uniUTF8NoBOM;
    nSkip = 0;
  }

  // close the file
  fclose( fp );

  if( npSkip )
    *npSkip = nSkip;

  return encoding;
}

/**
 * Read a file regardless of the the encoding.
 * if the nStart or nEnd positions are -1 then we will calculate it, (end or start of file.
 * @param const MYODD_CHAR* the file we want to read.
 * @param __int64 the start position we want to read from
 * @param __int64 the end position we want to read to.
 * @return MYODD_CHAR* the data or nullptr if there was an error.
 */
MYODD_CHAR* ReadFile(const MYODD_CHAR* file, __int64 nStartPos, __int64 nEndPos )
{
  // sanity check
  if(nullptr == file )
  {
    return nullptr;
  }

  // get the file encoding.
  int nSkip = 0;
  FileEncode encoding = DetermineEncoding( file, &nSkip );

  FILE* fp = 0;
  errno_t err = _tfopen_s( &fp, file, _T("rb" ));
  if ( err || nullptr == fp )
  {
    // could not open the file.
    return nullptr;
  }

  //
  // get the beginning of the file if not given to us.
  if( nStartPos < 0 )
  {
    nStartPos = 0;
  }

  __int64 length = 0;
  // get the end position of the file if not given to us
  if( nEndPos < 0 )
  {
    // obtain file size.
    _fseeki64( fp, 0, SEEK_END );
    length = _ftelli64( fp );
    nEndPos = length;             // not used.
    length -= nSkip;
  }
  else
  {
    // get the length of the file.
    // we don't subtract the nSkip as the user did not include it in the values given.
    length = (nEndPos - nStartPos);
  }

  // we start from the start+skip position.
  _fseeki64( fp, nStartPos+nSkip, SEEK_SET );

  // create the return buffer.
  MYODD_CHAR* tbuf_ = nullptr;
  try
  {
    // @Todo this could be a problem here when length is longer than (size_t)-1
    // this will almost certainly never happen as the amount of memory needed would
    // probably exceed the amount of memory available.
    char* buf = new char[ (size_t)length+1 ]; 
    if(nullptr != buf )
    {
      // @Todo this could be a problem here when length is longer than (size_t)-1
      // this will almost certainly never happen as the amount of memory needed would
      // probably exceed the amount of memory available.
      ASSERT( length+1 < (size_t)-1 );
      memset( buf, 0, (size_t)length+1 );
      length = fread( buf, sizeof(char), (size_t)length, fp );

      tbuf_ = Byte2Char( buf, (size_t)length, encoding );

      // we don't need the char* anymore.
      delete [] buf;
    }// could we allocate memory?
  }
  catch( ... )
  {
    // are we out of memory?
    tbuf_ = nullptr;
  }
  // we are done here
  fclose( fp );
  return tbuf_;
}

void Join( MYODD_STRING& returnPath, const MYODD_STRING& pathPartA, const MYODD_STRING& pathPartB )
{
  MYODD_STRING partA = pathPartA;
  MYODD_STRING partB = pathPartB;

  // make sure we have a trailing backslash
  AddTrailingBackSlash( partA );

  // and make sure that we don't have a leading one.
  RemoveLeadingBackSlash( partB );
  
  // join the two together.
  returnPath = partA + partB;
}

// --------------------------------------------------------------------------------------------------
Version::Version(const MYODD_CHAR* lpFileName /*= nullptr*/ )
{
  if( lpFileName )
  {
    DetermineFileVersion( lpFileName );
  }
  else
  {
    MYODD_CHAR	strTempBuffer[ MAX_PATH ];
    // Retrieve the fully-qualified path from the current process
    if ( NULL != ::GetModuleFileName(nullptr,	// Current module
                                      strTempBuffer,
                                      MAX_PATH ) )
    {
      DetermineFileVersion( strTempBuffer );
    }
  }
}

Version::~Version()
{
}

/**
 * Get the current file version.
 * @param const MYODD_CHAR* the filename of the file whose version we want.
 * @return none
 */
void Version::DetermineFileVersion(const MYODD_CHAR* lpFileName )
{
  ASSERT( lpFileName );
  if( !lpFileName )
  {
    return;
  }

  unsigned long	dwHandle = 0;

  // Determines whether the operating system can retrieve version information
  // for a specified file.
  unsigned long	dwFileVersionInfoSize = GetFileVersionInfoSize( lpFileName, &dwHandle );

  if (NULL == dwFileVersionInfoSize )
  {
    return;	// Can't retrieve version information size.
  }

  // Allocate space to retrieve version information using vector to prevent
  // memory leaks
  std::vector<BYTE>	pData( dwFileVersionInfoSize );

  // Retrieves version information for the specified file.
  if ( false == GetFileVersionInfo( lpFileName,
                                    dwHandle,
                                    dwFileVersionInfoSize,
                                    static_cast<void*>( &pData[0] ) ) )
  {
    return; // Can't retrieve version information.
  }

  // The memory of ptFileInfo is freed when pData is freed.
  VS_FIXEDFILEINFO *ptFileInfo;
  UINT	uintSize;

  // Retrieves version information from the version-information resource
  if ( false == VerQueryValue( static_cast<void*>( &pData[0] ),
                               _T("\\"),
                               reinterpret_cast<void**> ( &ptFileInfo ),
                               &uintSize ) )
  {
    return; // Can't retrieve version information
  }

  // Resolve major, minor, release and build number.
  m_fileVersionMajor		= static_cast<unsigned short>(  
    ( ptFileInfo->dwFileVersionMS >> 16 ) &0xffff );

  m_fileVersionMinor		= static_cast<unsigned short>( 
    ptFileInfo->dwFileVersionMS &0xffff );

  m_fileVersionMaintenance	= static_cast<unsigned short>( 
    ( ptFileInfo->dwFileVersionLS >> 16 ) &0xffff);

  m_fileVersionBuild		= static_cast<unsigned short>( 
    ptFileInfo->dwFileVersionLS & 0xffff );

  // get the last time we wrote to the file.
  FILETIME fileTime = {0};
  HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ,
    nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if( hFile ) 
  {
    if( 0 != GetFileTime( hFile, nullptr, nullptr, &fileTime ) )
    {
      FileTimeToSystemTime(&fileTime, &m_fileSystemFime); 
    }
    CloseHandle(hFile); 
  }
}

/**
 * Get all the app keys from a .cfg filename.
 * @param const MYODD_CHAR* the name of the file
 * @param std::vector<MYODD_STRING>& the various keys.
 * @param const MYODD_CHAR* the app name, nullptr if we want all the keys.
 * @param const MYODD_CHAR* | nullptr the wild search to limit certain keys only or nullptr to get all the keys.
 * @return std::vector<MYODD_STRING> list of all the keys or some that match the seatch.
 */
size_t GetKeys
( 
  const MYODD_CHAR* lpFileName,
  std::vector<MYODD_STRING>& tokens,
  const MYODD_CHAR* lpAppName,
  const MYODD_CHAR* lpWild /*=nullptr*/
)
{
  tokens.clear();

  //  get all the keys
  unsigned long keys_size = T_MAX_PATH;
  while( true )
  {
    MYODD_CHAR* keys = new TCHAR[ keys_size ];
    auto nLen = ::GetPrivateProfileString( lpAppName, nullptr, _T(""), keys, keys_size, lpFileName );
    if( (keys_size-2) == nLen )
    {
      keys_size+=T_MAX_PATH;
      delete [] keys;
    }
    else
    {
      //  get the tokens.
      std::vector<MYODD_STRING> tokensTmp;
      strings::explode_by_null_char( tokensTmp, keys, nLen );
      for( std::vector<MYODD_STRING>::const_iterator it = tokensTmp.begin(); 
           it != tokensTmp.end();
           ++it
         )
      {
        if( lpWild )
        {
          // limit the search
          if( !strings::wildcmp( lpWild, (*it).c_str() ))
          {
            continue;
          }
        }

        // add this to our list
        tokens.push_back( *it );
      }

      // clean-up
      delete [] keys;
      break;
    }
  }
  return tokens.size();
}

// Utf8_16.cxx
// Copyright (C) 2002 Scott Kirkwood
//
// Permission to use, copy, modify, distribute and sell this code
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies or
// any derived copies.  Scott Kirkwood makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////
// 
// Modificated 2006 Jens Lorenz
// 
// - Clean up the sources
// - Removing UCS-Bug in Utf8_Iter
// - Add convert function in Utf8_16_Write
////////////////////////////////////////////////////////////////////////////////

class Utf8_16 {
public:
  typedef unsigned short utf16; // 16 bits
  typedef UCHAR utf8; // 8 bits
  typedef UCHAR ubyte;
  static const utf8 k_Boms[uniEnd][3];
};

// Reads UTF-16 and outputs UTF-8
class Utf16_Iter : public Utf8_16 {
public:
  enum eState {
    eStart,
    e2Bytes2,
    e3Bytes2,
    e3Bytes3
  };

  Utf16_Iter();
  void reset();
  void set(const ubyte* pBuf, size_t nLen, FileEncode eEncoding);
  utf8 get() const { return m_nCur; };
  void operator++();
  eState getState() const { return m_eState; };
  operator bool() const { return m_pRead <= m_pEnd; };

protected:
  void toStart(); // Put to start state, swap bytes if necessary

protected:
  FileEncode m_eEncoding;
  eState m_eState;
  utf8 m_nCur;
  utf16 m_nCur16;
  const ubyte* m_pBuf;
  const ubyte* m_pRead;
  const ubyte* m_pEnd;
};

//==================================================
Utf16_Iter::Utf16_Iter()
{
  reset();
}

void Utf16_Iter::reset()
{
  m_pBuf = nullptr;
  m_pRead = nullptr;
  m_pEnd = nullptr;
  m_eState = eStart;
  m_nCur = 0;
  m_nCur16 = 0;
  m_eEncoding = uni8Bit;
}

void Utf16_Iter::set(const ubyte* pBuf, size_t nLen, FileEncode eEncoding)
{
  m_pBuf = pBuf;
  m_pRead = pBuf;
  m_pEnd = pBuf + nLen;
  m_eEncoding = eEncoding;
  m_eState = eStart;
  operator++();
  // Note: m_eState, m_nCur, m_nCur16 not reinitalized.
}

// Goes to the next byte.
// Not the next symbol which you might expect.
// This way we can continue from a partial buffer that doesn't align
void Utf16_Iter::operator++()
{
  switch (m_eState)
  {
  case eStart:
    if (m_eEncoding == uni16LE || m_eEncoding == uni16LE_NoBOM) 
    {
      m_nCur16 = *m_pRead++;
      m_nCur16 |= static_cast<utf16>(*m_pRead << 8);
    }
    else //(m_eEncoding == uni16BE || m_eEncoding == uni16BE_NoBOM)
    {
      m_nCur16 = static_cast<utf16>(*m_pRead++ << 8);
      m_nCur16 |= *m_pRead;
    }
    ++m_pRead;

    if (m_nCur16 < 0x80) {
      m_nCur = static_cast<ubyte>(m_nCur16 & 0xFF);
      m_eState = eStart;
    } else if (m_nCur16 < 0x800) {
      m_nCur = static_cast<ubyte>(0xC0 | m_nCur16 >> 6);
      m_eState = e2Bytes2;
    } else {
      m_nCur = static_cast<ubyte>(0xE0 | m_nCur16 >> 12);
      m_eState = e3Bytes2;
    }
    break;
  case e2Bytes2:
  case e3Bytes3:
    m_nCur = static_cast<ubyte>(0x80 | m_nCur16 & 0x3F);
    m_eState = eStart;
    break;
  case e3Bytes2:
    m_nCur = static_cast<ubyte>(0x80 | ((m_nCur16 >> 6) & 0x3F));
    m_eState = e3Bytes3;
    break;
  }
}

/**
 * Convert a chat pointer to 
 * @param const char the data we are trying to decode.
 * @param __int64 the number of chars.
 * @param FileEncode the file encoding we are converting from.
 * @return MYODD_CHAR* the converted file.
 */
MYODD_CHAR* Byte2Char
( 
  const char* buf, 
  size_t len, 
  FileEncode fileEncoding 
)
{
#ifndef _UNICODE
  char* tbuf = new char( len +1);
  memset( tbuf, 0, len );
  memcpy( tbuf, buf, len );
#else
  std::wstring convertedString = _T("");
  switch( fileEncoding )
  {
  case uni8Bit:
    convertedString = strings::String2WString( buf );
    break;

  case uni16BE: 
  case uni16LE:
  case uni16LE_NoBOM:
    {
      // get the max new size
      auto newSize = len + len / 2 + 1;
      auto cbuf  = new char[newSize];
      memset( cbuf, 0, newSize );

      BYTE nSkip = 2;
      switch( fileEncoding )
      {
      case uni16BE: 
      case uni16LE:
        nSkip = 2;
        break;

      case uni16LE_NoBOM:
        nSkip = 0;
        break;
      }

      // 'read' the buffer to the best of our ability
      Utf8_16::ubyte* pCur = (Utf8_16::ubyte*)cbuf;
      Utf16_Iter _Iter16;
      _Iter16.set( (Utf8_16::ubyte*)buf + nSkip, len - nSkip, fileEncoding );
      for (; _Iter16; ++_Iter16)
      {
        *pCur++ = _Iter16.get();
      }
      
      // actual size
      // size_t actual = pCur - (Utf8_16::ubyte*)cbuf;

      // convert to MYODD_CHAR
      convertedString = strings::String2WString( cbuf );

      // clean up
      delete [] cbuf;
      cbuf = 0;
    }
    break;

  default:
    convertedString = strings::String2WString( buf );
    break;
  }

  auto l = convertedString.length();
  auto tbuf = new wchar_t[ l + sizeof(wchar_t)];
  lstrcpy(tbuf, convertedString.c_str());

  return tbuf;
#endif
}

/** 
 * Get the size of a file
 * We return -1 if the file does not exist, we do not have permission and/or there was some other error.
 * @param const MYODD_STRING& stdFullPathFileName the file we are checking.
 * @return long Total size, in bytes
 */
long GetFileSizeInBytes( const MYODD_STRING& stdFullPathFileName )
{
  auto cleannedFileName = stdFullPathFileName;
  if (!ExpandEnvironment(cleannedFileName, cleannedFileName))
  {
    return -1;
  }

  // http://linux.die.net/man/2/stat
  // https://msdn.microsoft.com/en-us/library/14h5k7ff(VS.71).aspx
  long fileSize = -1;
#ifdef UNICODE
  struct _stat64i32 filestatus;
  if (-1 == _wstat(cleannedFileName.c_str(), &filestatus))
  {
    return fileSize;
  }
  fileSize = static_cast<long>(filestatus.st_size);
#else
  struct stat filestatus;
  if (-1 == stat(cleannedFileName.c_str(), &filestatus))
  {
    return fileSize;
  }
  fileSize = static_cast<long>filestatus.st_size;
#endif // UNICODE

  return static_cast<long>(filestatus.st_size);
}

} //  files
} //  myodd