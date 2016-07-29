#pragma once
#include "ActiveAction.h"

class ActiveExeAction :
  public ActiveAction
{
public:
  ActiveExeAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActiveExeAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};