// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#ifndef _SECURE_ATL
#   define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#   define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

// and the hook lib
#ifdef _WIN64
# pragma comment(lib, "hook64.lib" )
#else
# pragma comment(lib, "hook.lib" )
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>               // MFC core and standard components
#include <afxext.h>               // MFC extensions

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//  the Dll hooks
#include "../hook/hook.h"

//  the config files && path.
#ifdef _DEBUG
static const TCHAR CONF_FULLPATH[]	= _T("%appdata%\\MyOddWeb\\ActionMonitor_d\\profile.xml");
static const TCHAR LOG_PATH[] = _T("%appdata%\\MyOddWeb\\ActionMonitor_d\\Logs\\");
#else
static const TCHAR CONF_FULLPATH[]	= _T("%appdata%\\MyOddWeb\\ActionMonitor\\profile.xml");
static const TCHAR LOG_PATH[] = _T("%appdata%\\MyOddWeb\\ActionMonitor\\Logs\\");
#endif

static const TCHAR CONF_MUTEXT[]	  = _T("MyOddweb_com_ActionMonitor"); //  if you are going to change it
                                                                        //  remember to change it in the InnoSetup as well.

#ifndef _AFX_NO_OLE_SUPPORT
#   include <afxdtctl.h>		      // MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#   include <afxcmn.h>			      // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdlgs.h>

//
// Include all the common files.
#include <myoddinclude.h>

#include "../api/plugins.h"