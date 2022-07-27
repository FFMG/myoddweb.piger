#include "stdafx.h"

#include "../files/files.h"
#include "../string/string.h"
#include "../math/math.h"
#include "../log/log.h"
#include "../pcre2/regex2.h"
#include <io.h>

// Look for version.lib
#pragma comment( lib, "version.lib" )

static const wchar_t* FILE_APPPATH = _T("%apppath%");

/**
 * TODO : Maybe move to myodd::strings
 * TODO : Why are we getting dead chars in the first place? 
 * This came from casting in the Clipboard, so maybe data is not copied properly.
 *
 * @param std::wstring& the string we want to remove dead code from
 * @return none 
 */
inline void _TrimDeadChars( std::wstring& s )
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
static const wchar_t* MYODD_FILE_WINSEPARATOR = _T("\\");

// the default separator
static const wchar_t* MYODD_FILE_SEPARATOR   = MYODD_FILE_WINSEPARATOR;
static const wchar_t MYODD_FILE_SEPARATOR_C  = MYODD_FILE_WINSEPARATOR[0];

namespace myodd{ namespace files{
void Test()
#ifdef _DEBUG
{
  wchar_t* lpdest = nullptr;
  auto appPath = GetAppPath( false );
  ASSERT( ExpandEnvironment( FILE_APPPATH, lpdest ) );
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  ASSERT( ExpandEnvironment( L"%AppPath%", lpdest ) );   //  case insensitive
  ASSERT( _tcsicmp( lpdest, appPath.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;
  ASSERT( ExpandEnvironment( L"%AppPath%\\somedir\\somefile.txt", lpdest ) );   //  case insensitive
  std::wstring s = appPath + L"\\somedir\\somefile.txt";
  ASSERT( _tcsicmp( lpdest, s.c_str() ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;

  ASSERT( UnExpandEnvironment( L"%systemdrive%", lpdest ) );
  delete [] lpdest;

  ASSERT( UnExpandEnvironment( appPath.c_str(), lpdest ) );
  ASSERT( _tcsicmp( lpdest, FILE_APPPATH ) == 0 );
  delete [] lpdest;
  lpdest = nullptr;
}
#else
{
  __noop;
}
#endif // _DEBUG

/**
 * Add an extension to a given string/filename
 * The extension does not need to have a '.', a check is already made.
 * @param const std::wstring& the 'file' or string we want to add the extension to.
 * @param const std::wstring& the extension we want to add to the string.
 * @param bool strip the current extension if there is one.
 * @return none
 */
void AddExtension(std::wstring& filename, const std::wstring& extension, bool strip_current_if_exists ) 
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

  if( *filename.rbegin() == L'.' )
  {
    filename = filename.substr( 0, filename.length() -1 );
  }

  // do we have a dot in the extension?
  // we already know the size is not 0
  if( *extension.begin() == L'.' )
  {
    filename += extension;
  }
  else
  {
    filename += L'.' + extension;
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
 * @param std::wstring& the string we want to remove the extension from.
 * @return none 
 */
void StripExtension( std::wstring& filename)
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
 * @param std::wstring the file name
 * @return std::wstring the extension, (or empty string if there are none).
 */
std::wstring GetExtension( const std::wstring& filename )
{
  // make a copy so we can strip the dead characters.
  auto filenameCopy(filename);
  _TrimDeadChars(filenameCopy);

  // look for the last '.'
  auto pos = filenameCopy.find_last_of( L'.' );

  // we don't want to return extensions like 'htaccess' from a file '.htaccess'
  // this is because the file name itself is '.htaccess' with no extention.
  return (pos > 0 && pos != std::wstring::npos ? filenameCopy.substr( pos+1 ) : L"" );
}

/**
 * Check if a files extension is the one given
 * @param std::wstring the file name
 * @param std::wstring the extension we are checking for.
 * @return bool if the file extension is the one we wanted.
 */
bool IsExtension( const std::wstring& given_filename, const std::wstring& extension )
{
  // make a copy of the given filename so we can trim it
  auto copy_given_filename(given_filename);
  _TrimDeadChars(copy_given_filename);
  strings::Trim(copy_given_filename);
  
  // make a copy of the given extension so we can trim it
  auto copy_extension(extension);
  _TrimDeadChars( copy_extension );
  strings::Trim(copy_extension);

  // if we have a leading '.' then we need to remove it.
  // in case the extension is something like '.txt' it becomes 'txt'
  if(copy_extension.length() >= 1 && copy_extension[0] == '.' )
  {
    copy_extension = copy_extension.substr( 1 );
  }

  // if either of them are size 0 then we cannot check anything.
  if(copy_extension.length() == 0 || copy_extension.length() == 0 )
  {
    return false; //  we cannot check 0 lengths
  }

  // unescape the characters in the extension.
  // this is in case we are looking for something like .txt.file
  // we will trim the first '.' but we need to unescape the second one.
  copy_extension = strings::Replace(copy_extension, L"\\.", L"." );     //  in case the user escaped it already

  // escape all the other characters.
  copy_extension = myodd::regex::Regex2::Escape(copy_extension);
  auto pattern = L"^(.*)\\.(" + copy_extension + L")$";

  // search for this pattern
  return myodd::regex::Regex2::Search(pattern.c_str(), copy_given_filename.c_str(), false);
}


/**
 * Remove a leading backslash at the start of a path
 * This is normally used so we can joining 2 paths together.
 * @param std::wstring the path we want to remove the backslash from
 * @return none
 */
void RemoveLeadingBackSlash( std::wstring& szPath)
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
 * @param std::wstring& the string we want to remove the trailing back slash
 * @return none 
 */
void RemoveTrailingBackSlash
(	
 std::wstring& szPath
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
 * @see AddTrailingBackSlash( std::wstring& s )
 *
 * @param std::wstring& the string we want to add a trailing back slash
 * @return none 
 */
void AddTrailingBackSlash( std::wstring& subPath )
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
 * @param const std::wstring the path we are editing
 * @param std::wstring& the return file we are unexpanding.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment( const std::wstring& src, std::wstring& dest )
{
  wchar_t* unExpandEd;
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
 * @param const wchar_t* the path we are editing
 * @param wchar_t* the object that will contain the unexpended file, (if return true), the calling function must delete this file.
 * @return bool false if there was a problem with the string 
 */
bool UnExpandEnvironment(const wchar_t* lpSrc, wchar_t*& dest )
{
  dest = nullptr;
  if(nullptr == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  std::wstring stdUnDst = _T("");
  size_t unExpandSize = 0;

  std::wstring stdSrc = lpSrc;

  //  we don't want the trailing back slash
  auto appPath = GetAppPath( false );
  if( std::wstring::npos != strings::Find( lpSrc, appPath, 0, false ) )
  {
    // we have to expand the app path first
    // otherwise the drive letter will be replaced by %systemdrive%
    stdUnDst = strings::Replace( stdSrc, appPath, FILE_APPPATH, false );
    unExpandSize = stdUnDst.length();
  }
  else
  {
    wchar_t unDst[ T_MAX_PATH ];
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
  dest = new wchar_t[ unExpandSize ];
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
 * @param const std::wstring& the path we are editing
 * @param std::wstring& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment( const std::wstring& src, std::wstring& dest )
{
  wchar_t* expandEd;
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
 * @param const wchar_t* the path we are editing
 * @param wchar_t*& the destination.
 * @return bool false if there was a problem with the string 
 */
bool ExpandEnvironment(const wchar_t* lpSrc, wchar_t*& dest )
{
  dest = nullptr;
  if(nullptr == lpSrc || _tcslen( lpSrc ) == 0  )
  {
    // no error, just nothing to do really.
    return true;
  }

  // we might expand the value
  // so convert it to a string
  std::wstring stdSrc = lpSrc;

  // look for the %apppath% in case we need to expand it.
  if( std::wstring::npos != strings::Find( lpSrc, FILE_APPPATH, 0, false) )
  {
    //  we don't want the trailing back slash
    static auto appPath = GetAppPath( false );

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
  dest = new wchar_t[ expandSize ];
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
* @param const std::wstring& the file we want to delete
* @return bool if the file was deleted or not.
*/
bool DeleteFile( const std::wstring& c )
{
  return DeleteFile( c.c_str() );
}

/**
 * Delete a file, try and expand it first.
 * @see ::DeleteFile( ... )
 * @param const wchar_t* the file we want to delete
 * @return bool if the file was deleted or not.
 */
bool DeleteFile(const wchar_t* c )
{
  wchar_t* lpExpand = nullptr;
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
 * @param const std::wstring& the existing filename 
 * @param const std::wstring& the name of the new file.
 * @param unsigned long* if not nullptr we will pass the last error code
 * @return bool success or not.
 */
bool CopyFile( const std::wstring& lpExistingFileName, const std::wstring& lpNewFileName, unsigned long* dwErr /*= 0*/ )
{
  return CopyFile( lpExistingFileName.c_str(), lpNewFileName.c_str(), dwErr );
}

/**
* Copy a file to a new file.
* @param const std::wstring& the existing filename 
* @param const std::wstring& the name of the new file.
* @param unsigned long* if not nullptr we will pass the last error code
* @return bool success or not.
*/
bool CopyFile(const wchar_t* lpExistingFileName, LPCTSTR lpNewFileName, unsigned long* dwErr /*= 0*/ )
{
  if( dwErr )
  {
    *dwErr = ERROR_SUCCESS;
  }

  std::wstring stdExistingFileName( lpExistingFileName );
  if( !ExpandEnvironment( stdExistingFileName, stdExistingFileName ) )
  {
    //  cannot even expand it.
    if( dwErr )
    {
      *dwErr = ERROR_FILE_NOT_FOUND;
    }
    return false;
  }
  std::wstring stdNewFileName( lpNewFileName );
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
* @param const std::wstring& the full path of the file we want to check for
* @return bool if the file exists or not.
*/
bool FileExists( const std::wstring& c )
{
  return FileExists( c.c_str() );
}

/**
 * check if a file is valid and exists on the hard drive or the network.
 * @param const wchar_t* the full path of the file we want to check for
 * @return bool if the file exists or not.
 */
bool FileExists(const wchar_t* c )
{
  wchar_t* lpExpand = nullptr;
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
* @param const wchar_t* the full path of the file we want to check for
* @return bool if the directory exists or not.
*/
bool DirectoryExists( const std::wstring& c )
{
  return DirectoryExists( c.c_str() );
}

/**
 * check if a directory is valid and exists on the hard drive or the network.
 * @param const wchar_t* the full path of the file we want to check for
 * @return bool if the directory exists or not.
 */
bool DirectoryExists(const wchar_t* c )
{

  // look for that file
  // if it exists then there is nothing to do really.
  std::wstring findFile = (c);
  AddTrailingBackSlash( findFile );
  findFile += _T("*.*"); 
  
  return FileExists( findFile.c_str() );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param const std::wstring& the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory( const std::wstring& c, bool bIsFile )
{
  return CreateFullDirectory( c.c_str(), bIsFile );
}

/**
 * Create a full directly including all the sub directory.
 * the function looks for the first existing directory and creates directories from then on.
 *
 * @param const wchar_t* the path we trying to created
 * @param bool true|false if the file includes a file, we need to know that so we don't create a directory with the file name.
 * @return bool false if there was a problem with the string / creating the dir.
 */
bool CreateFullDirectory(const wchar_t* lpPath, bool bIsFile )
{
  wchar_t* lpDest = nullptr;
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

  wchar_t tmpDir[ T_MAX_PATH+1 ];
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
  const wchar_t* p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != nullptr)
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  const wchar_t* p2 = _tcsrchr( tmpDir, '/' );
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

bool _IsUrlCommon(const std::wstring& path)
{
  // empty is allowed, it just means we have a '/', (at the end)
  // or that we have doubles '/', not ideal, but not a train smash
  if ( path.length() == 0)
  {
    return true;
  }

  // the pattern for each sections.
  static const auto pattern = _T("^(?:[\\p{L}\\p{M}\\p{S}\\p{N}\\p{P}])+$");
  return regex::Regex2::Search(pattern, path, false);
}

bool _IsUrlFirst(const std::wstring& path)
{
  std::wstring authority;
  std::wstring host = path;
  std::wstring port;

  // get the authority
  std::vector<std::wstring> authorityAndHost;
  if (2 == strings::Explode(authorityAndHost, host, _T('@'), 2))
  {
    authority = authorityAndHost[0];
    host = authorityAndHost[1];

    // check the authority now
    static const auto pattern = _T("^(?:\\S+(?::\\S*)?)?$");

    // in the first string read the username[:password] 
    if (!regex::Regex2::Search(pattern, authority, false))
    {
      return false;
    }
  }

  // get the port
  std::vector<std::wstring> hostAndPort;
  if (2 == strings::Explode(hostAndPort, host, _T(':'), 2))
  {
    host = hostAndPort[0];
    port = hostAndPort[1];

    // check the port now.
    static const auto pattern = _T("^[0-9]{2,}$");

    if (!regex::Regex2::Search(pattern, port, false))
    {
      return false;
    }
  }

  // look for invalid parametters
  static const auto pattern = _T("^(?:[\\?\\.#])");
  if (regex::Regex2::Search(pattern, host, false))
  {
    return false;
  }
  
  // first item cannot be '/'
  if (host.length() == 0)
  {
    return false;
  }

  //  now the host must be valid.
  return _IsUrlCommon(host);
}

/**
 * Check if the string given LOOKS like a valid URL
 * This is a very basic check, we cannot actually ping the site to enusre that it is valid.
 *
 * @param const std::wstring& stdUrl the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsURL(const std::wstring& givenUrl)
{
  auto matches = regex::Regex2::Matches();
  std::wstring url(givenUrl);

  try
  {
    //  clean it up.
    _TrimDeadChars(url);

    //  get the protocol identifier and remove it.
    // http://www.example.com becomes www.example.com
    const auto pattern_protocol = _T("^([[:alnum:]]+:\\/{2}).*");
    if (0 == regex::Regex2::Match(pattern_protocol, url, matches, false))
    {
      return false;
    }

    // https://www.mattcutts.com/blog/seo-glossary-url-definitions/
    auto protocolIdentifier = static_cast<std::wstring>(matches[1]);
    url = url.substr(protocolIdentifier.length());

    // get the domain and path
    std::vector<std::wstring> hostAndPaths;
    if (url.length() == 0 || 0 == strings::Explode(hostAndPaths, url, _T('/')))
    {
      // if we are here, we got a protocol, but nothing after that
      // in other words, we got "http://" and nothing else.
      // so we cannot go any further than that.
      return false;
    }

    //  look at the domain and path(s)
    for (auto it = hostAndPaths.begin(); it != hostAndPaths.end(); ++it)
    {
      if (it == hostAndPaths.begin())
      {
        if (!_IsUrlFirst( *it ))
        {
          return false;
        }
      }
      else
      {
        if (!_IsUrlCommon( *it ))
        {
          return false;
        }
      }
    }

    //  if we are here, then all is good!
    return true;
  }
  catch (const std::runtime_error & e)
  {
    UNUSED_ALWAYS(e);
    // std::runtime_error if the complexity of matching the expression 
    // against an N character string begins to exceed O(N2), 
    // or if the program runs out of stack space
  }
  catch (...)
  {
    // boost couldn't work it out.
  }
  return false;
}

/**
 * Check if a given string is a dot/dotdot directory
 * @param const wchar_t* the file we are checking.
 * @return bool if it is a directory or not
 */
bool IsDot(const std::wstring& givenFile)
{
  // now we need to replace all the '/' with '\' so we don't worry about UNC stuff.
  auto copyOfGivenFile = strings::Replace(givenFile, _T("/"), _T("\\"));

  std::vector<std::wstring> v_copyOfGivenFile;
  auto size = strings::Explode(v_copyOfGivenFile, copyOfGivenFile, _T('\\'), MYODD_MAX_INT32, false);
  if (size == 0)
  {
    return false;
  }

  // check if it is a dot.
  if (v_copyOfGivenFile[size - 1] == _T(".."))
  {
    return true;
  }
  if (v_copyOfGivenFile[size - 1] == _T("."))
  {
    return true;
  }
  return false;
}

/**
 * Check if the string given is a valid existing file on the drive.
 * the string is expanded.
 *
 * @param const std::wstring& stdFile the string we are checking for.
 * @return bool true|false if it is a file or not.
 */
bool IsFile( const std::wstring& givenFile )
{
  // expand the string.
  // the user could have passed environment variables.
  auto expandedGivenFile = givenFile;
  if( !ExpandEnvironment(givenFile, expandedGivenFile) )
  {
    // something broke
    return false;
  }

  _tfinddata_t fdata;
  auto ffhandle = _tfindfirst(expandedGivenFile.c_str(), &fdata );

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
 * @param const wchar_t* the string we are checking for.
 * @return bool true|false if it is a directory or not.
 */
bool IsDirectory( const std::wstring& givenDirectory )
{
  // expand the string.
  // the user could have passed environment variables.
  auto expandedGivenDirectory = givenDirectory;
  if( !ExpandEnvironment(givenDirectory, expandedGivenDirectory) )
  {
    // something broke
    // look inside the ExpandEnvironment( ... ) for details.
    return false;
  }

  _tfinddata_t fdata;
  intptr_t ffhandle = _tfindfirst(expandedGivenDirectory.c_str(), &fdata );
  if( -1 == ffhandle )
  {
    files::Join(expandedGivenDirectory, expandedGivenDirectory, _T("*.*"));

    // if we find something we will not check 
    // the file attrib as know it is a directory.
    ffhandle = _tfindfirst(expandedGivenDirectory.c_str(), &fdata );
    if( -1 == ffhandle )
    {
      return false;
    }

    // clear the handle
    // we found this item and it is a directory.
    _findclose( ffhandle );
    return true;
  }// find returned a -1

  // clear the handle
  _findclose( ffhandle );

  if( (fdata.attrib & _A_SUBDIR) != _A_SUBDIR )
  {
    return false;
  }

  return true;
}

/**
 * Check if the file information has changed, we pass the file name we are checking
 * as well as an old file information.
 * @see GetFileInformationByName( const wchar_t*, ... )
 * @param const wchar_t* the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged( const std::wstring& file, const BY_HANDLE_FILE_INFORMATION& info )
{
  return HasFileInformationChanged( file.c_str(), info );
}

/**
 * Check if the file information has changed, we pass the file name we are checking
 * as well as an old file information.
 * @see GetFileInformationByName( const wchar_t*, ... )
 * @param const wchar_t* the old filename.
 * @param const BY_HANDLE_FILE_INFORMATION& the file information we are comparing.
 * @return bool if the file has changed at all or not.
 */
bool HasFileInformationChanged(const wchar_t* file, const BY_HANDLE_FILE_INFORMATION& info )
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
 * @see GetFileInformationByName( const wchar_t*, ... )
 * @param const wchar_t* the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName( const std::wstring& file, BY_HANDLE_FILE_INFORMATION& info )
{
  return GetFileInformationByName( file.c_str(), info );
}

/**
 * Get a file information given a file name
 * @see ::GetFileInformationByHandle ( ... )
 * @param const wchar_t* the file full path we are getting info on
 * @param BY_HANDLE_FILE_INFORMATION& the item information
 * @return bool if there was an error or not.
 */
bool GetFileInformationByName(const wchar_t* file, BY_HANDLE_FILE_INFORMATION& info )
{
  // expand the file.
  wchar_t* lpDest;
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
 * @return std::wstring the path of the current exe.
 */
const std::wstring GetAppPath( bool bAddtrailing /*=true*/)
{
  std::wstring sReturn = L"";
  auto lpBuffer = new wchar_t[ T_MAX_PATH ];
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
 * @param const wchar_t* the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @param bool add a trailing backslash or not.
 * @return std::wstring the directory of the file given
 */
std::wstring GetBaseFromFile
( 
  const std::wstring& stdPath, 
  bool bExpand /*= true*/,
  bool bAddTrailing /*= true*/
)
{
  return GetBaseFromFile( stdPath.c_str(), bExpand, bAddTrailing );
}

/**
 * Get the base path from the file path so "c:\somthing\whatever.txt" would return "c:\somthing\"
 * we don't check if the file exits as it might not be a real file.
 * @see GetFileName( ... ) for the file name
 * @param const wchar_t* the full path we are getting.
 * @param bool if we want to expand the string or not, used to prevent recursive calls.
 * @return std::wstring the directory of the file given
 */
std::wstring GetBaseFromFile
( 
  const wchar_t* lpPath,
  bool bExpand /*= true*/, 
  bool bAddTrailing /*= true*/ 
)
{
  // expand the full path
  // this is in case the user uses some weird environment variable that contains the file name in it.
  // reverse look for the first '\' or '/'
  wchar_t tmpDir[ T_MAX_PATH+1 ];
  memset( tmpDir, 0, T_MAX_PATH+1 );
  if( bExpand )
  {
    wchar_t* lpDest = nullptr;
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
  const wchar_t* p1 = _tcsrchr( tmpDir, '\\' );
  if( p1 != nullptr)
  {
    r1 = (int)(p1 - tmpDir + 1);
  }

  const wchar_t* p2 = _tcsrchr( tmpDir, '/' );
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

  std::wstring s = ( tmpDir );
  if( bAddTrailing )
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
 * @param std::wstring& container for the return value, unset in case of error. 
 * @param const std::wstring& the relative path we would like to get the absolute path from.
 * @param const std::wstring& the original path, if empty we will use the current directory as origin.
 *                            if the path is '.' or '.\xxx' the current path will be added to it. 
 * @return bool if we were able to get the absolute path, false if the given path is unrealistic/impossible.
 */
bool GetAbsolutePath( std::wstring& dest, const std::wstring& givenRelative, const std::wstring& givenOrigin )
{
  //
  // first do some house-keeping
  //

  // make copies.
  auto copyOfRelative = givenRelative;
  auto copyOfOrigin = givenOrigin;

  // clean them up
  _TrimDeadChars(copyOfRelative);
  _TrimDeadChars(copyOfOrigin);

  // clean them
  strings::Trim(copyOfRelative);
  strings::Trim(copyOfOrigin);

  // make sure that the directory separators are correct.
  copyOfRelative = strings::Replace(copyOfRelative, _T("/"), MYODD_FILE_SEPARATOR, false);
  copyOfOrigin = strings::Replace(copyOfOrigin, _T("/"), MYODD_FILE_SEPARATOR, false);

  // the origin must have a trailing backslash
  // makes it easier if we are adding the working directory.
  files::AddTrailingBackSlash(copyOfOrigin);

  // save the unexpanded variables.
  auto unExpandedCopyOfOrigin = copyOfOrigin;
  auto unExpandedCopyOfRelative = copyOfRelative;

  // expand the relative path.
  files::ExpandEnvironment(copyOfRelative, copyOfRelative);

  // assume we will not be using the origin, (the path given was good enough).
  auto useOrigin = false;

  std::wstring pathToEvaluate;

  // (?:\\\\[^.]|[a-zA-Z]:[\\\/])
  if (!regex::Regex2::Search(_T("(?:\\\\\\\\[^.]|[a-zA-Z]:[\\\\\\/])"), copyOfRelative))
  {
    // we will be using the origin hrere.
    useOrigin = true;

    // we need to expand the origine now.
    files::ExpandEnvironment(copyOfOrigin, copyOfOrigin);

    // there is a special case for the origin been '.\' or '.'
    // in this case we want to assume that the user wants the 'current directory'
    regex::Regex2::Matches matches;

    // if the origin has a '.' or './' then we need to add the current path to it.
    // that's an assumption made to make the life of the caller easier.
    // ^((?:\.[\/\\])+)
    const auto pattern = _T("^((?:\\.[\\/\\\\])+)");
    if (regex::Regex2::Match(pattern, copyOfOrigin, matches) > 1)
    {
      // we can replace the all the '././././' with the 
      // current directory even if the user just gave us a '.'
      
      // join the current directory and the origin without the '.' or './'.
      files::Join(copyOfOrigin, GetAppPath(true), copyOfOrigin.substr(matches[1].length()));
    }

    // we can now join the origin and relative path.
    files::Join(pathToEvaluate, copyOfOrigin, copyOfRelative);
  }
  else
  {
    pathToEvaluate = copyOfRelative;
  }

  // now we need to get the absolute path of both of them.
  std::vector<std::wstring> partsOfPathToEvaluate;
  strings::Explode(partsOfPathToEvaluate, pathToEvaluate, MYODD_FILE_SEPARATOR_C, MYODD_MAX_INT32, false);

  // now we need to know we we go forward or backward in our final solution.
  std::vector<std::wstring> evaluatedParts;
  for( auto it = partsOfPathToEvaluate.begin(); it != partsOfPathToEvaluate.end(); ++it )
  {
    if ( *it == _T("."))
    {
      //  this means nothing
      continue;
    }

    if (*it == _T(".."))
    {
      // before we go backward, can we go backward...
      if( evaluatedParts.size() == 0 )
      {
        // nope, we cannot go back onto ourselves.
        dest.clear();
        return false;
      }

      // go back one step
      evaluatedParts.pop_back();
      continue;
    }

    // this is neither a single nor double dot
    // so we can just add it to our list.
    // (this could be empty as well).
    evaluatedParts.push_back(*it);
  }

  // this is it... put it all back together now.
  dest = strings::implode(evaluatedParts, MYODD_FILE_SEPARATOR );

  const auto pattern_end = _T("[\\/\\\\]$");
  if ( regex::Regex2::Search(pattern_end, copyOfRelative, false) )
  {
    AddTrailingBackSlash(dest);
  }

  // now check if we need to unexpand the data.
  // this is where we use the before expand value.
  if (0 != strings::Compare(copyOfOrigin, unExpandedCopyOfOrigin, false) ||
     (0 != strings::Compare(copyOfRelative, unExpandedCopyOfRelative, false))
      )
  {
    files::UnExpandEnvironment(dest, dest);
  }

  // was the original file UNC?
  // or, if we did not use the original, was the relative path UNC
  if (useOrigin)
  {
    if (myodd::regex::Regex2::Search(_T("(?:\\\\{2})"), unExpandedCopyOfOrigin))
    {
      dest = _T("\\\\") + dest;
    }
  }
  else if (myodd::regex::Regex2::Search(_T("(?:\\\\{2})"), unExpandedCopyOfRelative))
  {
    dest = _T("\\\\") + dest;
  }

  // success!
  return true;
}

/**
 * Remove illegal characters in a file name.
 * Do not pass a full path name as '/' will be replaced.
 * @param std::wstring& the file been cleaned.
 * @return none
 */
void CleanFileName( std::wstring& dirtyFileName )
{
  // the bad characters.
  static auto badChars = _T("?[]/\\=+<>:;\",*|^");

  // the good characters.
  static auto goodChar = _T('_');

  // go around looking for a bad characters.
  for(;;)
  {
    // look for the first bad character
    auto found = dirtyFileName.find_first_of(badChars);

    // did we find it?
    if( std::wstring::npos == found )
    {
      break;
    }

    // then replace it.
    dirtyFileName[found] = goodChar;
  }
}

/**
 * Create a temp filename and return the FULL path
 * The file will be be unique
 * @param std::wstring& the return value.
 * @param const wchar_t* the prefix or nullptr if no prefix is needed.
 * @param const wchar_t* the prefix or nullptr if no extension is needed, (if nullptr '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName( std::wstring& stdFileName, const wchar_t* lpPrefix, const wchar_t* lpExt )
{
  // use the std::wstring function.
  wchar_t* lpFileName = nullptr;
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
 * @param wchar_t*& the return value, the calling function will need to delete that value.
 * @param const wchar_t* the prefix or nullptr if no prefix is needed.
 * @param const wchar_t* the prefix or nullptr if no extension is needed, (if nullptr '.tmp' will be added).
 * @return bool success or not if there was an error
 */
bool GetFullTempFileName(wchar_t*& lpFileName, const wchar_t* lpPrefix, const wchar_t* lpExt )
{
  // add a unique, random number.
  int unique_num = math::random_range( 0, 65535 );

  // create the return file
  wchar_t* lpTmpFile = new TCHAR[T_MAX_PATH];
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
 * @param std::wstring& the return buffer that will contain the file+full path.
 * @param const wchar_t* the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName( std::wstring& stdFullPathFileName, const wchar_t* lpFileName )
{
  wchar_t* lpFullPathFileName = nullptr;
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
 * @param wchar_t*& the return buffer that will contain the file+full path.
 * @param const wchar_t* the given filename.
 * @return bool success or not
 */
bool GetFullTempFileName(wchar_t*& lpFullPathFileName, const wchar_t* lpFileName )
{
  // get the temp path
  wchar_t* lpTmpBuffer = new TCHAR[T_MAX_PATH];
  unsigned long dwLen = GetTempPath( T_MAX_PATH, lpTmpBuffer );
  if( 0 == dwLen )
  {
    // clean the temp path.
    delete [] lpTmpBuffer;

    //  could not create the temp path.
    return false;
  }

  // create the return buffer.
  lpFullPathFileName = new wchar_t[ T_MAX_PATH + 1];
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
* Get the filename from a full path, so "c:\somthing\whatever.txt" would return "whatever.txt"
* we don't check if the file exits as it might not be a real file.
* @see GetBaseFromFile( ... )
* @param const std::wstring& givenPath the full path we are getting.
* @param bool if we want to expand the string or not, used to prevent recursive calls.
* @return std::wstring the filename of the file given without the directory
*/
std::wstring GetFileName( const std::wstring& givenPath, bool bExpand /*= true*/ )
{
  // we want to expand the environment variable.
  // this is in case the user uses some weird environment variable that contains the file name in it.
  std::wstring copyOfGivenPath;
  if( bExpand )
  {
    //  try and expand it.
    if( !files::ExpandEnvironment(givenPath, copyOfGivenPath))
    {
      return _T("");
    }
  }
  else
  {
    // not expanding so we are using what was given
    copyOfGivenPath = givenPath;
  }
  // make sure that we use only '\'
  copyOfGivenPath = strings::Replace(copyOfGivenPath, _T("/"), _T("\\"), false);

  // now go back and create the parent directory.
  auto pos = copyOfGivenPath.find_last_of(_T('\\'));
  return (pos != std::wstring::npos ? copyOfGivenPath.substr(pos + 1) : L"");
}

/**
 * Get the file encoding
 * @param const wchar_t* the file we are checking.
 * @param int* if not nullptr we will set the number of characters to skip
 * @return FileEncode the file encoding.
 */
FileEncode DetermineEncoding(const wchar_t* file, int* npSkip )
{
  if( npSkip )
    *npSkip = 0;
  if(nullptr == file )
  {
    assert( 0 ); //  what are we trying to do?
    return uniError;
  }

  FILE* fp = 0;
  errno_t err = _wfopen_s( &fp, file, L"rb" );
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
 * @param const wchar_t* the file we want to read.
 * @param __int64 the start position we want to read from
 * @param __int64 the end position we want to read to.
 * @return wchar_t* the data or nullptr if there was an error.
 */
wchar_t* ReadFile(const wchar_t* file, __int64 nStartPos, __int64 nEndPos )
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
  errno_t err = _wfopen_s( &fp, file, L"rb");
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
  wchar_t* tbuf_ = nullptr;
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

void Join( std::wstring& returnPath, const std::wstring& pathPartA, const std::wstring& pathPartB )
{
  std::wstring partA = pathPartA;
  std::wstring partB = pathPartB;

  // make sure we have a trailing backslash
  AddTrailingBackSlash( partA );

  // and make sure that we don't have a leading one.
  RemoveLeadingBackSlash( partB );
  
  // join the two together.
  returnPath = partA + partB;
}

// --------------------------------------------------------------------------------------------------
Version::Version(const wchar_t* lpFileName /*= nullptr*/ )
{
  if( lpFileName )
  {
    DetermineFileVersion( lpFileName );
  }
  else
  {
    wchar_t	strTempBuffer[ MAX_PATH ];
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
 * @param const wchar_t* the filename of the file whose version we want.
 * @return none
 */
void Version::DetermineFileVersion(const wchar_t* lpFileName )
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
 * @param const wchar_t* the name of the file
 * @param std::vector<std::wstring>& the various keys.
 * @param const wchar_t* the app name, nullptr if we want all the keys.
 * @param const wchar_t* | nullptr the wild search to limit certain keys only or nullptr to get all the keys.
 * @return std::vector<std::wstring> list of all the keys or some that match the seatch.
 */
size_t GetKeys
( 
  const wchar_t* lpFileName,
  std::vector<std::wstring>& tokens,
  const wchar_t* lpAppName,
  const wchar_t* lpWild /*=nullptr*/
)
{
  tokens.clear();

  //  get all the keys
  unsigned long keys_size = T_MAX_PATH;
  while( true )
  {
    wchar_t* keys = new TCHAR[ keys_size ];
    auto nLen = ::GetPrivateProfileString( lpAppName, nullptr, _T(""), keys, keys_size, lpFileName );
    if( (keys_size-2) == nLen )
    {
      keys_size+=T_MAX_PATH;
      delete [] keys;
    }
    else
    {
      //  get the tokens.
      std::vector<std::wstring> tokensTmp;
      strings::explode_by_null_char( tokensTmp, keys, nLen );
      for( std::vector<std::wstring>::const_iterator it = tokensTmp.begin(); 
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
 * @return wchar_t* the converted file.
 */
wchar_t* Byte2Char
( 
  const char* buf, 
  size_t len, 
  FileEncode fileEncoding 
)
{
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

      // convert to wchar_t
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

  const auto& length = convertedString.length();
  auto tbuf = new wchar_t[ length + sizeof(wchar_t)];
  lstrcpy(tbuf, convertedString.c_str());

  return tbuf;
}

/** 
 * Get the size of a file
 * We return -1 if the file does not exist, we do not have permission and/or there was some other error.
 * @param const std::wstring& stdFullPathFileName the file we are checking.
 * @return long Total size, in bytes
 */
long GetFileSizeInBytes( const std::wstring& stdFullPathFileName )
{
  auto cleannedFileName = stdFullPathFileName;
  if (!ExpandEnvironment(cleannedFileName, cleannedFileName))
  {
    return -1;
  }

  // http://linux.die.net/man/2/stat
  // https://msdn.microsoft.com/en-us/library/14h5k7ff(VS.71).aspx
  long fileSize = -1;
  struct _stat64i32 filestatus;
  if (-1 == _wstat(cleannedFileName.c_str(), &filestatus))
  {
    return fileSize;
  }
  fileSize = static_cast<long>(filestatus.st_size);
  return static_cast<long>(filestatus.st_size);
}

} //  files
} //  myodd