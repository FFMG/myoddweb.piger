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
#include "IMessagesHandler.h"
#include "../api/VirtualMachines.h"

#define _MAX_CMD_LINE_ARGS  128

class CActionMonitorApp final : public CWinApp
{
public:
	CActionMonitorApp();
  virtual ~CActionMonitorApp();
  
	public:
	virtual BOOL InitInstance();

  // Implementation
	DECLARE_MESSAGE_MAP()

  static bool InitConfig(const myodd::variables& vm);
  static void InitLog();
  static void InitReservedPaths();
  void InitMaxClipboardSize();

public:
  static CWnd* GetLastForegroundWindow();
  static void SetLastForegroundWindow( CWnd *w);

  static size_t GetMaxClipboardMemory();

public:
  bool CreateAndShowActionDialog();
  void CreateTaskBar();
  void CreateActionsList();
  void CreateMessageHandler();
  void CreateVirtualMachines();
  void CreateIpcListener();

public:
  void DoVersion();
  void DoReload();
  void DoClose();

  void DoStartActionsList( bool wait );
  void DoEndActionsList(bool wait);
  void WaitForEndActionsToComplete();
  void WaitForStartActionsToComplete();

private:
  /**
   * \brief Wait for the active windows to complete.
   */
  void WaitForHandlersToComplete();

  /**
   * \brief check if the special key is currently pressed down.
   * \return if the special key is down.
   */
  bool IsSpecialKeyDown()const;

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

  /**
   * \brief the handle of the mutex
   */
  HANDLE _mutex;

  /**
   * \brief CWnd* The window that last had the focus when we pressed the special key.
   */
  CWnd* _cwndLastForegroundWindow;

  /**
   * \brief The maximum memory we want to use when getting clipboard data.
   */
  size_t _maxClipboardSize;
private:
  CFrameWnd* _taskBar;

  /**
   * \brief the messages handler
   */
  IMessagesHandler* _messagesHandler;

  /**
   * \brief the list of possible actions
   */
  IActions* _possibleActions;

  /**
   * \brief the IPC Listener.
   */
  IIpcListener* _ipcListener;

  /**
   * \brief the virtual machines handler.
   */
  VirtualMachines* _virtualMachines;

public:

  VirtualMachines& VirtualMachinesHandler()
  {
    return *_virtualMachines;
  }

  const VirtualMachines& VirtualMachinesHandler() const
  {
    return *_virtualMachines;
  }

public:
  void DestroyActiveActions() const;
};

CActionMonitorApp& App();
