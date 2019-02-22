#include "stdafx.h"
#include "ActivePowershellAction.h"
#include "ActionMonitor.h"

/**
 * The powershell contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActivePowershellAction::ActivePowershellAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActivePowershellAction::~ActivePowershellAction()
{
}

bool ActivePowershellAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActivePowershellAction::OnInitialize()
{
  //  all good
  return true;
}

void ActivePowershellAction::OnExecuteInThread()
{
  //  the file.
  const auto& szFile = File();

  // create the Python Api.
  auto& pvm = App().VirtualMachinesHandler().Get< PowershellVirtualMachine>();
  
  // we can now execute the thread.
  pvm.ExecuteInThread( szFile.c_str(), *this, App().MsgHandler() );
}