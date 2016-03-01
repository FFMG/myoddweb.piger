// ActionMonitor.h : main header file for the ACTIONMONITOR application
//

#pragma once

#include "Actions.h"    //  the actions we can call, (the name of the file)
#include "Action.h"     //  the action we are currently working with

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		          // main symbols

/////////////////////////////////////////////////////////////////////////////
// CActionMonitorApp:
// See ActionMonitor.cpp for the implementation of this class
//

#define _MAX_CMD_LINE_ARGS  128

class CActionMonitorApp : public CWinApp
{
public:
	CActionMonitorApp();
  virtual ~CActionMonitorApp();
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	DECLARE_MESSAGE_MAP()

protected:
  void InitConfig();
  void InitReservedPaths();

public:
  static CWnd* GetLastForegroundWindow();
  static void SetLastForegroundWindow( CWnd *w);

public:
  void BuildActionsList();

public:
  void DoStartActionsList();
  void DoEndActionsList();

public:
//  virtual int ExitInstance();
  virtual int ExitInstance();

  // check Mutext and so on
  bool CanStartApp();

  // self elevate us to admin.
  void SelfElavate();

  // the handle of the mutex
  HANDLE m_hMutex; 

public:
  const Actions& PossibleActions() const {
    return *_possibleActions;
  }
  Actions& PossibleActions() {
    return *_possibleActions;
  }
private:
  Actions* _possibleActions;

#ifdef ACTIONMONITOR_API_LUA
protected:
  LuaVirtualMachine* _lvm;

public:
  LuaVirtualMachine* GetLuaVirtualMachine();
#endif

#ifdef ACTIONMONITOR_API_PY
protected:
  PythonVirtualMachine* _pvm;

public:
  PythonVirtualMachine* GetPythonVirtualMachine();
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
protected:
  PluginVirtualMachine* _plugvm;

public:
  PluginVirtualMachine* GetPluginVirtualMachine();
#endif
};

CActionMonitorApp& App();

/////////////////////////////////////////////////////////////////////////////
