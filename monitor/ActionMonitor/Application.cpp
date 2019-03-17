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
  _lastForegroundWindow(nullptr),
  _messagesHandler(nullptr),
  _possibleActions(nullptr),
  _ipcListener(nullptr),
  _tray(nullptr),
  _virtualMachines(nullptr),
  _dlg(nullptr),
  _hookWnd(nullptr),
  _activeActionsRunner(nullptr),
  _startActions(nullptr)
{
}

Application::~Application()
{
  // we have to destroy everything.
  DestroyForRestart();
  DestroyBase();
}

/**
 * \brief close the main application
 */
void Application::Close()
{
  // log that we are closing down
  myodd::log::LogMessage(_T("Piger is shutting down."));

  // prepare for closing
  PrepareForClose();

  // and then close
  if (_dlg != nullptr)
  {
    _dlg->Close();
  }

}

/**
 * \brief reload all the actions and re-create the base/actions/machines etc...
 */
void Application::Restart()
{
  // log that we are restarting
  myodd::log::LogMessage(_T("Piger is restarting."));

  // prepare to close.
  PrepareForClose();

  // remove the old stuff
  // and recreate it right away
  DestroyForRestart();
  CreateForRestart();

  // and then show the new start message.
  ShowStart();
}

void Application::Show()
{
  // show the dialog box and wait for it to complete.
 
  // create everything
  CreateBase();
  CreateForRestart();

  // sanity check
  assert(_dlg != nullptr);

  // show the dialog box
  _dlg->Create();

  // show the start messages.
  ShowStart();

  // wait for everything to finish
  _dlg->Wait();

  // we are now closed, we can destroy everything
  DestroyForRestart();
  DestroyBase();
}

/**
 * \brief called before we destroy the variable to wait for certain actions/machines to complete
 *        we will send them messages to close/stop what they are doing
 *        so we do not destroy anything while threads are still running.
 */
void Application::PrepareForClose()
{
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
}

/**
 * \brief create variables that are destroyed on application reload.
 */
void Application::DestroyForRestart()
{
  // the running active actions
  delete _activeActionsRunner;
  _activeActionsRunner = nullptr;

  // the start actions
  delete _startActions;
  _startActions = nullptr;

  // hook window
  delete _hookWnd;
  _hookWnd = nullptr;

  // remove the actions
  // we are about to close, we are no longer monitoring anything
  delete _possibleActions;
  _possibleActions = nullptr;

  // stop the virtuall machines
  delete _virtualMachines;
  _virtualMachines = nullptr;
}

/**
 * \brief destroy all the variables that are unique to the application
 *        they are, (normally), only created once.
 */
