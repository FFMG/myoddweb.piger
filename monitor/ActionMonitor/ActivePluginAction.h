#pragma once
#include "ActiveAction.h"

class ActivePluginAction :
  public ActiveAction
{
public:
  ActivePluginAction(const Action& src, HWND hTopHWnd, const STD_TSTRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePluginAction();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
  virtual void OnExecuteInThread();
};

