#ifndef __Includes_h__
#define __Includes_h__

#pragma once

#include <windows.h>
#include <string>

#ifdef _WIN64
  typedef __int64 MYODD_LPARAM;
  typedef unsigned __int64 MYODD_WPARAM;
#else
  typedef long MYODD_LPARAM;
  typedef unsigned int MYODD_WPARAM;
#endif

#ifndef MYODD_STRING
  #ifdef _UNICODE
    typedef std::wstring MYODD_STRING;
    typedef wchar_t MYODD_CHAR;
  #else
    typedef std::string MYODD_STRING;
    typedef char MYODD_CHAR;
  #endif
#endif  //  MYODD_STRING

#endif  //  __Includes_h__