void Application::DestroyBase()
{
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

#pragma region Create variables
void Application::CreateBase()
{
  // destroy the old one
  DestroyBase();

  // create the system tray
  CreateTray();

  // create the messages handler.
  CreateMessageHandler();

  // create the messages/listener.
  CreateIpcListener();

  // create the actual dicali.
  _dlg = new ActionMonitorDlg(*_tray);
}

void Application::CreateForRestart()
{
  // create the possible actions
  CreateActionsList();

  // the actions runner
  CreateActiveActionsRunner();

  // create the virtual machines
  CreateVirtualMachines();

  // sanity checks
  assert(_possibleActions != nullptr);
  assert(_ipcListener != nullptr);
  assert(_tray != nullptr);
  assert(_dlg != nullptr);

  // the hook window, we need dlg, so we have to this last.
  // also we do not want to fire the hook before needed.
  CreateHookWindow();
}

void Application::CreateHookWindow()
{
  delete _hookWnd;

  // sanity checks
  assert(_dlg != nullptr);
  assert(_possibleActions != nullptr);
  assert(_virtualMachines != nullptr);

  _hookWnd = new HookWnd(*this, *_dlg, *_possibleActions, *_virtualMachines);
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
  assert(_messagesHandler != nullptr);
  assert(_ipcListener != nullptr);

  // stop the virtuall machines
  delete _virtualMachines;
  _virtualMachines = new VirtualMachines(*this, *_messagesHandler, *_ipcListener);
}

/**
 * \brief the IPC messages handler from the virtual machines.
 */
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
 * \brief (Re)build the active action runner.
 */
void Application::CreateActiveActionsRunner()
{
  delete _activeActionsRunner;
  _activeActionsRunner = new ActiveActionsRunner();
}

/**
 * \brief (Re)build a list of possible actions.
 */
void Application::CreateActionsList()
{
  //  remove the old one
  delete _possibleActions;

  //  create a new one.
  _possibleActions = new Actions( *this );

  //  parse the directory for all possible files.
  _possibleActions->Initialize();

  //TODO these really need to move out of here
  //  add the default commands
  _possibleActions->Add(new ActionBye(*this));
  _possibleActions->Add(new ActionLoad(*this));
  _possibleActions->Add(new ActionVersion(*this));
}
#pragma endregion

/**
 * \brief Close all the virtual machines.
 *        we send them all a message to end what they are doing
 *        or we detach from them all so we no longer can handle their requests.
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

#pragma region Show common messages
void Application::ShowEnd()
{
  // do we have anything to show.
  if (_dlg == nullptr)
  {
    return;
  }

  // sanity check
  assert(_possibleActions != nullptr);
  assert(_virtualMachines != nullptr);

  // start the new ones
  auto endActions = new ActionsImmediate(*this, AM_DIRECTORY_OUT, *_possibleActions, *_virtualMachines);
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

  if (_startActions != nullptr)
  {
    _startActions->WaitForAll();
    delete _startActions;
    _startActions = nullptr;
  }

  // start the new ones
  _startActions = new ActionsImmediate(*this, AM_DIRECTORY_IN, *_possibleActions, *_virtualMachines);
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
#pragma endregion

/**
 * \brief Execute a file.
 *        We will expend all the environment variables as needed.
 * \param argv [0] the file path, [1] the arguments to launch with, (optional).
 * \param isPrivileged if we need administrator privilege to run this.
 * \param hProcess if this value is not nullptr, we will return the handle of the started process.
 *               it is then up to the calling application to close this handle when done with it...
 * \return bool true|false success or not.
 */
bool Application::Execute(const std::vector<std::wstring>& argv, const bool isPrivileged, HANDLE* hProcess) const
{
  // get the number of arguments.
  const auto argc = argv.size();

  // sanity check
  if (argc < 1 || argc > 2)
  {
    ASSERT(0); //  wrong number of arguments.
    return false;
  }

  LPTSTR argvModule = nullptr;
  LPTSTR argvCmd = nullptr;

  // get the module name, (what we are running).
  // Expand the values that might have been passed.
  if (!myodd::files::ExpandEnvironment(argv[0].c_str(), argvModule))
  {
    myodd::log::LogError(_T("Could not execute statement: Unable to expand command line '%s'"), argv[0].c_str());
    return false;
  }

  // But we might also have a command line item.
  if (2 == argc)
  {
    // Expand the values that might have been passed.
    if (!myodd::files::ExpandEnvironment(argv[1].c_str(), argvCmd))
    {
      myodd::log::LogError(_T("Could not execute statement: Unable to expand arguments '%s'"), argv[1].c_str());
      delete[] argvModule;
      return false;
    }
  }

  //
  // ShellExecuteEx
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb759784(v=vs.85).aspx 
  //
  SHELLEXECUTEINFO sei = {};
  sei.cbSize = sizeof(sei);     // in, required, sizeof of this structure
  sei.fMask = SEE_MASK_DEFAULT; // in, SEE_MASK_XXX values
  sei.hwnd = nullptr;           // in, optional
  sei.lpFile = argvModule;      // in, either this value or lpIDList must be specified
  sei.lpParameters = argvCmd;   // in, optional
  sei.lpDirectory = nullptr;    // in, optional
  sei.nShow = SW_NORMAL;        // in, required
  //sei.hInstApp;               // out when SEE_MASK_NOCLOSEPROCESS is specified
  //sei.lpIDList;               // in, valid when SEE_MASK_IDLIST is specified, PCIDLIST_ABSOLUTE, for use with SEE_MASK_IDLIST & SEE_MASK_INVOKEIDLIST
  //sei.lpClass;                // in, valid when SEE_MASK_CLASSNAME is specified
  //sei.hkeyClass;              // in, valid when SEE_MASK_CLASSKEY is specified
  //sei.dwHotKey;               // in, valid when SEE_MASK_HOTKEY is specified
  if (isPrivileged == true && !myodd::os::IsElevated())
  {
    sei.lpVerb = _T("runas"); // in, optional when unspecified the default verb is choosen
  }
  else
  {
    //  launch as a normal file.
    sei.lpVerb = _T("open");  // in, optional when unspecified the default verb is choosen
  }

  // did the user pass a handle?
  // if they did then they want to take ownership of the process and close the handle.
  if (hProcess != nullptr)
  {
    sei.fMask = SEE_MASK_NOCLOSEPROCESS; // in, SEE_MASK_XXX values
  }

  // Assume error
  auto result = false;
  if (!ShellExecuteEx(&sei))
  {
    myodd::log::LogError(_T("Could not execute statement: could not execute '%s'"), argvModule);
    myodd::log::LogError(_T("Could not execute statement: Last error '%d'"), ::GetLastError());
  }
  else
  {
    result = true;
    if (hProcess != nullptr)
    {
      // return the handle.
      *hProcess = sei.hProcess;
    }
  }

  // clean up the expended variable.
  delete[] argvCmd;
  delete[] argvModule;

  // return what we found
  return result;
}

bool Application::AddAction(IAction* action)
{
  if( nullptr == _possibleActions )
  {
    return false;
  }
  return _possibleActions->Add(action);
}

bool Application::RemoveAction(const std::wstring& szText, const std::wstring& szPath) const
{
  if (nullptr == _possibleActions)
  {
    return false;
  }
  return _possibleActions->Remove(szText, szPath );
}

const IAction* Application::FindAction(unsigned int idx, const std::wstring& szText) const
{
  if (nullptr == _possibleActions)
  {
    return nullptr;
  }
  return _possibleActions->Find(szText, idx );
}

bool Application::ExecuteCurrentAction()
{
  if( nullptr == _possibleActions)
  {
    return false;
  }
  if( nullptr == _virtualMachines )
  {
    return false;
  }

  //  We use the Actions to find the actual command
  //  because either it is an exact match, (and it doesn't matter)
  //  or it is not an exact match and this is what we meant to use
  // 
  //  so if the user enters 'goo french victories'
  //  we will use the first command 'google' with the arguments 'french victories'
  //
  //  we use getCommand in case the user has chosen number 1, 2 ... in the list of possible commands 

  // if there is an active action, get it.
  const auto action = _possibleActions->GetCommand();
  if (nullptr == action)
  {
    return false;
  }

  try
  {
    // get the command line
    const auto szCommandLine = _possibleActions->GetCommandLine();

    //  do the action now
    //  we might not have any, but that's not for us to decides :).
    const auto pWnd = GetLastForegroundWindow();
    _activeActionsRunner->QueueAndExecute(action->CreateActiveAction(*_virtualMachines, pWnd, szCommandLine, false));
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

void Application::SetLastForegroundWindow()
{
  _lastForegroundWindow = CWnd::GetForegroundWindow();
}

CWnd* Application::GetLastForegroundWindow() const
{
  return _lastForegroundWindow;
}