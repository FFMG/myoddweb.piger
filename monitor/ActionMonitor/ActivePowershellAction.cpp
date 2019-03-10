#include "stdafx.h"
#include "ActivePowershellAction.h"

/**
 * \brief The powershell constructor
 * \param src the action that is now active.
 * \param virtualMachines the virtual machines
 * \param hTopHWnd the window that was on top at the time the command was given.
 * \param szCommandLine the given command line that is, the words after the command itself
 * \param isPrivileged if this action is privileged or not.
 */
ActivePowershellAction::ActivePowershellAction(const Action& src, IVirtualMachines& virtualMachines, const HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, virtualMachines, hTopHWnd, szCommandLine, isPrivileged )
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
  auto& pvm = _virtualMachines.Get(IVirtualMachines::Type::Powershell );
  
  // we can now execute the thread.
  pvm.Execute(*this, szFile );
}