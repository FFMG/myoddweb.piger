#pragma once
#include "ActiveVirtualMachineAction.h"

class ActivePythonAction final :
  public ActiveVirtualMachineAction
{
public:
  ActivePythonAction(IApplication& application, const IAction& src, IVirtualMachine& virtualMachine, HWND hTopHWnd, const std::wstring& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
};

