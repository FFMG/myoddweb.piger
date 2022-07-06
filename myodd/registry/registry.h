// Registry.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _Registry_h
#define _Registry_h

#pragma once
#include "../common/includes.h"

namespace myodd{ namespace reg{
  bool DeleteFullPath(const wchar_t* lpSubKey, const wchar_t* section, HKEY hkey );

  bool SaveStringFullPath(const wchar_t* lpSubKey, const wchar_t* section, LPCTSTR newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
  bool SaveDWORDFullPath(const wchar_t* lpSubKey, const wchar_t* section, unsigned long newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL);

  bool LoadStringFullPath(const wchar_t* lpSubKey, const wchar_t* section, std::wstring &oldVal, HKEY hkey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
  bool LoadDWORDFullPath(const wchar_t* lpSubKey, const wchar_t* section, unsigned long &oldVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
} //  reg
} //  myodd
#endif // _Reg_h
