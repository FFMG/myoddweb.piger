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
#include "Action.h"
#include "ActionMonitor.h"
#include "activeaction.h"
#include "activepythonaction.h"
#include "ActiveVirtualMachineAction.h"
#include "activedefaultaction.h"
#include "activebatchaction.h"
#include "activecmdaction.h"
#include "activecomaction.h"
#include "activeexeaction.h"

#include "os/os.h"

/**
 * \brief Constructor
 */
Action::Action()
{
  Reset();
}

/**
 * \brief Constructor
 * \param szCommand the name of the action, (the the user will enter)
 * \param szPath the full path of the action that we will execute.
 * \return none
 */
Action::Action(const std::wstring& szCommand, const std::wstring& szPath ) : 
  Action()
{
  if(szCommand.length() == 0 )
  {
    //  we cannot have nullptr commands.
    throw -1;
  }

  // set the command and make sure it is valid.
  _szCommand = szCommand;
  myodd::strings::Trim(_szCommand);

  //  make sure that the command is valid.
  if ( 0 == Len() )
  {
    //  we cannot have nullptr commands.
    throw - 1;
  }

  // expand the path
  if( szPath.length() > 0  )
  {
    LPTSTR lpExpandPath = nullptr;
    if( myodd::files::ExpandEnvironment( szPath.c_str(), lpExpandPath ) )
    {
      _szFile = lpExpandPath;
      delete [] lpExpandPath;
    }
    else
    {
      // keep whatever value was given to us.
      _szFile = szPath;
    }
  }// if we have an szPath != nullptr
}

/**
 * copy constructor
 *
 * @version 0.1
 * @return none
 */
Action::Action( const Action&action) : 
  Action()
{
  *this = action;
}

/**
 * copy constructor
 *
 * @version 0.1
 * @return none
 */
auto Action::operator=(const Action& action) -> const Action&
{
  if( this != &action )
  {
    _szCommand = action._szCommand; 
    _szFile   = action._szFile;
  }
  return *this;
}

/**
 * \brief Reset the path and the command name.
 */
void Action::Reset()
{
  _szFile = _szCommand = _T("");
}

/**
 * \brief Set the path for this action.
 * \param szPath set the path for this command.
 * \return void
 */
void Action::SetCommandPath(const std::wstring& szPath )
{
  _szFile = szPath;
}

/**
 * \brief Run the command, we take into account the current selection and command parameters given.
 * \param virtualMachines the virtual machine.
 * \param pWnd the last foreground window.
 * \param szCommandLine the command line argument.
 * \param isPrivileged if we need administrator privilege to run this.
 * \return BOOL true.
 */
IActiveAction* Action::CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, const bool isPrivileged) const
{
  //  not sure how to do that...
  if ( Len() == 0)
  {
    return nullptr;
  }

  // we are about to execute a command, we don't know how long the command will last
  // a badly written plugin could take forever to return.
  const auto bThen = hook_RejectKeyboad( FALSE );

  //  if we are here then we are going to load a user command
  //
  //  If the user did not pass any arguments/command line then we must get them from the clipboard.
  IActiveAction* aa;
  if( szCommandLine.length() == 0 )
  {
    aa = CreateActiveActionWithNoCommandLine( virtualMachines, pWnd, isPrivileged);
  }
  else
  {
    //  so now, at last we can call the command line
    aa = CreateActiveActionDirect( virtualMachines, pWnd, szCommandLine.c_str(), isPrivileged);
  }

  // now that we are back from calling the plugin, restore the keyboard state.
  hook_RejectKeyboad( bThen );

  return aa;
}

/**
 * \brief Try and do it when we have no command line
 * \param virtualMachines
 * \param pWnd the last foreground window.
 * \param isPrivileged if this action is privileged or not.
 * \return bool success or not.
 */
