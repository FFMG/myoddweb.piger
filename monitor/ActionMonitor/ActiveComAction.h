#pragma once
#include "ActiveAction.h"
#include "../string/string.h"

class ActiveComAction :
  public ActiveAction
{
public:
  ActiveComAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine);
  virtual ~ActiveComAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};
