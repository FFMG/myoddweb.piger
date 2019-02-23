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
ActivePythonAction::ActivePythonAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
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
  auto& py = App().VirtualMachinesHandler().Get<PythonVirtualMachine>();
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
  auto& pyvm = App().VirtualMachinesHandler().Get<PythonVirtualMachine>();

  // we can now execute the action.
  pyvm.Execute(*this, szFile);
}
