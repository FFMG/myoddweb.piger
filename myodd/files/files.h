// files.h: interface for the Actions class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __File_h__
#define __Files_h__

#pragma once

#include "../common/includes.h"

namespace myodd{ namespace files{
void Test();  //  test data

// remove the file extension
void strip_extension( STD_TSTRING& f );
void add_extension( STD_TSTRING& f, const STD_TSTRING& e, bool strip_current_if_exists );


STD_TSTRING get_extension( const STD_TSTRING& f );
bool is_extension( const STD_TSTRING& f, const STD_TSTRING& e );

// remove a leading back slash
void RemoveLeadingBackSlash( STD_TSTRING& szPath);
void RemoveLeadingBackSlash
(
  LPTSTR szPath,	      // pointer to buffer to receive module path
  unsigned int nSize		// size of buffer, in characters
);

// remove a trialling back slash
void RemoveTrailingBackSlash( STD_TSTRING& szPath);
void RemoveTrailingBackSlash
(
  LPTSTR szPath,	      // pointer to buffer to receive module path
  unsigned int nSize		// size of buffer, in characters
);

// add a trialling backlash
void AddTrailingBackSlash( STD_TSTRING& subPath );
void AddTrailingBackSlash
(
  LPTSTR szPath,	    // pointer to buffer to receive module path
  unsigned int nSize	// size of buffer, in characters
);

// expand the environment variables.
bool ExpandEnvironment( LPCTSTR src, LPTSTR& dest );
bool ExpandEnvironment( const STD_TSTRING& src, STD_TSTRING& dest );

// un-expand the path and replace environment variables.
bool UnExpandEnvironment( LPCTSTR src, LPTSTR& dest );
bool UnExpandEnvironment( const STD_TSTRING& src, STD_TSTRING& dest );

// create a full path variable.
bool CreateFullDirectory( LPCTSTR c, bool bIsFile );
bool CreateFullDirectory( const STD_TSTRING& c, bool bIsFile );

bool IsFile( LPCTSTR lp );
bool IsDirectory( LPCTSTR lp );
bool IsDirectory( const STD_TSTRING& stdDir );
bool IsURL( LPCTSTR lp );

bool FileExists( const STD_TSTRING& c );
bool FileExists( LPCTSTR c );

bool DirectoryExists( const STD_TSTRING& c );
bool DirectoryExists( LPCTSTR c );

bool DeleteFile( LPCTSTR c );
bool DeleteFile( const STD_TSTRING& c );

bool CopyFile( const STD_TSTRING& lpExistingFileName, const STD_TSTRING& lpNewFileName, DWORD* dwErr = 0 ); 
bool CopyFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, DWORD* dwErr = 0 );

bool HasFileInformationChanged( const STD_TSTRING& file, const BY_HANDLE_FILE_INFORMATION& info );
bool HasFileInformationChanged( LPCTSTR file, const BY_HANDLE_FILE_INFORMATION& info );

bool GetFileInformationByName( const STD_TSTRING& file, BY_HANDLE_FILE_INFORMATION& info );
bool GetFileInformationByName( LPCTSTR file, BY_HANDLE_FILE_INFORMATION& info );

STD_TSTRING GetFileName( LPCTSTR lpPath, bool bExpand = true );
STD_TSTRING GetFileName( const STD_TSTRING& stdPath, bool bExpand = true );

STD_TSTRING GetBaseFromFile( LPCTSTR lpPath, bool bExpand = true, bool bAddTrailling = true );
STD_TSTRING GetBaseFromFile( const STD_TSTRING& stdPath, bool bExpand = true, bool bAddTrailling = true );

STD_TSTRING GetAppPath( bool bAddTrailling =true);

bool GetAbsolutePath( LPTSTR& dest, LPCTSTR lpRelative, LPCTSTR lpOrigin = NULL );
bool GetAbsolutePath( STD_TSTRING& dest, const STD_TSTRING& lpRelative, LPCTSTR lpOrigin = NULL );

void CleanFileName( STD_TSTRING& dirtyFileName );

bool GetFullTempFileName( STD_TSTRING& stdFileName, LPCTSTR lpPrefix, LPCTSTR lpExt );
bool GetFullTempFileName( LPTSTR& lpFileName, LPCTSTR lpPrefix, LPCTSTR lpExt );

bool GetFullTempFileName( STD_TSTRING& stdFullPathFileName, LPCTSTR lpFileName );
bool GetFullTempFileName( LPTSTR& lpFullPathFileName, LPCTSTR lpFileName );

bool is_dot( const STD_TSTRING& stdFile );
bool is_dot( LPCTSTR lpFile );

size_t GetKeys
( 
  LPCTSTR lpFileName, 
  std::vector<STD_TSTRING>& tokens,
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

void Join( STD_TSTRING& returnPath, const STD_TSTRING& pathPartA, const STD_TSTRING& pathPartB );

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