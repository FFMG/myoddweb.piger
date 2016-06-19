#ifndef __Includes_h__
#define __Includes_h__

#pragma once

#include <windows.h>
// #include <atlbase.h>
// #include <atlconv.h>

#include <string>

#ifndef MYODD_STRING
#   ifdef _UNICODE
      typedef std::wstring MYODD_STRING;
#   else
      typedef std::string MYODD_STRING;
#   endif
#endif  //  MYODD_STRING

#endif  //  __Includes_h__