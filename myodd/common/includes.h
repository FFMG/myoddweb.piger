#ifndef __Includes_h__
#define __Includes_h__

#pragma once

#include <windows.h>
// #include <atlbase.h>
// #include <atlconv.h>

#include <string>

#ifndef STD_TSTRING
#   ifdef _UNICODE
      typedef std::wstring STD_TSTRING;
#   else
      typedef std::string STD_TSTRING;
#   endif
#endif  //  STD_TSTRING

#endif  //  __Includes_h__