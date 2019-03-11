#pragma once
#include "ActiveAction.h"

class ActivePluginAction :
  public ActiveAction
{
public:
  ActivePluginAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePluginAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

