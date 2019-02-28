#pragma once
#include "ActiveAction.h"
#include "../string/string.h"

class ActiveCmdAction :
  public ActiveAction
{
public:
  ActiveCmdAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine);
  virtual ~ActiveCmdAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};
