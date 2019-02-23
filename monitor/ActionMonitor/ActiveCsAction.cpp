#include "stdafx.h"
#include "ActiveCsAction.h"
#include "ActionMonitor.h"

/**
 * The powershell contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveCsAction::ActiveCsAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActiveCsAction::~ActiveCsAction()
{
}

bool ActiveCsAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveCsAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveCsAction::OnExecuteInThread()
{
  //  the file.
  const auto& szFile = File();

  // create the Python Api.
  auto& csvm = App().VirtualMachinesHandler().Get1<CsVirtualMachine>();
  
  // we can now execute the thread.
  csvm.Execute( *this, szFile.c_str() );
}