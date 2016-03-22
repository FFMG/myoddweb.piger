#include "stdafx.h"
#include "ActiveLuaAction.h"
#include "ActionMonitor.h"

/**
 * The LUA contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const STD_TSTRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveLuaAction::ActiveLuaAction(const Action& src, HWND hTopHWnd, const STD_TSTRING& szCommandLine, bool isPrivileged) :
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

void ActiveLuaAction::ExecuteInThread()
{
  //  the file.
  const STD_TSTRING& szFile = File();

  // create the Python Api.
  LuaVirtualMachine* lua = App().GetLuaVirtualMachine();
  luaapi* api = new luaapi( *this );

  //  save it.
  // we can now execute the thread.
  lua->ExecuteInThread(szFile, api);

  // we can now dispose of the lua
  delete api;
}
