// files.h: interface for the Actions class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __File_h__
#define __Files_h__

#pragma once

#include "../common/includes.h"
#include <windows.h>
#include <vector>

namespace myodd{ namespace files{
void Test();  //  test data

// remove the file extension
void StripExtension( std::wstring& filename );
void AddExtension( std::wstring& filename, const std::wstring& extension, bool strip_current_if_exists );

std::wstring GetExtension( const std::wstring& filename );
bool IsExtension(const std::wstring& fOriginal, const std::wstring& fExt);

// remove a leading back slash
void RemoveLeadingBackSlash( std::wstring& szPath);

// remove a trialling back slash
void RemoveTrailingBackSlash( std::wstring& szPath);

// add a trialling backlash
void AddTrailingBackSlash( std::wstring& subPath );

// expand the environment variables.
bool ExpandEnvironment(const MYODD_CHAR* src, MYODD_CHAR*& dest );
bool ExpandEnvironment( const std::wstring& src, std::wstring& dest );

// un-expand the path and replace environment variables.
bool UnExpandEnvironment(const MYODD_CHAR* src, MYODD_CHAR*& dest );
bool UnExpandEnvironment( const std::wstring& src, std::wstring& dest );

// create a full path variable.
bool CreateFullDirectory(const MYODD_CHAR* c, bool bIsFile );
bool CreateFullDirectory( const std::wstring& c, bool bIsFile );

bool IsDirectory( const std::wstring& givenDirectory );
bool IsURL( const std::wstring& givenUrl );
bool IsDot( const std::wstring& givenFile );
bool IsFile( const std::wstring& givenFile );

bool FileExists( const std::wstring& c );
bool FileExists(const MYODD_CHAR* c );

bool DirectoryExists( const std::wstring& c );
bool DirectoryExists( const MYODD_CHAR* c );

bool DeleteFile(const MYODD_CHAR* c );
bool DeleteFile( const std::wstring& c );

bool CopyFile( const std::wstring& lpExistingFileName, const std::wstring& lpNewFileName, unsigned long* dwErr = 0 );
bool CopyFile(const MYODD_CHAR* lpExistingFileName, const MYODD_CHAR* lpNewFileName, unsigned long* dwErr = 0 );

bool HasFileInformationChanged( const std::wstring& file, const BY_HANDLE_FILE_INFORMATION& info );
bool HasFileInformationChanged(const MYODD_CHAR* file, const BY_HANDLE_FILE_INFORMATION& info );

bool GetFileInformationByName( const std::wstring& file, BY_HANDLE_FILE_INFORMATION& info );
bool GetFileInformationByName(const MYODD_CHAR* file, BY_HANDLE_FILE_INFORMATION& info );

std::wstring GetFileName( const std::wstring& givenPath, bool bExpand = true );

std::wstring GetBaseFromFile(const MYODD_CHAR* lpPath, bool bExpand = true, bool bAddTrailing = true );
std::wstring GetBaseFromFile( const std::wstring& stdPath, bool bExpand = true, bool bAddTrailing = true );

std::wstring GetAppPath( bool bAddTrailing =true);

bool GetAbsolutePath( std::wstring& dest, const std::wstring& givenRelative, const std::wstring& givenOrigin );

void CleanFileName( std::wstring& dirtyFileName );

bool GetFullTempFileName( std::wstring& stdFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt );
bool GetFullTempFileName(MYODD_CHAR*& lpFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt );

bool GetFullTempFileName( std::wstring& stdFullPathFileName, const MYODD_CHAR* lpFileName );
bool GetFullTempFileName(MYODD_CHAR*& lpFullPathFileName, const MYODD_CHAR* lpFileName );

long GetFileSizeInBytes( const std::wstring& stdFullPathFileName);

size_t GetKeys
( 
  const MYODD_CHAR* lpFileName,
  std::vector<std::wstring>& tokens,
  const MYODD_CHAR* lpAppName,
  const MYODD_CHAR* lpWild = nullptr
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

FileEncode DetermineEncoding(const MYODD_CHAR* file, int* npSkip = nullptr );
MYODD_CHAR* ReadFile(const MYODD_CHAR* file, __int64 nStartPos = -1, __int64 nEndPos = -1 );

MYODD_CHAR* Byte2Char( const char* buf, size_t len, FileEncode fileEncoding );

void Join( std::wstring& returnPath, const std::wstring& pathPartA, const std::wstring& pathPartB );

// get the version of a given file.
class Version
{
public:
  Version(const MYODD_CHAR* lpFileName = nullptr);
  virtual ~Version();

  unsigned short GetFileVersionMajor()        const{ return m_fileVersionMajor;}
  unsigned short GetFileVersionMinor()        const{ return m_fileVersionMinor;}
  unsigned short GetFileVersionMaintenance()  const{ return m_fileVersionMaintenance;}
  unsigned short GetFileVersionBuild()        const{ return m_fileVersionBuild;};
  const SYSTEMTIME& GetFileSystemTime()       const{ return m_fileSystemFime;};

private:
  void DetermineFileVersion(const MYODD_CHAR* lpFileName );

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