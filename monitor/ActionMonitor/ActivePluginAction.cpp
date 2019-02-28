#include "stdafx.h"
#include "ActivePluginAction.h"
#include "ActionMonitor.h"

/**
 * The plugin contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActivePluginAction::ActivePluginAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, virtualMachines, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActivePluginAction::~ActivePluginAction()
{
}

bool ActivePluginAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActivePluginAction::OnInitialize()
{
  //  all good
  return true;
}

void ActivePluginAction::OnExecuteInThread()
{
  //  the file.
  const auto& szFile = File();

  // create the Python Api.
  auto& pvm = _virtualMachines.Get(IVirtualMachines::Type::LegacyPlugin);
  
  //  save it.
  // we can now execute the thread.
  pvm.Execute( *this, szFile);
}