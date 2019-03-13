#include "stdafx.h"
#include "ActivePythonAction.h"

/**
 * \copydoc
 */
ActivePythonAction::ActivePythonAction(const Action& src, IVirtualMachine& virtualMachine, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveVirtualMachineAction( src, virtualMachine, hTopHWnd, szCommandLine, isPrivileged )
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
  if (!_virtualMachine.Initialize())
  {
    return false;
  }

  //  all good
  return true;
}
