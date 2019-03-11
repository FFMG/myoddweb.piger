#pragma once
#include "ActiveAction.h"
#include "../string/string.h"

class ActiveBatchAction :
  public ActiveAction
{
public:
  ActiveBatchAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine);
  virtual ~ActiveBatchAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};
