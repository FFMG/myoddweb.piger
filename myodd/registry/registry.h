// Registry.h: interface for the Offset class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _Registry_h
#define _Registry_h

#pragma once
#include "../common/includes.h"

namespace myodd{ namespace reg{
  bool DeleteFullPath(const MYODD_CHAR* lpSubKey, const MYODD_CHAR* section, HKEY hkey );

  bool SaveStringFullPath(const MYODD_CHAR* lpSubKey, const MYODD_CHAR* section, LPCTSTR newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
  bool SaveDWORDFullPath(const MYODD_CHAR* lpSubKey, const MYODD_CHAR* section, unsigned long newVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL);

  bool LoadStringFullPath(const MYODD_CHAR* lpSubKey, const MYODD_CHAR* section, MYODD_STRING &oldVal, HKEY hkey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
  bool LoadDWORDFullPath(const MYODD_CHAR* lpSubKey, const MYODD_CHAR* section, unsigned long &oldVal, HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, unsigned long *dwRet = NULL );
} //  reg
} //  myodd
#endif // _Reg_h
