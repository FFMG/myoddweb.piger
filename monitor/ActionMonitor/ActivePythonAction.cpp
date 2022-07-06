#include "stdafx.h"
#include "ActivePythonAction.h"

/**
 * \copydoc
 */
ActivePythonAction::ActivePythonAction(IApplication& application, const IAction& src, IVirtualMachine& virtualMachine, HWND hTopHWnd, const std::wstring& szCommandLine, bool isPrivileged) :
  ActiveVirtualMachineAction( application, src, virtualMachine, hTopHWnd, szCommandLine, isPrivileged )
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
