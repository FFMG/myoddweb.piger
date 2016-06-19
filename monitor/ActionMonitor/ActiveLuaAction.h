#pragma once
#include "ActiveAction.h"
class ActiveLuaAction :
  public ActiveAction
{
public:
  ActiveLuaAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActiveLuaAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

