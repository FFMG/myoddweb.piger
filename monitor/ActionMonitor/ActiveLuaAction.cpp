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
ActiveLuaAction::ActiveLuaAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
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
  auto& lua = App().VirtualMachinesHandler().Get<LuaVirtualMachine>();
  const auto api = new LuaApi( *this, App().MsgHandler() );

  //  save it.
  // we can now execute the thread.
  lua.ExecuteInThread(szFile, api);

  // we can now dispose of the lua
  delete api;
}
