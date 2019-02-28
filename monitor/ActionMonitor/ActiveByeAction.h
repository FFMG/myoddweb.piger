#pragma once
#include "ActiveAction.h"
class ActiveByeAction :
  public ActiveAction
{
public:
  ActiveByeAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd );
  virtual ~ActiveByeAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

