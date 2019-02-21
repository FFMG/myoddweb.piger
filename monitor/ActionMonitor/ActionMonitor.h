//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once

#include "Actions.h"    //  the actions we can call, (the name of the file)
#include "ActionsImmediate.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionMonitorApp:
// See ActionMonitor.cpp for the implementation of this class
//

#define _MAX_CMD_LINE_ARGS  128

class IMessages;

class CActionMonitorApp final : public CWinApp
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
  void CreateActionsList();

  void CreateMessageHandler();

public:
  void DoStartActionsList( bool wait );
  void DoEndActionsList(bool wait);
  void WaitForEndActionsToComplete();
  void WaitForStartActionsToComplete();

private:
  ActionsImmediate* _startActions;
  ActionsImmediate* _endActions;

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

  const IMessages& MessageHandler() const {
    return *_messages;
  }
  IMessages& MessageHandler() {
    return *_messages;
  }
private:
  /**
   * \brief the messages handler
   */
  IMessages* _messages;

  /**
   * \brief the list of posible actions
   */
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

#ifdef ACTIONMONITOR_S_PLUGIN
protected:
  ShellVirtualMachine* _svm;

public:
  ShellVirtualMachine* GetShellVirtualMachine();
#endif

#ifdef ACTIONMONITOR_CS_PLUGIN
protected:
  CsVirtualMachine* _csvm;

public:
  CsVirtualMachine* GetCsVirtualMachine();
#endif

  void DestroyActiveActions();
};

CActionMonitorApp& App();

/////////////////////////////////////////////////////////////////////////////
