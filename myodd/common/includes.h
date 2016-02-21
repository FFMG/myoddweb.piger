#ifndef __Includes_h__
#define __Includes_h__

#pragma once

#include <windows.h>
#include <vector>
#include <string>

#include <map>
#include <windows.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlconv.h>

#ifndef STD_TSTRING
#   ifdef _UNICODE
      typedef std::wstring STD_TSTRING;
#   else
      typedef std::string STD_TSTRING;
#   endif
#endif  //  STD_TSTRING

#endif  //  __Includes_h__