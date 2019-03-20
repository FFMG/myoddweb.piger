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
#include "IApplication.h"
#include "IMessagesHandler.h"
#include "IActions.h"
#include "IIpcListener.h"
#include "../api/IVirtualMachines.h"
#include "ActionMonitorDlg.h"
#include "HookWnd.h"
#include "ActionsImmediate.h"
#include "ActiveActionsRunner.h"
#include <queue>
#include "threads/messages.h"

class Application final : public IApplication
{
public:
  Application();
  virtual ~Application();

  void Close() override;

  void Restart() override;

  void Version() override;

  void Show() override;

  bool AddAction(IAction* action) override;

  bool RemoveAction(const std::wstring& szText, const std::wstring& szPath) const override;

  const IAction* FindAction(unsigned int idx, const std::wstring& szText) const override;

  bool Execute(const std::vector<std::wstring>& argv, const bool isPrivileged, HANDLE* hProcess) const override;

  bool ExecuteActiveAction( IActiveAction* action) const override;

  bool IsActiveActionRunning(IActiveAction* action) const override;

  bool ExecuteCurrentAction() override;

  CWnd* GetLastForegroundWindow() const override;

  void SetLastForegroundWindow() override;

private:
  void OnVersion();
  void ShowVersion();

  void OnRestart();
  void ShowStart();

  void OnClose();
  void ShowClose();

  void CreateBase();
  void CreateForRestart();

  void CreateActionsList();
  void CreateMessageHandler();
  void CreateVirtualMachines();
  void CreateIpcListener();
  void CreateHookWindow();
  void CreateTray();
  void CreateActiveActionsRunner();
  void CreateMessages();
  
  void DestroyBase();
  void DestroyForRestart();

  void PrepareForClose();

  CWnd* _lastForegroundWindow;

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
   * \brief the task tray
   */
  ITray* _tray;

  /**
   * \brief the virtual machines handler.
   */
  IVirtualMachines* _virtualMachines;

  /**
   * \brief the current dialog being displayed.
   */
  ActionMonitorDlg* _dlg;

  /**
   * \brief the hook window.
   */
  HookWnd* _hookWnd;

  /**
   * \brief the actions runner/executor
   */
  ActiveActionsRunner* _activeActionsRunner;

  /**
   * \bief start actions that we last started
   *       we don't normally wait for actions to complete
   *       in case they are long running
   *       but we keep track of it, in case we need to kill it.
   */
  ActionsImmediate* _startActions;

  /**
   * \brief Close the virtual machines windows/scripts
   *        This is normally done when we want to close the application.
   */
  void CloseAllVirtualMachines() const;

  /**
   * \brief Wait for the active windows to complete.
   */
  void WaitForHandlersToComplete() const;

  myodd::threads::Messages* _messages;
};

