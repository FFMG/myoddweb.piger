// Dolly.h : main header file for the Dolly DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// support for PluginAPI
#include "../amplugin.h"

// add the libs
#ifdef _DEBUG
# ifdef _WIN64
#   pragma comment(lib, "../amplugin64d.lib" )
#else
#   pragma comment(lib, "../amplugind.lib" )
# endif
#else
# ifdef _WIN64
#   pragma comment(lib, "../amplugin64.lib" )
# else
#   pragma comment(lib, "../amplugin.lib" )
# endif
#endif
