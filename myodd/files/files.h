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
bool ExpandEnvironment(const wchar_t* src, wchar_t*& dest );
bool ExpandEnvironment( const std::wstring& src, std::wstring& dest );

// un-expand the path and replace environment variables.
bool UnExpandEnvironment(const wchar_t* src, wchar_t*& dest );
bool UnExpandEnvironment( const std::wstring& src, std::wstring& dest );

// create a full path variable.
bool CreateFullDirectory(const wchar_t* c, bool bIsFile );
bool CreateFullDirectory( const std::wstring& c, bool bIsFile );

bool IsDirectory( const std::wstring& givenDirectory );
bool IsURL( const std::wstring& givenUrl );
bool IsDot( const std::wstring& givenFile );
bool IsFile( const std::wstring& givenFile );

bool FileExists( const std::wstring& c );
bool FileExists(const wchar_t* c );

bool DirectoryExists( const std::wstring& c );
bool DirectoryExists( const wchar_t* c );

bool DeleteFile(const wchar_t* c );
bool DeleteFile( const std::wstring& c );

bool CopyFile( const std::wstring& lpExistingFileName, const std::wstring& lpNewFileName, unsigned long* dwErr = 0 );
bool CopyFile(const wchar_t* lpExistingFileName, const wchar_t* lpNewFileName, unsigned long* dwErr = 0 );

bool HasFileInformationChanged( const std::wstring& file, const BY_HANDLE_FILE_INFORMATION& info );
bool HasFileInformationChanged(const wchar_t* file, const BY_HANDLE_FILE_INFORMATION& info );

bool GetFileInformationByName( const std::wstring& file, BY_HANDLE_FILE_INFORMATION& info );
bool GetFileInformationByName(const wchar_t* file, BY_HANDLE_FILE_INFORMATION& info );

std::wstring GetFileName( const std::wstring& givenPath, bool bExpand = true );

std::wstring GetBaseFromFile(const wchar_t* lpPath, bool bExpand = true, bool bAddTrailing = true );
std::wstring GetBaseFromFile( const std::wstring& stdPath, bool bExpand = true, bool bAddTrailing = true );

const std::wstring& GetAppPath( bool bAddTrailing =true) const;

bool GetAbsolutePath( std::wstring& dest, const std::wstring& givenRelative, const std::wstring& givenOrigin );

void CleanFileName( std::wstring& dirtyFileName );

bool GetFullTempFileName( std::wstring& stdFileName, const wchar_t* lpPrefix, const wchar_t* lpExt );
bool GetFullTempFileName(wchar_t*& lpFileName, const wchar_t* lpPrefix, const wchar_t* lpExt );

bool GetFullTempFileName( std::wstring& stdFullPathFileName, const wchar_t* lpFileName );
bool GetFullTempFileName(wchar_t*& lpFullPathFileName, const wchar_t* lpFileName );

long GetFileSizeInBytes( const std::wstring& stdFullPathFileName);

size_t GetKeys
( 
  const wchar_t* lpFileName,
  std::vector<std::wstring>& tokens,
  const wchar_t* lpAppName,
  const wchar_t* lpWild = nullptr
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

FileEncode DetermineEncoding(const wchar_t* file, int* npSkip = nullptr );
wchar_t* ReadFile(const wchar_t* file, __int64 nStartPos = -1, __int64 nEndPos = -1 );

wchar_t* Byte2Char( const char* buf, size_t len, FileEncode fileEncoding );

void Join( std::wstring& returnPath, const std::wstring& pathPartA, const std::wstring& pathPartB );

// get the version of a given file.
class Version
{
public:
  Version(const wchar_t* lpFileName = nullptr);
  virtual ~Version();

  unsigned short GetFileVersionMajor()        const{ return m_fileVersionMajor;}
  unsigned short GetFileVersionMinor()        const{ return m_fileVersionMinor;}
  unsigned short GetFileVersionMaintenance()  const{ return m_fileVersionMaintenance;}
  unsigned short GetFileVersionBuild()        const{ return m_fileVersionBuild;};
  const SYSTEMTIME& GetFileSystemTime()       const{ return m_fileSystemFime;};

private:
  void DetermineFileVersion(const wchar_t* lpFileName );

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