#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "../files/files.h"
#include "../string/string.h"
#include "../math/math.h"
#include "../log/log.h"
#include <io.h>

// Look for version.lib
#pragma comment( lib, "version.lib" )

static LPCTSTR FILE_APPPATH = _T("%apppath%");

/**
 * TODO : Maybe move to myodd::strings
 * TODO : Why are we getting dead chars in the first place? 
 * This came from casting in the Clipboard, so maybe data is not copied properly.
 *
 * @param STD_TSTRING& the string we want to remove dead code from
 * @return none 
 */
inline void _trimDeadChars( STD_TSTRING& s )
{
  size_t l = s.length();
  while( l>1 && s[l-1] == 0 )
  {
    s = s.substr( 0, l-1 );
    --l;
  }
}

// this is the separator that we will be converting everything to if we are on an MS System.
static LPCTSTR MYODD_FILE_WINSEPARATOR = _T("\\");

// the default separator
static LPCTSTR MYODD_FILE_SEPARATOR   = MYODD_FILE_WINSEPARATOR;
static const TCHAR MYODD_FILE_SEPARATOR_C = MYODD_FILE_SEPARATOR[0];

namespace myodd{ namespace files{
void Test()
#ifdef _DEBUG
{
  STD_TSTRING sPath = _T("\\Hello");
  RemoveLeadingBackSlash( sPath );
  ASSERT( sPath == _T("Hello") );

  sPath = _T("\\Hello\\");
  RemoveLeadingBackSlash( sPath );
  ASSERT( sPath == _T("Hello\\") );

  sPath = _T("\\\\Hello");
  RemoveLeadingBackSlash( sPath );
  ASSERT( sPath == _T("Hello") );

  sPath = _T("");   //  empty string
  RemoveLeadingBackSlash( sPath );
  ASSERT( sPath == _T("") );

  sPath = _T("");   //  empty string!
  RemoveTrailingBackSlash( sPath );
  ASSERT( sPath == _T("") );

  sPath = _T("\\");   //  1 char
  RemoveLeadingBackSlash( sPath );
  ASSERT( sPath == _T("") );

  sPath = _T("\\");   //  1 char
  RemoveTrailingBackSlash( sPath );
  ASSERT( sPath == _T("") );

  sPath = _T("\\Hello\\");
  RemoveTrailingBackSlash( sPath );
  ASSERT( sPath == _T("\\Hello") );

  sPath = _T("Hello\\\\");
  RemoveTrailingBackSlash( sPath );
  ASSERT( sPath == _T("Hello") );

  TCHAR* directory = new TCHAR[T_MAX_PATH+1];
  memset( directory, 0, T_MAX_PATH+1);

  //  copy the directory
  _tcscpy_s( directory, T_MAX_PATH+1, _T("Hello") );  
  myodd::files::AddTrailingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello\\")) == 0 );

  memset( directory, 0, T_MAX_PATH+1);
  _tcscpy_s( directory, T_MAX_PATH+1, _T("Hello\\") );  
  myodd::files::AddTrailingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello\\")) ==0 );

  //  copy the directory
  memset( directory, 0, T_MAX_PATH+1);
  _tcscpy_s( directory, T_MAX_PATH+1, _T("Hello\\") );  
  myodd::files::RemoveTrailingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello")) == 0 );

  //  copy the directory
  memset( directory, 0, T_MAX_PATH+1);
  _tcscpy_s( directory, T_MAX_PATH+1, _T("Hello") );  
  myodd::files::RemoveTrailingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello")) == 0 );

  //  copy the directory
  memset( directory, 0, T_MAX_PATH+1);
  _tcscpy_s( directory, T_MAX_PATH+1, _T("Hello") );  
  myodd::files::RemoveLeadingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello")) == 0 );

  //  copy the directory
  memset( directory, 0, T_MAX_PATH+1);
  _tcscpy_s( directory, T_MAX_PATH+1, _T("\\Hello") );  
  myodd::files::RemoveLeadingBackSlash( directory, T_MAX_PATH );
  ASSERT( _tcscmp(directory, _T("Hello")) == 0 );

  delete [] directory;


  ASSERT( GetBaseFromFile( _T("c:\\a\\b\\something.txt"), false ) == _T("c:\\a\\b\\") );
  ASSERT( GetBaseFromFile( _T("c:\\a\\b\\something.txt"), false, false ) == _T("c:\\a\\b") );
  ASSERT( GetBaseFromFile( _T("C:\\test.txt" ) ) == _T("C:\\"));
  ASSERT( GetBaseFromFile( _T("test.txt" ) ) == MYODD_FILE_SEPARATOR );
  ASSERT( GetBaseFromFile( _T("C:\\dir/test.txt" ) ) == _T("C:\\dir\\"));
  ASSERT( GetBaseFromFile( _T("C:\\" ) ) == _T("C:\\"));

  ASSERT( get_extension( _T("somefile.txt") ) == _T("txt") );
  ASSERT( get_extension( _T("somefile.txt") ) != _T("ftp") );

  //  sanity checks
  ASSERT( !is_extension( _T("somefile.txt"), _T("")) );
  ASSERT( !is_extension( _T(""), _T("")) );
  ASSERT( !is_extension( _T(""), _T("text")) );
  ASSERT( !is_extension( _T("somefile.txt"), _T(".")) );

  ASSERT( is_extension( _T("somefile.txt"), _T("txt")) );
  ASSERT( is_extension( _T("somefile.txt"), _T(".txt")) );
  ASSERT( is_extension( _T("somefile.txt"), _T("tXt")) );
  ASSERT( is_extension( _T("somefile.TXT"), _T("txt")) );
  ASSERT( !is_extension( _T("somefile.TXT"), _T("ftp")) );
  ASSERT( !is_extension( _T("somefile.TXT"), _T(".ftp")) );

  // special cases.
  ASSERT( is_extension( _T("somefile.txt1.text2"), _T(".txt1\\.text2")) );  // the '.' is escaped.
  ASSERT( is_extension( _T("somefile.txt1.text2"), _T(".txt1.text2")) );  // the '.' is not escaped.
  ASSERT( is_extension( _T("somefile.txt1.text2"), _T("txt1.text2")) );
  
  LPTSTR lpdest = NULL;
  ASSERT( GetAbsolutePath( lpdest, _T( "../../somefile.txt" ), _T( "c:\\dira\\dirb\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T("c:\\somefile.txt") ) == 0 );
  delete [] lpdest;
  lpdest = NULL;

  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\.\\Application Data\\..\\..\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = NULL;
  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\..\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\All Users\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = NULL;
  ASSERT( GetAbsolutePath( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\..\\Application Data\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\Documents and Settings\\All Users\\Application Data\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = NULL;

  ASSERT( GetAbsolutePath( lpdest, _T("somepath2\\"), _T( "C:\\somepath1\\" ) ) );
  ASSERT( _tcsicmp( lpdest, _T( "C:\\somepath1\\somepath2\\" ) ) == 0 );
  delete [] lpdest;
  lpdest = NULL;


  // this should not work because of the depth of the path
  ASSERT( !GetAbsolutePath( lpdest, _T( "../../../somefile.txt" ), _T( "c:\\dira\\dirb\\" ) ) );

  STD_TSTRING appPath = GetAppPath( false );
  ASSERT( ExpandEnvironment( FILE_APPPATH, lpdest ) );
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  ASSERT( ExpandEnvironment( _T("%AppPath%"), lpdest ) );   //  case insensitive
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = NULL;
  ASSERT( ExpandEnvironment( _T("%AppPath%\\somedir\\somefile.txt"), lpdest ) );   //  case insensitive
  STD_TSTRING s = appPath + _T("\\somedir\\somefile.txt" );
  ASSERT( _tcsicmp( lpdest, s.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = NULL;

  ASSERT( UnExpandEnvironment( _T("%systemdrive%"), lpdest ) );
  delete [] lpdest;

  ASSERT( UnExpandEnvironment( appPath.c_str(), lpdest ) );
  ASSERT( _tcsicmp( lpdest, FILE_APPPATH ) == 0 );
  delete [] lpdest;
  lpdest = NULL;

  STD_TSTRING dirtyFileName = _T("[bad]^*£");
  CleanFileName( dirtyFileName );
  ASSERT( dirtyFileName == _T("_bad___£"));

  ASSERT( is_dot( _T("." )) );
  ASSERT( is_dot( _T(".." )) );
  ASSERT( is_dot( _T("c:\\example\\.." )) );
  ASSERT( is_dot( _T("c://example\\.." )) );
  ASSERT( !is_dot( _T("c://example\\..\\root\\" )) );

  ASSERT( GetFileName( _T("c:\\a\\b\\something.txt"), false ) == _T("something.txt") );

  ASSERT( IsURL( _T("http:\\www.google.com")) );
  ASSERT( IsURL( _T("ftp:\\www.google.com")) );
  ASSERT( IsURL( _T("https:\\www.google.com")) );
  ASSERT( IsURL( _T("ftps:\\www.google.com")) );

  STD_TSTRING f = _T("somefile");
  add_extension( f, _T("txt"), false );  // strip extension false.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile");
  add_extension( f, _T("txt"), true );    // strip extension true.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile.old");
  add_extension( f, _T("txt"), true );    // strip extension true.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile.");                    // with just a dot at the end
  add_extension( f, _T("txt"), true );    // strip extension true.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile.old");                    // with just a dot at the end 
  add_extension( f, _T(".txt"), true );   // strip extension true.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile.");                    // with just a dot at the end 
  add_extension( f, _T(".txt"), true );   // strip extension true.
  ASSERT( f == _T("somefile.txt") );

  f = _T("somefile.");                     // with just a dot at the end 
  add_extension( f, _T(".txt"), false );   // strip extension _false_.
  ASSERT( f == _T("somefile.txt") );
}
#else
{
  __noop;
}
#endif // _DEBUG

/**
 * Add an extension to a given string/filename
 * The extension does not need to have a '.', a check is already made.
 * @param const STD_TSTRING& the 'file' or string we want to add the extension to.
 * @param const STD_TSTRING& the extension we want to add to the string.
 * @param bool strip the current extension if there is one.
 * @return none
 */
void add_extension
( 
  STD_TSTRING& f, 
  const STD_TSTRING& e, 
  bool strip_current_if_exists  
)
{
  _trimDeadChars( f );
  if( f.length() == 0 || e.length() == 0 )
  {
    // nothing to do...
    return;
  }

  if( true == strip_current_if_exists )
  {
    strip_extension( f ); //  remove the current extension.
  }

  if( *f.rbegin() == _T('.') )
  {
    f = f.substr( 0, f.length() -1 );
  }

  // do we have a dot in the extension?
  // we already know the size is not 0
  if( *e.begin() == _T('.') )
  {
    f += e;
  }
  else
  {
    f += _T('.') + e;
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
 * @param STD_TSTRING& the string we want to remove the extension from.
 * @return none 
 */
void strip_extension( STD_TSTRING& f )
{
  _trimDeadChars( f );

  size_t result;
  LPCTSTR pdest = _tcsrchr( f.c_str(), '.' );

  if( pdest != NULL )
  {
    result = (pdest - (f.c_str()));
    if( result < f.length() )
    {
      f = f.substr( 0, result );
    }
  }
}

/**
 * get the file extension, if any. Return an empty string if there isn't one.
 * @param STD_TSTRING the file name
 * @return STD_TSTRING the extension, (or empty string if there are none).
 */
STD_TSTRING get_extension( const STD_TSTRING& fOriginal )
{
  STD_TSTRING f( fOriginal );
  _trimDeadChars( f );

  LPCTSTR pdest = _tcsrchr( f.c_str(), '.' );
  return (pdest ? STD_TSTRING(pdest+1) : _T(""));
}

/**
 * Check if a files extension is the one given
 * @param STD_TSTRING the file name
 * @param STD_TSTRING the extension we are checking for.
 * @return bool if the file extension is the one we wanted.
 */
bool is_extension( const STD_TSTRING& fOriginal, const STD_TSTRING& fExt )
{
  STD_TSTRING f( fOriginal );
  _trimDeadChars( f );

  STD_TSTRING e( fExt );
  _trimDeadChars( e );
  if( e.length() > 1 && e[0] == '.' )
  {
    e = e.substr( 1 );
  }

  if( f.length() == 0 || e.length() == 0 )
  {
    return false; //  we cannot check 0 lengths
  }

  e = myodd::strings::replace( e, _T("\\."), _T("." ));     //  in case the user escaped it already
  e = myodd::strings::replace( e, _T("."), _T("\\." ));     //  escape it now.
  STD_TSTRING stdMatches = _T("^(.*)\\.(") + e + _T(")$");
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
 * @param LPTSTR the path we want to remove the backslash from
 * @param unsigned int the max number of characters.
 * @return none
 */
void RemoveLeadingBackSlash
(
 LPTSTR szPath,	      // pointer to buffer to receive module path
 unsigned int nSize		// size of buffer, in characters
 )
{
  if( NULL == szPath )
    return;

  STD_TSTRING s( szPath, _tcslen(szPath) );

  // the the STD_TSTRING version
  RemoveLeadingBackSlash( s );

  // re-copy it across
  // strncpy( szPath, pstrRoot, nSize );
  memset( szPath, 0, nSize );
  _tcsncpy_s( szPath, nSize, s.c_str() , nSize );
}

/**
 * Remove a leading backslash at the start of a path
 * This is normally used so we can joining 2 paths together.
 * @param STD_TSTRING the path we want to remove the backslash from
 * @return none
 */
void RemoveLeadingBackSlash( STD_TSTRING& szPath)
{
  _trimDeadChars( szPath );

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
 * As we are _removing_ a backslash the variable nSize might seem redundant, but it ensures that
 * the user can pass values with no null ending char.
 * @param LPTSTR the file that has a trailing back slash.
 * @param unsigned int the maximum size of the buffer to prevent overruns.
 * @return none 
 */
void RemoveTrailingBackSlash
(
  LPTSTR szPath,	// pointer to buffer to receive module path
  unsigned int nSize		  // size of buffer, in characters
)
{
  if( NULL == szPath )
    return;

  STD_TSTRING s( szPath, _tcslen(szPath) );

  // the the STD_TSTRING version
  RemoveTrailingBackSlash( s );

  // re-copy it across
  // strncpy( szPath, pstrRoot, nSize );
  memset( szPath, 0, nSize );
  _tcsncpy_s( szPath, nSize, s.c_str() , nSize );
}

/**
 * Look for a trailing back slash at the end of the path and strip it.
 * @param STD_TSTRING& the string we want to remove the trailing back slash
 * @return none 
 */
void RemoveTrailingBackSlash
(	
 STD_TSTRING& szPath
)
{
  _trimDeadChars( szPath );
  
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
 * @see AddTrailingBackSlash( STD_TSTRING& s )
 *
 * @param LPTSTR the path we are editing
 * @param DWORD the max size of the file.
 * @return none 
 */
void AddTrailingBackSlash
(
  LPTSTR szPath,	    // pointer to buffer to receive module path
  unsigned int nSize	// size of buffer, in characters
)
{
  if( NULL == szPath )
    return;

	STD_TSTRING s( szPath, _tcslen(szPath) );

  // the the STD_TSTRING version
  AddTrailingBackSlash( s );

	// re-copy it across
	// strncpy( szPath, pstrRoot, nSize );
  memset( szPath, 0, nSize );
  _tcsncpy_s( szPath, nSize, s.c_str() , nSize );
}

/**
 * Add a trailing back slash at the end of a directory
 * @see AddTrailingBackSlash( STD_TSTRING& s )
 *
 * @param STD_TSTRING& the string we want to add a trailing back slash
 * @return none 
 */
void AddTrailingBackSlash( STD_TSTRING& subPath )
{
  _trimDeadChars( subPath );

  //  make sure we have the ending '/' or '\' 
  size_t l = subPath.length();
  if( l == 0 || (subPath[l-1] != '\\' && subPath[l-1] != '/') )
  {
    subPath += MYODD_FILE_SEPARATOR;
  }
  else
  {
    subPath[l-1] = MYODD_FILE_SEPARATOR_C;
  }
}

/**
 * UnExpand a string path into environment variables.
 * that way the user can pass a full path and get the environment variable.
 * This ensures that the user can change/rename paths and we should still be able to locate the files.
 * the calling function is responsible for cleaning the code.
 *
 * @param const STD_TSTRING the path we are editing
 * @param STD_TSTRING& the return file we are unexpanding.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment( const STD_TSTRING& src, STD_TSTRING& dest )
{
  LPTSTR unExpandEd;
  if( !myodd::files::UnExpandEnvironment( src.c_str(), unExpandEd ))
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
 * @param LPCTSTR the path we are editing
 * @param LPTSTR the object that will contain the unexpended file, (if return true), the calling function must delete this file.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment( LPCTSTR lpSrc, LPTSTR& dest )
{
  dest = NULL;
  if( NULL == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  STD_TSTRING stdUnDst = _T("");
  size_t unExpandSize = 0;

  STD_TSTRING stdSrc = lpSrc;

  //  we don't want the trailing back slash
  static STD_TSTRING appPath = GetAppPath( false );
  if( STD_TSTRING::npos != myodd::strings::ifind( lpSrc, appPath) )
  {
    // we have to expand the app path first
    // otherwise the drive letter will be replaced by %systemdrive%
    stdUnDst = myodd::strings::ireplace( stdSrc, appPath, FILE_APPPATH );
    unExpandSize = stdUnDst.length();
  }
  else
  {
    TCHAR unDst[ T_MAX_PATH ];
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
  dest = new TCHAR[ unExpandSize ];
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
 * @param const STD_TSTRING& the path we are editing
 * @param STD_TSTRING& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment( const STD_TSTRING& src, STD_TSTRING& dest )
{
	LPTSTR expandEd;
  if( !myodd::files::ExpandEnvironment( src.c_str(), expandEd ))
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
 * @param LPTSTR the path we are editing
 * @param LPTSTR& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment( LPCTSTR lpSrc, LPTSTR& dest )
{
  dest = NULL;
  if( NULL == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  // we might expand the value
  // so convert it to a string
  STD_TSTRING stdSrc = lpSrc;

  // look for the %apppath% in case we need to expand it.
  if( STD_TSTRING::npos != myodd::strings::ifind( lpSrc, FILE_APPPATH) )
  {
    //  we don't want the trailing back slash
    static STD_TSTRING appPath = GetAppPath( false );

    //  replace the string
    stdSrc = myodd::strings::ireplace( stdSrc, FILE_APPPATH, appPath );
  }

  // Get the 'expanded' size of the argument.
  // We don't expand anything.
  DWORD expandSize = (ExpandEnvironmentStrings( stdSrc.c_str(), 0, 0 ) + 1);
  if( 0 == expandSize )
  {
    // there was an error, use get last error.
    myodd::log::LogSystem( _T("ExpandEnvironment Error: %d"), ::GetLastError() );
    return false;
  }

  // Create the expanded item with the size we have.
  // and set it to 0 the terminating null should be included.
  dest = new TCHAR[ expandSize ];
  memset( dest, 0, expandSize );

  // now that we have all the needed information we can expand the string
  // but if there is an error we will return 0
  if( ExpandEnvironmentStrings( stdSrc.c_str(), dest, expandSize ) == 0 )
  {
    // there was an error, use get last error.
    myodd::log::LogSystem( _T("ExpandEnvironment Error2: %d"), ::GetLastError() );

    // simply return false as there was an error.
    // use GetLastError( ... )
    delete [] dest;
    dest = NULL;
    return false;
  }

  // success the file was expanded.
  return true;
}

/**
* Delete a file, try and expand it first.
* @see ::DeleteFile( ... )
* @param const STD_TSTRING& the file we want to delete
* @return bool if the file was deleted or not.
*/
bool DeleteFile( const STD_TSTRING& c )
{
  return DeleteFile( c.c_str() );
}

/**
 * Delete a file, try and expand it first.
 * @see ::DeleteFile( ... )
 * @param LPCTSTR the file we want to delete
 * @return bool if the file was deleted or not.
 */
bool DeleteFile( LPCTSTR c )
{
  LPTSTR lpExpand = NULL;
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
 * @param const STD_TSTRING& the existing filename 
 * @param const STD_TSTRING& the name of the new file.
 * @param DWORD* if not NULL we will pass the last error code
 * @return bool success or not.
 */
bool CopyFile( const STD_TSTRING& lpExistingFileName, const STD_TSTRING& lpNewFileName, DWORD* dwErr /*= 0*/ )
{
  return CopyFile( lpExistingFileName.c_str(), lpNewFileName.c_str(), dwErr );
}

/**
* Copy a file to a new file.
* @param const STD_TSTRING& the existing filename 
* @param const STD_TSTRING& the name of the new file.
* @param DWORD* if not NULL we will pass the last error code
* @return bool success or not.
*/
bool CopyFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD* dwErr /*= 0*/ )
{
  if( dwErr )
  {
    *dwErr = ERROR_SUCCESS;
  }

  STD_TSTRING stdExistingFileName( lpExistingFileName );
  if( !ExpandEnvironment( stdExistingFileName, stdExistingFileName ) )
  {
    //  cannot even expand it.
    if( dwErr )
    {
      *dwErr = ERROR_FILE_NOT_FOUND;
    }
    return false;
  }
  STD_TSTRING stdNewFileName( lpNewFileName );
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
* @param const STD_TSTRING& the full path of the file we want to check for
* @return bool if the file exists or not.
*/
bool FileExists( const STD_TSTRING& c )
{
  return FileExists( c.c_str() );
}

/**
 * check if a file is valid and exists on the hard drive or the network.
 * @param LPCTSTR the full path of the file we want to check for
 * @return bool if the file exists or not.
 */
bool FileExists( LPCTSTR c )
{
  LPTSTR lpExpand = NULL;
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
* @param LPCTSTR the full path of the file we want to check for
* @return bool if the directory exists or not.
*/
bool DirectoryExists( const STD_TSTRING& c )
{
  return DirectoryExists( c.c_str() );
}

/**
 * check if a directory is valid and exists on the hard drive or the network.
 * @param LPCTSTR the full path of the file we want to check for
 * @return bool if the directory exists or not.
 */
bool DirectoryExists( LPCTSTR c )
{

  // look for that file
  // if it exists then there is nothing to do really.
  STD_TSTRING findFile = (c);
  AddTrailingBackSlash( findFile );
  findFile += _T("*.*"); 
  
  return FileExists( findFile.c_str() );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param const STD_TSTRING& the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory( const STD_TSTRING& c, bool bIsFile )
{
  return CreateFullDirectory( c.c_str(), bIsFile );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param LPCTSTR the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory( LPCTSTR lpPath, bool bIsFile )
{
  LPTSTR lpDest = NULL;
  if( !myodd::files::ExpandEnvironment( lpPath, lpDest ))
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

  TCHAR tmpDir[ T_MAX_PATH+1 ];
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
  LPCTSTR p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != NULL )
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  LPCTSTR p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != NULL )
  {
    r2 = (int)(p2 - tmpDir + 1);
  }

  if( r1 == -1 && r2 == -1 )
  {
    // this looks like a relative path
    // so we just create the directory from where ever we are.
    return (CreateDirectory( tmpDir, NULL )?true:false);
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
  bool bResult = (CreateDirectory( lpDest, NULL )?true:false);
  delete [] lpDest;
  return bResult;
}

/**
 * Check if the string given is a valid existing file on the drive.
 * the string is expanded.
 *
 * @param const STD_TSTRING& stdFile the string we are checking for.
 * @return bool true|false if it is a file or not.
 */
bool IsFile(const STD_TSTRING& stdFile)
{
  return IsFile(stdFile.c_str());
}

/**
 * Check if the string given is a valid existing file on the drive.
 * the string is expanded.
 *
 * @param LPCTSTR the string we are checking for.
 * @return bool true|false if it is a file or not.
 */
bool IsFile( LPCTSTR lp )
{
  // expand the string.
  // the user could have passed environment variables.
  LPTSTR lpExpand = NULL;
  if( !ExpandEnvironment( lp, lpExpand ) )
  {
    // something broke
    return false;
  }

  // if the value was empty we have nothing
  if( NULL == lpExpand )
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
 * @param LPCTSTR the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsDirectory( const STD_TSTRING& stdDir )
{
  return IsDirectory( stdDir.c_str() );
}

/**
 * Check if the string given is a valid existing directory on the drive.
 * the string is expanded.
 *
 * @param LPCTSTR the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsDirectory( LPCTSTR lp )
{
  // expand the string.
  // the user could have passed environment variables.
  LPTSTR lpExpand = NULL;
  if( !ExpandEnvironment( lp, lpExpand ) )
  {
    // something broke
    // look inside the ExpandEnvironment( ... ) for details.
    return false;
  }

  // if the value was empty we have nothing
  if( NULL == lpExpand )
  {
    return false;
  }

  _tfinddata_t fdata;
  intptr_t ffhandle = _tfindfirst( lpExpand, &fdata );
  if( -1 == ffhandle )
  {
    STD_TSTRING s = lpExpand;
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
* @param const STD_TSTRING& stdUrl the string we are checking for.
* @return bool true|false if it is a directory or not.
*/
bool IsURL(const STD_TSTRING& stdUrl)
{
  return IsURL(stdUrl.c_str());
}

/**
 * Check if the string given LOOKS like a valid URL
 * This is a very basic check, we cannot actually ping the site to enusre that it is valid.
 *
 * @param LPCTSTR the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsURL( LPCTSTR lp )
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
    LPCTSTR wild = _T("^(ftps?|https?|gopher|mailto|news|nntp|telnet|wais|file|prospero|aim|webcal):.+");
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
 * @see GetFileInformationByName( LPCTSTR, ... )
 * @param LPCTSTR the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged( const STD_TSTRING& file, const BY_HANDLE_FILE_INFORMATION& info )
{
  return HasFileInformationChanged( file.c_str(), info );
}

/**
 * Check if the file information has changed, we pass the file name we are checking
 * as well as an old file information.
 * @see GetFileInformationByName( LPCTSTR, ... )
 * @param LPCTSTR the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged( LPCTSTR file, const BY_HANDLE_FILE_INFORMATION& info )
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
 * @see GetFileInformationByName( LPCTSTR, ... )
 * @param LPCTSTR the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName( const STD_TSTRING& file, BY_HANDLE_FILE_INFORMATION& info )
{
  return GetFileInformationByName( file.c_str(), info );
}

/**
 * Get a file information given a file name
 * @see ::GetFileInformationByHandle ( ... )
 * @param LPCTSTR the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName( LPCTSTR file, BY_HANDLE_FILE_INFORMATION& info )
{
  // expand the file.
  LPTSTR lpDest;
  if( !myodd::files::ExpandEnvironment( file, lpDest ))
  {
    return false;
  }

  // get the original file info.
  HANDLE hSrcFile;
  if ((hSrcFile = CreateFile(lpDest,  
                             GENERIC_READ, 
                             FILE_SHARE_READ,
                             NULL, 
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
 * @return STD_TSTRING the path of the current exe.
 */
STD_TSTRING GetAppPath( bool bAddtrailing /*=true*/)
{
  STD_TSTRING sReturn = _T("");
  TCHAR* lpBuffer = new TCHAR[ T_MAX_PATH ];
  if (GetModuleFileName( NULL, lpBuffer, T_MAX_PATH ) != 0)
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
 * @param LPCTSTR the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @param bool add a trailling backslash or not.
 * @return STD_TSTRING the directory of the file given
 */
STD_TSTRING GetBaseFromFile
( 
  const STD_TSTRING& stdPath, 
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
 * @param LPCTSTR the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @return STD_TSTRING the directory of the file given
 */
STD_TSTRING GetBaseFromFile
( 
  LPCTSTR lpPath, 
  bool bExpand /*= true*/, 
  bool bAddTrailling /*= true*/ 
)
{
  // expand the full path
  // this is in case the user uses some weird environment variable that contains the file name in it.
  // reverse look for the first '\' or '/'
  TCHAR tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  if( bExpand )
  {
    LPTSTR lpDest = NULL;
    if( !myodd::files::ExpandEnvironment( lpPath, lpDest ))
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
  LPCTSTR p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != NULL )
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  LPCTSTR p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != NULL )
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

  STD_TSTRING s = ( tmpDir );
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
* @param STD_TSTRING& container for the return value, unset in case of error. 
* @param const STD_TSTRING& the relative path we would like to get the absolute path from.
* @param LPCTSTR the original path, if NULL we will use the current directory as origin.
* @return bool if we were able to get the absolute path, false if the given path is unrealistic/impossible.
*/
bool GetAbsolutePath
( 
 STD_TSTRING& dest, 
 const STD_TSTRING& stdRelative, 
 LPCTSTR lpOrigin /*= NULL*/ 
 )
{
  //  pass this to the other function.
  LPTSTR lpDest = NULL;
  if( !myodd::files::GetAbsolutePath( lpDest, stdRelative.c_str(), lpOrigin ))
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
 * @param LPTSTR& container for the return value, unset in case of error. 
 * @param LPCTSTR the relative path we would like to get the absolute path from.
 * @param LPCTSTR the original path, if NULL we will use the current directory as origin.
 * @return bool if we were able to get the absolute path, false if the given path is unrealistic/impossible.
 */
bool GetAbsolutePath
( 
  LPTSTR& dest, 
  LPCTSTR lpRelative, 
  LPCTSTR lpOrigin /*= NULL*/ 
)
{
  dest = NULL;
  if( NULL == lpRelative )
  {
    //  we need a relative path to work with.
    return false;
  }

  STD_TSTRING sRelative( lpRelative );
  STD_TSTRING sOrigin( _T("") );
  if( NULL == lpOrigin )
  {
    TCHAR lpBuffer[ T_MAX_PATH ] = {0};
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
  static LPCTSTR SEPARATOR_REPLACE = _T("\\" );
  static TCHAR SEPARATOR_REPLACE_C = _T('\\' );
  static LPCTSTR SEPARATOR_REPLACE_FROM = _T("/");

  // now we need to replace all the '/' with '\' so we don't worry about UNC stuff.
  // don't use MYODD_FILE_SEPARATOR otherwise we might not replace anything at all.
  sOrigin = myodd::strings::replace ( sOrigin   , SEPARATOR_REPLACE_FROM, SEPARATOR_REPLACE );
  sRelative = myodd::strings::replace( sRelative, SEPARATOR_REPLACE_FROM, SEPARATOR_REPLACE );

  //  will we need to add a trailing char?
  // remember we are not checking for MYODD_FILE_SEPARATOR as we replace all the "\"
  bool addtrailing = (sRelative[ sRelative.length() -1 ] == SEPARATOR_REPLACE_C );

  // split them all
  std::vector<STD_TSTRING> e_sRelative;
  myodd::strings::explode( e_sRelative, sRelative, SEPARATOR_REPLACE_C, -1, false );

  std::vector<STD_TSTRING> e_sOrigin;
  myodd::strings::explode( e_sOrigin, sOrigin, SEPARATOR_REPLACE_C, -1, false );
  // reverse from the origin path we only reverse the ../

  size_t dotdotCount = 0;
  size_t dotCount = 0;
  for( std::vector<STD_TSTRING>::const_iterator it = e_sRelative.begin();
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
    if( sRelative.find( _T("%") ) !=  STD_TSTRING::npos )
    {
      STD_TSTRING expanded = _T("");
      if( ExpandEnvironment( sRelative, expanded ) )
      {
        if( GetAbsolutePath( dest, expanded.c_str(), sOrigin.c_str() ) )
        {
          STD_TSTRING absolute = dest;
          delete [] dest;
          dest = NULL;

          return UnExpandEnvironment( absolute.c_str(), dest );
        }
      }
    }
    else if( sRelative.find( _T(":") ) ==  STD_TSTRING::npos )
    {
      // so what we will do is actually add the origin to the given path
      // and try and see if we can get that to work.
      AddTrailingBackSlash(sOrigin);
      STD_TSTRING correctedPath = sOrigin + sRelative;
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
  std::vector<STD_TSTRING> e_sClean;
  for( std::vector<STD_TSTRING>::const_iterator it = e_sOrigin.begin();
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
  STD_TSTRING imp = myodd::strings::implode( e_sClean, MYODD_FILE_SEPARATOR );
  if( addtrailing )
  {
    imp += MYODD_FILE_SEPARATOR;
  }
  size_t implodeSize = imp.length()+1;

  dest = new TCHAR[ implodeSize ];
  memset( dest, 0, implodeSize );

  _tcscpy_s( dest, implodeSize, imp.c_str() );

  return true;
}

/**
 * Remove illegal characters in a file name.
 * Do not pass a full path name as '/' will be replaced.
 * @param STD_TSTRING& the file been cleaned.
 * @return none
 */
void CleanFileName( STD_TSTRING& dirtyFileName )
{
  static TCHAR* badChars = _T("?[]/\\=+<>:;\",*|^\"");
  BOOST_FOREACH( TCHAR &tch, dirtyFileName )
  {
    BOOST_FOREACH( const TCHAR& bad, badChars )
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
 * @param STD_TSTRING& the return value.
 * @param LPCTSTR the prefix or NULL if no prefix is needed.
 * @param LPCTSTR the prefix or NULL if no extension is needed, (if NULL '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName( STD_TSTRING& stdFileName, LPCTSTR lpPrefix, LPCTSTR lpExt )
{
  // use the STD_TSTRING function.
  TCHAR* lpFileName = NULL;
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
 * @param LPTSTR& the return value, the calling function will need to delete that value.
 * @param LPCTSTR the prefix or NULL if no prefix is needed.
 * @param LPCTSTR the prefix or NULL if no extension is needed, (if NULL '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName( LPTSTR& lpFileName, LPCTSTR lpPrefix, LPCTSTR lpExt )
{
  // add a unique, random number.
  int unique_num = myodd::math::random_range( 0, 65535 );

  // create the return file
  TCHAR* lpTmpFile = new TCHAR[T_MAX_PATH];
  _stprintf_s( lpTmpFile, T_MAX_PATH, _T("%s%05d.%s"), (lpPrefix == NULL ? _T("~myodd") : lpPrefix),
                                                        unique_num,
                                                       (lpExt == NULL ? _T("tmp") : lpExt)
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
 * @param STD_TSTRING& the return buffer that will contain the file+full path.
 * @param LPCTSTR the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName( STD_TSTRING& stdFullPathFileName, LPCTSTR lpFileName )
{
  TCHAR* lpFullPathFileName = NULL;
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
 * @param LPTSTR& the return buffer that will contain the file+full path.
 * @param LPCTSTR the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName( LPTSTR& lpFullPathFileName, LPCTSTR lpFileName )
{
  // get the temp path
  TCHAR* lpTmpBuffer = new TCHAR[T_MAX_PATH];
  DWORD dwLen = GetTempPath( T_MAX_PATH, lpTmpBuffer );
  if( 0 == dwLen )
  {
    // clean the temp path.
    delete [] lpTmpBuffer;

    //  could not create the temp path.
    return false;
  }

  // add the AddTrailingBackSlash to it.
  myodd::files::AddTrailingBackSlash( lpTmpBuffer, T_MAX_PATH );

  // create the return buffer.
  lpFullPathFileName = new TCHAR[ T_MAX_PATH + 1];
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
 * @param LPCTSTR the file we are checking.
 * @return bool if it is a directory or not
 */
bool is_dot( const STD_TSTRING& stdFile )
{
  return is_dot( stdFile.c_str() );
}

/**
 * Check if a given string is a dot/dotdot directory
 * @param LPCTSTR the file we are checking.
 * @return bool if it is a directory or not
 */
bool is_dot( LPCTSTR lpFile )
{
  // now we need to replace all the '/' with '\' so we don't worry about UNC stuff.
  STD_TSTRING sOrigin = lpFile;
  sOrigin = myodd::strings::replace( sOrigin, _T("/"), _T("\\") );
  
  std::vector<STD_TSTRING> e_sOrigin;
  size_t size = myodd::strings::explode( e_sOrigin, sOrigin, _T('\\'), -1, false );
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
* @param LPCTSTR the full path we are getting.
* @param bool if we want to expand the string or not, used to prevent recursive calls.
* @return STD_TSTRING the filename of the file given without the directory
*/
STD_TSTRING GetFileName( const STD_TSTRING& stdPath, bool bExpand /*= true*/ )
{
  return GetFileName( stdPath.c_str(), bExpand );
}

/**
 * Get the filename from a full path, so "c:\somthing\whatever.txt" would return "whatever.txt"
 * we don't check if the file exits as it might not be a real file.
 * @see GetBaseFromFile( ... )
 * @param LPCTSTR the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @return STD_TSTRING the filename of the file given without the directory
 */
STD_TSTRING GetFileName( LPCTSTR lpPath, bool bExpand /*= true*/ )
{
  // expand the full path
  // this is in case the user uses some weird environment variable that contains the file name in it.
  // reverse look for the first '\' or '/'
  TCHAR tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  if( bExpand )
  {
    LPTSTR lpDest = NULL;
    if( !myodd::files::ExpandEnvironment( lpPath, lpDest ))
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
  LPCTSTR p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != NULL )
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  LPCTSTR p2 = _tcsrchr( tmpDir, '/' );
  if( p2 != NULL )
  {
    r2 = (int)(p2 - tmpDir + 1);
  }

  if( r1 == -1 && r2 == r2 -1 )
  {
    return tmpDir;  // return was was passed.
  }

  STD_TSTRING s = _T("");
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
 * @param LPCTSTR the file we are checking.
 * @param int* if not NULL we will set the number of characters to skip
 * @return FileEncode the file encoding.
 */
FileEncode DetermineEncoding( LPCTSTR file, int* npSkip )
{
  if( npSkip )
    *npSkip = 0;
  if( NULL == file )
  {
    ASSERT( 0 ); //  what are we trying to do?
    return uniError;
  }

  FILE* fp = 0;
  errno_t err = _tfopen_s( &fp, file, _T("rb") );
  if ( err != 0 || !fp )
  {
    ASSERT( 0 );
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
  else if (nLen >= 2 && b[0] != NULL && b[1] == NULL && IsTextUnicode(b, (int)nLen, NULL))
  {
    encoding = uni16LE_NoBOM;
    nSkip = 0;
  }
  else if (nLen >= 2 && IsTextUnicode(b, (int)nLen, NULL))
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
 * @param LPCTSTR the file we want to read.
 * @param __int64 the start position we want to read from
 * @param __int64 the end position we want to read to.
 * @return TCHAR* the data or NULL if there was an error.
 */
TCHAR* ReadFile( LPCTSTR file, __int64 nStartPos, __int64 nEndPos )
{
  // sanity check
  if( NULL == file )
  {
    return NULL;
  }

  // get the file encoding.
  int nSkip = 0;
  FileEncode encoding = DetermineEncoding( file, &nSkip );

  FILE* fp = 0;
  errno_t err = _tfopen_s( &fp, file, _T("rb" ));
  if ( err || NULL == fp )
  {
    // could not open the file.
    return NULL;
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
  TCHAR* tbuf_ = NULL;
  try
  {
    // @Todo this could be a problem here when length is longer than (size_t)-1
    // this will almost certainly never happen as the amount of memory needed would
    // probably exceed the amount of memory available.
    char* buf = new char[ (size_t)length+1 ]; 
    if( NULL != buf )
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
    tbuf_ = NULL;
  }
  // we are done here
  fclose( fp );
  return tbuf_;
}

void Join( STD_TSTRING& returnPath, const STD_TSTRING& pathPartA, const STD_TSTRING& pathPartB )
{
  STD_TSTRING partA = pathPartA;
  STD_TSTRING partB = pathPartB;

  // make sure we have a trailing backslash
  AddTrailingBackSlash( partA );

  // and make sure that we don't have a leading one.
  RemoveLeadingBackSlash( partB );
  
  // join the two together.
  returnPath = partA + partB;
}

// --------------------------------------------------------------------------------------------------
Version::Version( LPCTSTR lpFileName /*= NULL*/ )
{
  if( lpFileName )
  {
    DetermineFileVersion( lpFileName );
  }
  else
  {
    TCHAR	strTempBuffer[ MAX_PATH ];
    // Retrieve the fully-qualified path from the current process
    if ( NULL != ::GetModuleFileName( NULL,	// Current module
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
 * @param LPCTSTR the filename of the file whose version we want.
 * @return none
 */
void Version::DetermineFileVersion( LPCTSTR lpFileName )
{
  ASSERT( lpFileName );
  if( !lpFileName )
  {
    return;
  }

  DWORD	dwHandle = 0;

  // Determines whether the operating system can retrieve version information
  // for a specified file.
  DWORD	dwFileVersionInfoSize = GetFileVersionInfoSize( lpFileName, &dwHandle );

  if ( NULL == dwFileVersionInfoSize )
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
                                    static_cast<LPVOID>( &pData[0] ) ) )
  {
    return; // Can't retrieve version information.
  }

  // The memory of ptFileInfo is freed when pData is freed.
  VS_FIXEDFILEINFO *ptFileInfo;
  UINT	uintSize;

  // Retrieves version information from the version-information resource
  if ( false == VerQueryValue( static_cast<LPVOID>( &pData[0] ),
                               _T("\\"),
                               reinterpret_cast<LPVOID*> ( &ptFileInfo ),
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
                                      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if( hFile ) 
  {
    if( 0 != GetFileTime( hFile, NULL, NULL, &fileTime ) )
    {
      FileTimeToSystemTime(&fileTime, &m_fileSystemFime); 
    }
    CloseHandle(hFile); 
  }
}

/**
 * Get all the app keys from a .cfg filename.
 * @param LPCTSTR the name of the file
 * @param std::vector<STD_TSTRING>& the various keys.
 * @param LPCTSTR the app name, NULL if we want all the keys.
 * @param LPCTSTR | NULL the wild search to limit certain keys only or NULL to get all the keys.
 * @return std::vector<STD_TSTRING> list of all the keys or some that match the seatch.
 */
size_t GetKeys
( 
  LPCTSTR lpFileName, 
  std::vector<STD_TSTRING>& tokens,
  LPCTSTR lpAppName,
  LPCTSTR lpWild /*=NULL*/
)
{
  tokens.clear();

  //  get all the keys
  DWORD keys_size = T_MAX_PATH;
  while( true )
  {
    TCHAR* keys = new TCHAR[ keys_size ];
    size_t nLen = ::GetPrivateProfileString( lpAppName, NULL, _T(""), keys, keys_size, lpFileName );
    if( (keys_size-2) == nLen )
    {
      keys_size+=T_MAX_PATH;
      delete [] keys;
    }
    else
    {
      //  get the tokens.
      std::vector<STD_TSTRING> tokensTmp;
      myodd::strings::explode_by_null_char( tokensTmp, keys, nLen );
      for( std::vector<STD_TSTRING>::const_iterator it = tokensTmp.begin(); 
           it != tokensTmp.end();
           ++it
         )
      {
        if( lpWild )
        {
          // limit the search
          if( !myodd::strings::wildcmp( lpWild, (*it).c_str() ))
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
  eState getState() { return m_eState; };
  operator bool() { return m_pRead <= m_pEnd; };

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
  m_pBuf = NULL;
  m_pRead = NULL;
  m_pEnd = NULL;
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
 * @return TCHAR* the converted file.
 */
TCHAR* Byte2Char
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
  TCHAR* tbuf = 0;
  switch( fileEncoding )
  {
  case uni8Bit:
    tbuf = myodd::strings::char2wchar( buf, CP_ACP );
    break;

  case uni16BE: 
  case uni16LE:
  case uni16LE_NoBOM:
    {
      // get the max new size
      size_t newSize = len + len / 2 + 1;
      char* cbuf  = new char[newSize];
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

      // convert to TCHAR
      tbuf = myodd::strings::char2wchar( cbuf, CP_UTF8 );

      // clean up
      delete [] cbuf;
      cbuf = 0;
    }
    break;

  default:
    tbuf = myodd::strings::char2wchar( buf, CP_UTF8 );
    break;
  }
  return tbuf;
#endif
}

/** 
 * Get the size of a file
 * We return -1 if the file does not exist, we do not have permission and/or there was some other error.
 * @param const STD_TSTRING& stdFullPathFileName the file we are checking.
 * @return long Total size, in bytes
 */
long GetFileSizeInBytes( const STD_TSTRING& stdFullPathFileName )
{
  STD_TSTRING cleannedFileName = stdFullPathFileName;
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