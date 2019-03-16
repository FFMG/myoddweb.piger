#include "stdafx.h"
#include "ActiveVirtualMachineAction.h"

/**
 * \brief the constructor
 * \param src the action that is now active.
 * \param hTopHWnd the window that was on top at the time the command was given.
 * \param virtualMachine the virtual machine that will execute the command.
 * \param szCommandLine the given command line that is, the words after the command itself
 * \param isPrivileged if this action is privileged or not.
 */
ActiveVirtualMachineAction::ActiveVirtualMachineAction(const IAction& src, IVirtualMachine& virtualMachine, const HWND hTopHWnd, const std::wstring& szCommandLine, const bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged ),
  _virtualMachine( virtualMachine )
{
}

ActiveVirtualMachineAction::~ActiveVirtualMachineAction()
{
}

bool ActiveVirtualMachineAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveVirtualMachineAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveVirtualMachineAction::OnExecuteInThread()
{
  //  the file.
  const auto& szFile = File();

  // simply get the virtual machine th
  _virtualMachine.Execute(*this, szFile );
}
