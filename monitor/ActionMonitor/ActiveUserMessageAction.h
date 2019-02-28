#pragma once
#include "ActiveAction.h"
class ActiveUserMessageAction :
  public ActiveAction
{
public:
  ActiveUserMessageAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, UINT Msg );
  virtual ~ActiveUserMessageAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();

  UINT _Msg;
};