IActiveAction* Action::CreateActiveActionWithNoCommandLine(IVirtualMachines& virtualMachines, CWnd* pWnd, const bool isPrivileged ) const
{
  //  the command line we will try and make.
  std::wstring szCommandLine = L"";

  // how much memory we want to use .... max value.
  const auto maxClipboardMemory = CActionMonitorApp::GetMaxClipboardMemory();

  //
  //  we need to wrap the whole clipboard around try/catch as not all clipboard cases have been tested
  //  I try to restore data  to the clipboard without really knowing if the data itself is valid
  //  so that can cause some kind of problem(s)
  try
  {
    //  ask the system if anything was copied.
    //  the clipboard function will ask us to get the most likely text
    //  there will probably only be a conflict with explorer, (of any flavor)
    //  that could copy text and/or file names.
    //
    const auto foregroundWnd = CActionMonitorApp::GetLastForegroundWindow();
    Clipboard clipboard(foregroundWnd, maxClipboardMemory );

    // any other values are rejected, (bitmaps and so on).
    // we do not want to add a quote around the text
    // it is up to the calling action to surround the text with quotes of need be
    std::wstring sText = L"";
    if (clipboard.GetTextFromClipboard(sText, false ))
    {
      //  we need to trim all the items into one single line
      //  because command lines cannot accept multiple lines
      //
      //  or we might need to replace certain characters.
      szCommandLine = ToSingleLine(sText.c_str());
    }
  }
  catch (...)
  {
    szCommandLine = _T("");
    myodd::log::LogError(_T("Critical error while trying to run an action, [%s]."), Command().c_str() );
    _ASSERT(0);         //  the main reason for failure is probably because  
                        //  there is a format in the Clipboard that I am not handling properly
                        //  there should be a way of sending me a mail when this happens so we can look into fixing it.
  }

  // we can now do it direct.
  return CreateActiveActionDirect( virtualMachines, pWnd, szCommandLine, isPrivileged);
}

/**
 * \brief Get a text an remove the single lines out of it/
 * TODO: Move this to the am_string namespace?
 * \param text the text that we would like to enforce into a single line.
 * \return a single line of text.
 */
std::wstring Action::ToSingleLine( const wchar_t* text ) const
{
  // Sanity checks.
  if(nullptr == text )
  {
    return L"";
  }

  auto pdest = _tcschr( text, L'\n' );
  size_t  result;

  std::wstring ret( text );
  if( pdest != nullptr)
  {
    result = pdest - text;
    ret = ret.substr( 0, result );
    return ToSingleLine( ret.c_str() );
  }

  pdest = _tcschr( text, L'\r' );
  if( pdest != nullptr)
  {
    result = pdest - text;
    ret = ret.substr( 0, result );
    return ToSingleLine( ret.c_str() );
  }
  return ret;
}

/**
 * \brief Return the command as a string
 * \return the command, (the name the user has to enter).
 */
const std::wstring& Action::Command() const
{
  return _szCommand;
}

/**
 * \brief Execute a file.
 *        We will expend all the environment variables as needed.
 * \param argv [0] the file path, [1] the arguments to launch with, (optional).
 * \param isPrivileged if we need administrator privilege to run this.
 * \param hProcess if this value is not nullptr, we will return the handle of the started process.
 *               it is then up to the calling application to close this handle when done with it...
 * \return bool true|false success or not.
 */
