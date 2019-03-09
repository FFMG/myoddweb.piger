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
#include "stdafx.h"
#include "Application.h"
#include "../api/VirtualMachines.h"
#include "ActionBye.h"
#include "ActionLoad.h"
#include "ActionVersion.h"
#include "Actions.h"
#include "MessagesHandler.h"
#include "IpcListener.h"
#include "ActionsImmediate.h"
#include "ApplicationTray.h"

Application::Application() : 
  IApplication(),
  _messagesHandler(nullptr),
  _possibleActions(nullptr),
  _ipcListener(nullptr),
  _virtualMachines(nullptr),
  _dlg( nullptr ),
  _hookWnd( nullptr ),
  _startActions( nullptr),
  _restartApplication(false),
  _tray( nullptr )
{
}

Application::~Application()
{
  // we have to destroy everything.
  Destroy();
}

void Application::Close()
{
  // log that we are closing down
  myodd::log::LogMessage(_T("Piger is shutting down."));

  // stop and destroy all the running virtual machines.
  // we need to do that now as we are about to go away
  // 
  CloseAllVirtualMachines();

  // for them to finish properly
  WaitForHandlersToComplete();

  // call the end actions to run
  ShowEnd();

  // wait all the active windows.
  // those are the ones created by the end Action list.
  WaitForHandlersToComplete();

  //  close us
  if (_dlg != nullptr )
  {
    _dlg->Close();
  }
}

void Application::Restart()
{
  // log that we are restarting
  myodd::log::LogMessage(_T("Piger is restarting."));

  // close it
  _restartApplication = true;
  Close();
}

void Application::Create()
{
  // destroy the old one
  Destroy();

  // create the message handler
  CreateMessageHandler();

  // create the messages/listener.
  CreateIpcListener();

  // create the possible actions
  CreateActionsList();

  // create the virtual machines
  CreateVirtualMachines();

  // create the system tray
  CreateTray();

  // sanity checks
  assert(_possibleActions != nullptr);
  assert(_ipcListener != nullptr);
  assert(_tray != nullptr);
  assert(_dlg == nullptr);

  // create the actual dicali.
  _dlg = new ActionMonitorDlg( *_tray );

  // the hook window, we need dlg, so we have to this last.
  // also we do not want to fire the hook before needed.
  CreateHookWindow();
}

void Application::Show()
{
  // show the dialog box and wait for it to complete.
  for (;;)
  {
    // assume that we will not restart the app.
    _restartApplication = false;

    // create everything
    Create();

    // sanity check
    assert(_dlg != nullptr);

    // show the start messages.
    ShowStart();

    // show the dialog box
    _dlg->CreateAndWait();

    // we are now closed, we can destroy everything
    Destroy();

    // are we done here?
    if(_restartApplication == false )
    {
      break;
    }
  }
}

void Application::Destroy()
{
  delete _startActions;
  _startActions = nullptr;

  // hook wind
  delete _hookWnd;
  _hookWnd = nullptr;

  // remove the actions
  // we are about to close, we are no longer monitoring anything
  delete _possibleActions;
  _possibleActions = nullptr;

  // stop the virtuall machines
  delete _virtualMachines;
  _virtualMachines = nullptr;

  // stop all the message handling
  delete _messagesHandler;
  _messagesHandler = nullptr;

  delete _ipcListener;
  _ipcListener = nullptr;

  delete _tray;
  _tray = nullptr;

  delete _dlg;
  _dlg = nullptr;
}

void Application::ShowEnd()
{
  // do we have anything to show.
  if( _dlg == nullptr )
  {
    return;
  }

  // sanity check
  assert(_possibleActions != nullptr);
  assert(_virtualMachines != nullptr);

  // start the new ones
  auto endActions = new ActionsImmediate(AM_DIRECTORY_OUT, *_possibleActions, *_virtualMachines);
  endActions->Initialize();
  endActions->WaitForAll();
  delete endActions;
  endActions = nullptr;
}

void Application::ShowStart()
{
  // sanity check
  assert(_possibleActions != nullptr);
  assert(_virtualMachines != nullptr);
  assert(_dlg != nullptr);

  if( _startActions != nullptr )
  {
    _startActions->WaitForAll();
    delete _startActions;
    _startActions = nullptr;
  }

  // start the new ones
  _startActions = new ActionsImmediate(AM_DIRECTORY_IN, *_possibleActions, *_virtualMachines);
  _startActions->Initialize();

  // we do not want to wait for start messages
  // we will continue in case of long running stat processes.
  // it is bad to have blocking scripts anyway
  // rather have a script to call another process.
}

void Application::ShowVersion()
{
  myodd::files::Version ver;
  const auto strSay = myodd::strings::Format(_T("<b>Version : </b>%d.%d.%d.%d"),
    ver.GetFileVersionMajor(),
    ver.GetFileVersionMinor(),
    ver.GetFileVersionMaintenance(),
    ver.GetFileVersionBuild());

  assert(_messagesHandler != nullptr);
  _messagesHandler->Show(strSay.c_str(), 500, 3000);
}

void Application::CreateHookWindow()
{
  delete _hookWnd;

  // sanity checks
  assert(_dlg != nullptr);
  assert(_possibleActions != nullptr);
  assert(_virtualMachines != nullptr);

  _hookWnd = new HookWnd(*_dlg, *_possibleActions, *_virtualMachines);
  _hookWnd->Create();
}

void Application::CreateTray()
{
  delete _tray;
  auto tray = new ApplicationTray();
  tray->SetApplication(this);

  // we can pass the owneship.
  _tray = tray;
}

void Application::CreateVirtualMachines()
{
  // sanity checks
  assert(_possibleActions != nullptr);
  assert(_messagesHandler != nullptr);
  assert(_ipcListener != nullptr);

  // stop the virtuall machines
  delete _virtualMachines;
  _virtualMachines = new VirtualMachines(*_possibleActions, *_messagesHandler, *_ipcListener);
}

void Application::CreateIpcListener()
{
  // remove the old one
  delete _ipcListener;

  // create a new one
  _ipcListener = new IpcListener();
  _ipcListener->Initialize( );
}

/**
 * \brief (Re)build the message handler
 */
void Application::CreateMessageHandler()
{
  //  remove the old one
  delete _messagesHandler;

  //  create a new one.
  _messagesHandler = new MessagesHandler();
}

/**
 * \brief (Re)build a list of possible actions.
 */
void Application::CreateActionsList()
{
  //  remove the old one
  delete _possibleActions;

  //  create a new one.
  _possibleActions = new Actions();

  //  parse the directory for all possible files.
  _possibleActions->Initialize();

  //TODO these really need to move out of here
  //  add the default commands
  _possibleActions->Add(new ActionBye(*this));
  _possibleActions->Add(new ActionLoad(*this));
  _possibleActions->Add(new ActionVersion(*this));
}

/**
 * \brief destroy all the virtual machines.
 */
void Application::CloseAllVirtualMachines() const
{
  if (_virtualMachines == nullptr)
  {
    return;
  }
  _virtualMachines->Destroy();
}

/**
 * \brief Wait for all the active windows to complete.
 */
void Application::WaitForHandlersToComplete() const
{
  // wait for the Ipc windows.
  if (_ipcListener != nullptr)
  {
    _ipcListener->WaitForAllToComplete();
  }

  if (_messagesHandler != nullptr)
  {
    // wait for all the messages
    _messagesHandler->WaitForAllToComplete();
  }
}