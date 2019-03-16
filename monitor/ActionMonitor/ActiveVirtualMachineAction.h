#pragma once
#include "ActiveAction.h"
class ActiveVirtualMachineAction :
  public ActiveAction
{
public:
  ActiveVirtualMachineAction(const IAction& src, IVirtualMachine& virtualMachine, HWND hTopHWnd, const std::wstring& szCommandLine, bool isPrivileged);
  virtual ~ActiveVirtualMachineAction();

protected:
  IVirtualMachine& _virtualMachine;

  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;
};

