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
void StripExtension( MYODD_STRING& filename );
void AddExtension( MYODD_STRING& filename, const MYODD_STRING& extension, bool strip_current_if_exists );

MYODD_STRING GetExtension( const MYODD_STRING& filename );
bool IsExtension(const MYODD_STRING& fOriginal, const MYODD_STRING& fExt);

// remove a leading back slash
void RemoveLeadingBackSlash( MYODD_STRING& szPath);

// remove a trialling back slash
void RemoveTrailingBackSlash( MYODD_STRING& szPath);

// add a trialling backlash
void AddTrailingBackSlash( MYODD_STRING& subPath );

// expand the environment variables.
bool ExpandEnvironment(const MYODD_CHAR* src, MYODD_CHAR*& dest );
bool ExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest );

// un-expand the path and replace environment variables.
bool UnExpandEnvironment(const MYODD_CHAR* src, MYODD_CHAR*& dest );
bool UnExpandEnvironment( const MYODD_STRING& src, MYODD_STRING& dest );

// create a full path variable.
bool CreateFullDirectory(const MYODD_CHAR* c, bool bIsFile );
bool CreateFullDirectory( const MYODD_STRING& c, bool bIsFile );

bool IsFile(const MYODD_CHAR* lp );
bool IsFile(const MYODD_STRING& stdFile);

bool IsDirectory(const MYODD_CHAR* lp );
bool IsDirectory( const MYODD_STRING& stdDir );

bool IsURL(const MYODD_CHAR* lp );
bool IsURL(const MYODD_STRING& stdUrl );

bool IsDot(const MYODD_STRING& stdFile);
bool IsDot(const MYODD_CHAR* lpFile);

bool FileExists( const MYODD_STRING& c );
bool FileExists(const MYODD_CHAR* c );

bool DirectoryExists( const MYODD_STRING& c );
bool DirectoryExists( const MYODD_CHAR* c );

bool DeleteFile(const MYODD_CHAR* c );
bool DeleteFile( const MYODD_STRING& c );

bool CopyFile( const MYODD_STRING& lpExistingFileName, const MYODD_STRING& lpNewFileName, unsigned long* dwErr = 0 );
bool CopyFile(const MYODD_CHAR* lpExistingFileName, const MYODD_CHAR* lpNewFileName, unsigned long* dwErr = 0 );

bool HasFileInformationChanged( const MYODD_STRING& file, const BY_HANDLE_FILE_INFORMATION& info );
bool HasFileInformationChanged(const MYODD_CHAR* file, const BY_HANDLE_FILE_INFORMATION& info );

bool GetFileInformationByName( const MYODD_STRING& file, BY_HANDLE_FILE_INFORMATION& info );
bool GetFileInformationByName(const MYODD_CHAR* file, BY_HANDLE_FILE_INFORMATION& info );

MYODD_STRING GetFileName(const MYODD_CHAR* lpPath, bool bExpand = true );
MYODD_STRING GetFileName( const MYODD_STRING& stdPath, bool bExpand = true );

MYODD_STRING GetBaseFromFile(const MYODD_CHAR* lpPath, bool bExpand = true, bool bAddTrailing = true );
MYODD_STRING GetBaseFromFile( const MYODD_STRING& stdPath, bool bExpand = true, bool bAddTrailing = true );

MYODD_STRING GetAppPath( bool bAddTrailing =true);

bool GetAbsolutePath(MYODD_CHAR*& dest, const MYODD_CHAR* lpRelative, const MYODD_CHAR* lpOrigin = nullptr);
bool GetAbsolutePath( MYODD_STRING& dest, const MYODD_STRING& lpRelative, const MYODD_CHAR* lpOrigin = nullptr);

void CleanFileName( MYODD_STRING& dirtyFileName );

bool GetFullTempFileName( MYODD_STRING& stdFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt );
bool GetFullTempFileName(MYODD_CHAR*& lpFileName, const MYODD_CHAR* lpPrefix, const MYODD_CHAR* lpExt );

bool GetFullTempFileName( MYODD_STRING& stdFullPathFileName, const MYODD_CHAR* lpFileName );
bool GetFullTempFileName(MYODD_CHAR*& lpFullPathFileName, const MYODD_CHAR* lpFileName );

long GetFileSizeInBytes( const MYODD_STRING& stdFullPathFileName);

size_t GetKeys
( 
  const MYODD_CHAR* lpFileName,
  std::vector<MYODD_STRING>& tokens,
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

void Join( MYODD_STRING& returnPath, const MYODD_STRING& pathPartA, const MYODD_STRING& pathPartB );

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