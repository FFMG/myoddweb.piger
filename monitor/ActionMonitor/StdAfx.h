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
#pragma comment(lib, "hook.lib" )

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>               // MFC core and standard components
#include <afxext.h>               // MFC extensions

//  the Dll hooks
#include "../hook/hook.h"

//  the config files && path.
#ifdef _DEBUG
static const TCHAR CONF_FULLPATH[]	= _T("%appdata%\\MyOddWeb\\ActionMonitor_d\\profile.xml");
#else
static const TCHAR CONF_FULLPATH[]	= _T("%appdata%\\MyOddWeb\\ActionMonitor\\profile.xml");
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

// this is the various API we are using
// remove/comment if we are not using it.
#define ACTIONMONITOR_API_LUA
// #define ACTIONMONITOR_API_PY
#define ACTIONMONITOR_API_PLUGIN

#ifdef ACTIONMONITOR_API_LUA
#   include "../api/luavirtualmachine.h"
#endif

#ifdef ACTIONMONITOR_API_PY
#   include "../api/pythonvirtualmachine.h"
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
#   include "../api/pluginvirtualmachine.h"
#endif
