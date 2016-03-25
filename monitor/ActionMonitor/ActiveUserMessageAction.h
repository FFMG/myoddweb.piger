#pragma once
#include "ActiveAction.h"
class ActiveUserMessageAction :
  public ActiveAction
{
public:
  ActiveUserMessageAction(const Action& src, HWND hTopHWnd, UINT Msg );
  virtual ~ActiveUserMessageAction();

  virtual void ExecuteInThread();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();

  UINT _Msg;
};