bool Action::Execute(const std::vector<std::wstring>& argv, const bool isPrivileged, HANDLE* hProcess)
{
  // get the number of arguments.
  auto argc = argv.size();

  // sanity check
  if( argc < 1 || argc > 2 )
  {
    ASSERT ( 0 ); //  wrong number of arguments.
    return false;
  }

  LPTSTR argvModule = nullptr;
  LPTSTR argvCmd = nullptr;

  // get the module name, (what we are running).
  // Expand the values that might have been passed.
  if( !myodd::files::ExpandEnvironment( argv[ 0 ].c_str(), argvModule ) )
  {
    myodd::log::LogError(_T("Could not execute statement: Unable to expand command line '%s'"), argv[0].c_str());
    return false;
  }

  // But we might also have a command line item.
  if( 2 == argc )
  {
    // Expand the values that might have been passed.
    if( !myodd::files::ExpandEnvironment( argv[ 1 ].c_str(), argvCmd ) )
    {
      myodd::log::LogError(_T("Could not execute statement: Unable to expand arguments '%s'"), argv[1].c_str());
      delete [] argvModule;
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
  if (isPrivileged == true && !myodd::os::IsElevated() )
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
  if (hProcess != nullptr )
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
  delete [] argvCmd;
  delete [] argvModule;

  // return what we found
  return result;
}

/**
 * \brief Launch a single action with all the command line arguments.
 * TODO : The API calls ignore the values been passed to them, so we should first check that we have all the values.
 * \param virtualMachines the virtual machines manager.
 * \param pWnd the last foreground window.
 * \param szCommandLine the command and the arguments we are launching this file with.
 * \param isPrivileged if we need administrator privilege to run this.
 * \return TRUE|FALSE success or not.
 */
IActiveAction* Action::CreateActiveActionDirect(IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, const bool isPrivileged) const
{
  // sanity check
  if (0 == _szFile.length())
  {
    return nullptr;
  }

  //  get the last forground window handle
  const auto hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;

#ifdef ACTIONMONITOR_API_LUA
  // Do the API calls.
  //
  if (LuaVirtualMachine::IsExt(_szFile ))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::Lua);
    auto ala = new ActiveVirtualMachineAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if (ala->Initialize())
    {
      return ala;
    }

    // did not work, try the default way...
    delete ala;
  }
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
  // Do the API calls.
  //
  if (PythonVirtualMachine::IsExt(_szFile))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::Python);
    auto apa = new ActivePythonAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if(apa->Initialize() )
    { 
      return apa;
    }

    // did not work, try the default way...
    delete apa;
  }
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_PS_PLUGIN
  // Do the API calls.
  //
  if (PowershellVirtualMachine::IsExt(_szFile))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::Powershell);
    auto apa = new ActiveVirtualMachineAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if (apa->Initialize())
    {
      return apa;
    }

    // did not work, try the default way...`
    delete apa;
  }
#endif // ACTIONMONITOR_PS_PLUGIN

#ifdef ACTIONMONITOR_S_PLUGIN
  // Do the Shell calls.
  //
  if (ShellVirtualMachine::IsExt(_szFile))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::Shell);
    auto asa = new ActiveVirtualMachineAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if (asa->Initialize())
    {
      return asa;
    }

    // did not work, try the default way...
    delete asa;
  }
#endif // ACTIONMONITOR_S_PLUGIN

#ifdef ACTIONMONITOR_CS_PLUGIN
  // Do the CSharp calls.
  //
  if (CsVirtualMachine::IsExt(_szFile))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::CSharp);
    auto acsa = new ActiveVirtualMachineAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if (acsa->Initialize())
    {
      return acsa;
    }

    // did not work, try the default way...
    delete acsa;
  }
#endif // ACTIONMONITOR_CS_PLUGIN

#ifdef ACTIONMONITOR_API_PLUGIN
  // Do the API calls.
  //
  if (PluginVirtualMachine::IsExt(_szFile))
  {
    auto& virtualMachine = virtualMachines.Get(IVirtualMachines::Type::LegacyPlugin);
    auto apa = new ActiveVirtualMachineAction(*this, virtualMachine, hTopHWnd, szCommandLine, isPrivileged);
    if (apa->Initialize())
    {
      return apa;
    }

    // did not work, try the default way...
    delete apa;
  }
#endif // ACTIONMONITOR_API_PLUGIN

  // Batch files...
  if( myodd::files::IsExtension (_szFile, _T("bat")))
  {
    return new ActiveBatchAction(*this, hTopHWnd, szCommandLine );
  }
  if (myodd::files::IsExtension(_szFile, _T("cmd")))
  {
    return new ActiveCmdAction(*this, hTopHWnd, szCommandLine);
  }
  if (myodd::files::IsExtension(_szFile, _T("com")))
  {
    return new ActiveComAction(*this, hTopHWnd, szCommandLine);
  }
  if (myodd::files::IsExtension(_szFile, _T("exe")))
  {
    return new ActiveExeAction(*this, hTopHWnd, szCommandLine, isPrivileged);
  }

  // run the default action.
  return new ActiveDefaultAction( *this, hTopHWnd, szCommandLine, isPrivileged );
}