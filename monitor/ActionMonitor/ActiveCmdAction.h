#pragma once
#include "ActiveAction.h"
#include "../string/string.h"

class ActiveCmdAction final :
  public ActiveAction
{
public:
  ActiveCmdAction(IApplication& application, const IAction& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine);
  virtual ~ActiveCmdAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};
