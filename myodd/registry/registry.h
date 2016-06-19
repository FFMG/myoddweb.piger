// Registry.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _Registry_h
#define _Registry_h

#pragma once
#include "../common/includes.h"

namespace myodd{ namespace reg{
  bool DeleteFullPath( LPCTSTR lpSubKey, LPCTSTR section, HKEY hkey );

  bool SaveStringFullPath( LPCTSTR lpSubKey, LPCTSTR section, LPCTSTR newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, DWORD *dwRet = NULL );
  bool SaveDWORDFullPath( LPCTSTR lpSubKey, LPCTSTR section, DWORD newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, DWORD *dwRet = NULL);

  bool LoadStringFullPath( LPCTSTR lpSubKey, LPCTSTR section, MYODD_STRING &oldVal, HKEY hkey /*= HKEY_LOCAL_MACHINE*/,  DWORD *dwRet = NULL );
  bool LoadDWORDFullPath( LPCTSTR lpSubKey, LPCTSTR section, DWORD &oldVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, DWORD *dwRet = NULL );
} //  reg
} //  myodd
#endif // _Reg_h
