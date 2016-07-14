#pragma once

/** 
 * We do not want any warnings for deprecated warnings if we are in windows.
 */
#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
  #define _CRT_SECURE_NO_WARNINGS
#endif

// define lparam/waprams
#ifdef _WIN64
  typedef __int64 MYODD_LPARAM;
  typedef unsigned __int64 MYODD_WPARAM;
#else
  typedef long MYODD_LPARAM;
  typedef unsigned int MYODD_WPARAM;
#endif

// Min/Max 32bit int 
#define MYODD_MAX_INT32 2147483647
#define MYODD_MIN_INT32 -2147483648 

// define the strings
#include <string>
#ifndef MYODD_STRING
  #ifdef _UNICODE
    typedef std::wstring MYODD_STRING;
    typedef wchar_t MYODD_CHAR;
  #else
    typedef std::string MYODD_STRING;
    typedef char MYODD_CHAR;
  #endif
#endif  //  MYODD_STRING

#ifndef __AFXWIN_H__


#ifdef _DEBUG
  #define UNUSED(x)
#else
  #define UNUSED(x) x
#endif
#define UNUSED_ALWAYS(x) x

#include <assert.h>
#ifdef _DEBUG
  #define ASSERT(f) assert(f)
#else
  #define ASSERT(f) __noop
#endif

#endif  //  __AFXWIN_H__
