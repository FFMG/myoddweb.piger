// ActionMonitor.h : main header file for the ACTIONMONITOR application
//

#pragma once

#include "Actions.h"    //  the actions we can call, (the name of the file)

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

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
  static bool InitConfig(const myodd::variables& vm);
  static void InitLog();
  static void InitReservedPaths();
  void InitMaxClipboardSize();

public:
  static CWnd* GetLastForegroundWindow();
  static void SetLastForegroundWindow( CWnd *w);

  static size_t GetMaxClipboardMemory();

public:
  void BuildActionsList();

public:
  static void DoStartActionsList();
  static void DoEndActionsList();

public:
//  virtual int ExitInstance();
  int ExitInstance() override;

  // check Mutext and so on
  bool CanStartApp();

  // self elevate us to admin.
  static void SelfElavate();

  // the handle of the mutex
  HANDLE m_hMutex; 

  /**
   * @param CWnd* The window that last had the focus when we pressed the special key.
   */
  CWnd* _cwndLastForegroundWindow;

  /**
   * @param size_t the maximum memory we want to use when getting clipboard data.
   */
  size_t _maxClipboardSize;
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

#ifdef ACTIONMONITOR_PS_PLUGIN
protected:
  PowershellVirtualMachine* _psvm;

public:
  PowershellVirtualMachine* GetPowershellVirtualMachine();
#endif

  void DestroyActiveActions();
};

CActionMonitorApp& App();

/////////////////////////////////////////////////////////////////////////////
