// LoaderPlugin.h : main header file for the LoaderPlugin DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "LoaderManager.h"

// -----------------------------------------------------------------------------------
class CLoaderPluginApp : public CWinApp
{
public:
	CLoaderPluginApp();
  virtual ~CLoaderPluginApp();

  // plugin functions
  void InitPlugin( amplugin* p );
  bool DestroyPlugIn( );
  bool Main( amplugin* p );

  void SetPluginPath( LPCWSTR lpPath );
// Overrides
public:
	virtual BOOL InitInstance();
  
	DECLARE_MESSAGE_MAP()

protected:
  std::wstring m_pluginPath;
  LoaderManager* m_loadManager;
};
