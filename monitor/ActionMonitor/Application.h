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

class Application final : public IApplication
{
public:
  Application();
  virtual ~Application();

  CWnd* Create() override;

  void Destroy() override;

  void Show() override;

  void ShowVersion() override;

  void ShowStart() override;

  void ShowEnd() override;

private:
  void CreateTaskBar();
  void CreateActionsList();
  void CreateMessageHandler();
  void CreateVirtualMachines();
  void CreateIpcListener();
  void CreateHookWindow();

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
   * \brief Close the virtual machines windows/scripts
   *        This is normally done when we want to close the application.
   */
  void CloseAllVirtualMachines() const;

  /**
   * \brief Wait for the active windows to complete.
   */
  void WaitForHandlersToComplete() const;
};

