#pragma once
#include "ActiveVirtualMachineAction.h"

class ActivePythonAction final :
  public ActiveVirtualMachineAction
{
public:
  ActivePythonAction(const Action& src, IVirtualMachine& virtualMachine, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
};

