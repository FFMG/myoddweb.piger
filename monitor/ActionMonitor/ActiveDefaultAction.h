#pragma once
#include "ActiveAction.h"

class ActiveDefaultAction :
  public ActiveAction
{
public:
  ActiveDefaultAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActiveDefaultAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};