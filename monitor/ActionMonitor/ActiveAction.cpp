// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActiveAction.h"
#include "ActionsCore.h"
#include "ActionMonitor.h"

/**
 * todo
 * @param void
 * @return void
 */
ActiveAction::ActiveAction( const ActiveAction& rhs ) : Action(), _clipboard(NULL)
{
  *this = rhs;
}

/**
* todo
* @param void
* @return void
*/
ActiveAction::ActiveAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged) : Action( src ), 
  _clipboard(NULL),
  _szCommandLine( szCommandLine ),
  _isPrivileged( isPrivileged )
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActiveAction::~ActiveAction()
{
  ClearClipboard();
}

const ActiveAction& ActiveAction::operator=(const ActiveAction& rhs)
{
  if (this != &rhs)
  {
    ClearClipboard();

    // base class.
    Action::operator=(rhs);

    _szCommandLine = rhs._szCommandLine;
    _isPrivileged = rhs._isPrivileged;
    if (rhs._clipboard)
    {
      _clipboard = new Clipboard(*rhs._clipboard);
    }
  }
  return *this;
}

/**
 * Clear the clipboard and free the memory.
 */
void ActiveAction::ClearClipboard()
{
  delete _clipboard;
  _clipboard = NULL;
}

void ActiveAction::CreateClipboard()
{
  //  clear the old one.
  ClearClipboard();

  // we must always get the clipboard because we never know for certain if the APIs might call
  // some functions. This will also reset the values if need be.
  //
  // so copy the text that the user could have currently selected or copy the name of the file that is probably selected
  // tell the clipboard to copy the data of the last known foreground window.
  CWnd* cwnd = CActionMonitorApp::GetLastForegroundWindow();
  _clipboard = new Clipboard(cwnd );
}

void ActiveAction::operator()()
{
  const STD_TSTRING& szExt = Extension();

#ifdef ACTIONMONITOR_API_PLUGIN
  // Do the API calls.
  //
  if (PluginVirtualMachine::IsPluginExt(szExt.c_str()))
  {
    DoItDirectPlugin();
    return;
  }
#endif // ACTIONMONITOR_API_PLUGIN

#ifdef ACTIONMONITOR_API_LUA
  // Do the API calls.
  //
  if (LuaVirtualMachine::IsLuaExt(szExt.c_str()))
  {
    DoItDirectLua();
    return;
  }
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
  // Do the API calls.
  //
  if (PythonVirtualMachine::IsPyExt(szExt.c_str()))
  {
    DoItDirectPython();
    return;
  }
#endif // ACTIONMONITOR_API_PY

  //  the file.
  const STD_TSTRING& szFile = File();

  //  join the two items together.
  std::vector<STD_TSTRING> argv;
  argv.push_back(szFile);
  argv.push_back( _szCommandLine);
  Action::Execute(argv, _isPrivileged);
}

#ifdef ACTIONMONITOR_API_LUA
void ActiveAction::DoItDirectLua() const
{
  //  the file.
  const STD_TSTRING& szFile = File();

  LuaVirtualMachine* lua = App().GetLuaVirtualMachine();
  lua->LoadFile(szFile.c_str(), *this);
}
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
void ActiveAction::DoItDirectPython() const
{
  //  the file.
  const STD_TSTRING& szFile = File();

  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  py->Execute(szFile.c_str(), *this);
}
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_API_PLUGIN
void ActiveAction::DoItDirectPlugin() const
{
  //  the file.
  const STD_TSTRING& szFile = File();

  PluginVirtualMachine* pg = App().GetPluginVirtualMachine();
  pg->LoadFile(szFile.c_str(), *this );
}
#endif // ACTIONMONITOR_API_PLUGIN