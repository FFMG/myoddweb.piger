#pragma once
#include "ActiveAction.h"
#include <config/config.h>

class ActivePowershellAction :
  public ActiveAction
{
public:
  ActivePowershellAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePowershellAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

