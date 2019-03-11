#include "stdafx.h"
#include "ActivePythonAction.h"
#include "ActionMonitor.h"

/**
 * The Python contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActivePythonAction::ActivePythonAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, virtualMachines, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActivePythonAction::~ActivePythonAction()
{
}

bool ActivePythonAction::OnDeInitialize()
{
  assert(Py_IsInitialized());
  return true;
}

bool ActivePythonAction::OnInitialize()
{
  auto& py = _virtualMachines.Get(IVirtualMachines::Type::Python);
  if (!py.Initialize())
  {
    return false;
  }

  //  all good
  return true;
}

void ActivePythonAction::OnExecuteInThread()
{
  //  the file who owns the action.
  const auto& szFile = File();

  // get the virtual machine
  auto& pyvm = _virtualMachines.Get(IVirtualMachines::Type::Python);

  // we can now execute the action.
  pyvm.Execute(*this, szFile);
}
