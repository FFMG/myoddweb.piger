#include "stdafx.h"
#include "ActiveLuaAction.h"
#include "ActionMonitor.h"

/**
 * The LUA contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveLuaAction::ActiveLuaAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, virtualMachines, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActiveLuaAction::~ActiveLuaAction()
{
}

bool ActiveLuaAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveLuaAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveLuaAction::OnExecuteInThread()
{
  //  the file.
  const auto& szFile = File();

  // create the Python Api.
  auto& lua = _virtualMachines.Get(IVirtualMachines::Type::Lua);

  //  save it.
  // we can now execute the thread.
  lua.Execute(*this, szFile );
}
