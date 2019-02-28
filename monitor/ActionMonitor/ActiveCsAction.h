#pragma once
#include "ActiveAction.h"
#include <config/config.h>

class ActiveCsAction :
  public ActiveAction
{
public:
  ActiveCsAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActiveCsAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

