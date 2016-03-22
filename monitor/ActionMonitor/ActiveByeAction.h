#pragma once
#include "ActiveAction.h"
class ActiveByeAction :
  public ActiveAction
{
public:
  ActiveByeAction(const Action& src, HWND hTopHWnd );
  virtual ~ActiveByeAction();

  virtual void ExecuteInThread();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
};

