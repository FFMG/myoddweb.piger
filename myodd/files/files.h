// files.h: interface for the Actions class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __File_h__
#define __Files_h__

#pragma once

#include "../common/includes.h"
#include <vector>

namespace myodd{ namespace files{
void Test();  //  test data

// remove the file extension
void strip_extension( MYODD_STRING& f );
void add_extension( MYODD_STRING& f, const MYODD_STRING& e, bool strip_current_if_exists );

MYODD_STRING get_extension( const MYODD_STRING& f );
bool is_extension( const MYODD_STRING& f, const MYODD_STRING& e );

// remove a leading back slash
void RemoveLeadingBackSlash( MYODD_STRING& szPath);
void RemoveLeadingBackSlash
(
  LPTSTR szPath,	      // pointer to buffer to receive module path
  unsigned int nSize		// size of buffer, in characters
);

// remove a trialling back slash
void RemoveTrailingBackSlash( MYODD_STRING& szPath);
void RemoveTrailingBackSlash
(
  LPTSTR szPath,	      // pointer to buffer to receive module path
  unsigned int nSize		// size of buffer, in characters
);

// add a trialling backlash
void AddTrailingBackSlash( MYODD_STRING& subPath );
void AddTrailingBackSlash
(
  LPTSTR szPath,	    // pointer to buffer to receive module path
  unsigned int nSize	// size of buffer, in characters
);

// expand the environment variables.
bool ExpandEnvironment( LPCTSTR src, LPTSTR& dest );
bool ExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest );

// un-expand the path and replace environment variables.
bool UnExpandEnvironment( LPCTSTR src, LPTSTR& dest );
bool UnExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest );

// create a full path variable.
bool CreateFullDirectory( LPCTSTR c, bool bIsFile );
bool CreateFullDirectory( const MYODD_STRING& c, bool bIsFile );

bool IsFile( LPCTSTR lp );
bool IsFile(const MYODD_STRING& stdFile);

bool IsDirectory( LPCTSTR lp );
bool IsDirectory( const MYODD_STRING& stdDir );

bool IsURL( LPCTSTR lp );
bool IsURL(const MYODD_STRING& stdUrl );

bool FileExists( const MYODD_STRING& c );
bool FileExists( LPCTSTR c );

bool DirectoryExists( const MYODD_STRING& c );
bool DirectoryExists( LPCTSTR c );

bool DeleteFile( LPCTSTR c );
bool DeleteFile( const MYODD_STRING& c );

bool CopyFile( const MYODD_STRING& lpExistingFileName, const MYODD_STRING& lpNewFileName, DWORD* dwErr = 0 ); 
bool CopyFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD* dwErr = 0 );

bool HasFileInformationChanged( const MYODD_STRING& file, const BY_HANDLE_FILE_INFORMATION& info );
bool HasFileInformationChanged( LPCTSTR file, const BY_HANDLE_FILE_INFORMATION& info );

bool GetFileInformationByName( const MYODD_STRING& file, BY_HANDLE_FILE_INFORMATION& info );
bool GetFileInformationByName( LPCTSTR file, BY_HANDLE_FILE_INFORMATION& info );

MYODD_STRING GetFileName( LPCTSTR lpPath, bool bExpand = true );
MYODD_STRING GetFileName( const MYODD_STRING& stdPath, bool bExpand = true );

MYODD_STRING GetBaseFromFile( LPCTSTR lpPath, bool bExpand = true, bool bAddTrailling = true );
MYODD_STRING GetBaseFromFile( const MYODD_STRING& stdPath, bool bExpand = true, bool bAddTrailling = true );

MYODD_STRING GetAppPath( bool bAddTrailling =true);

bool GetAbsolutePath( LPTSTR& dest, LPCTSTR lpRelative, LPCTSTR lpOrigin = NULL );
bool GetAbsolutePath( MYODD_STRING& dest, const MYODD_STRING& lpRelative, LPCTSTR lpOrigin = NULL );

void CleanFileName( MYODD_STRING& dirtyFileName );

bool GetFullTempFileName( MYODD_STRING& stdFileName, LPCTSTR lpPrefix, LPCTSTR lpExt );
bool GetFullTempFileName( LPTSTR& lpFileName, LPCTSTR lpPrefix, LPCTSTR lpExt );

bool GetFullTempFileName( MYODD_STRING& stdFullPathFileName, LPCTSTR lpFileName );
bool GetFullTempFileName( LPTSTR& lpFullPathFileName, LPCTSTR lpFileName );

bool is_dot( const MYODD_STRING& stdFile );
bool is_dot( LPCTSTR lpFile );

long GetFileSizeInBytes( const MYODD_STRING& stdFullPathFileName);

size_t GetKeys
( 
  LPCTSTR lpFileName, 
  std::vector<MYODD_STRING>& tokens,
  LPCTSTR lpAppName,
  LPCTSTR lpWild =NULL
);

enum FileEncode {
  uniError=-1,
  uni8Bit=0, 
  uniUTF8,
  uniUTF8NoBOM,
  uni16BE, 
  uni16LE,
  uni16LE_NoBOM,

  uniEnd
};

FileEncode DetermineEncoding( LPCTSTR file, int* npSkip = NULL );
TCHAR* ReadFile( LPCTSTR file, __int64 nStartPos = -1, __int64 nEndPos = -1 );

TCHAR* Byte2Char( const char* buf, size_t len, FileEncode fileEncoding );

void Join( MYODD_STRING& returnPath, const MYODD_STRING& pathPartA, const MYODD_STRING& pathPartB );

// get the version of a given file.
class Version
{
public:
  Version( LPCTSTR lpFileName = NULL );
  virtual ~Version();

  unsigned short GetFileVersionMajor()        const{ return m_fileVersionMajor;}
  unsigned short GetFileVersionMinor()        const{ return m_fileVersionMinor;}
  unsigned short GetFileVersionMaintenance()  const{ return m_fileVersionMaintenance;}
  unsigned short GetFileVersionBuild()        const{ return m_fileVersionBuild;};
  const SYSTEMTIME& GetFileSystemTime()       const{ return m_fileSystemFime;};

private:
  void DetermineFileVersion( LPCTSTR lpFileName );

  unsigned short m_fileVersionMajor;
  unsigned short m_fileVersionMinor;
  unsigned short m_fileVersionMaintenance;
  unsigned short m_fileVersionBuild;

  SYSTEMTIME m_fileSystemFime;

  // prevent copy/assignment
  Version( const Version&  );
  void operator=( const Version& );
};
} //  files
} //  myodd
#endif