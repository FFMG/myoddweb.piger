#include "stdafx.h"
#include "ActiveLuaAction.h"
#include "ActionMonitor.h"

ActiveLuaAction::ActiveLuaAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, szCommandLine, isPrivileged )
{
  Initialize();
}

ActiveLuaAction::~ActiveLuaAction()
{
  DeInitialize();
}

bool ActiveLuaAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveLuaAction::OnInitialize()
{
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  if (!py->Initialize())
  {
    return false;
  }

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
