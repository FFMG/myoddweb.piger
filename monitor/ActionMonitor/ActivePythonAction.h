#pragma once
#include "ActiveAction.h"
class ActivePythonAction :
  public ActiveAction
{
public:
  ActivePythonAction(const Action& src, HWND hTopHWnd, const STD_TSTRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

