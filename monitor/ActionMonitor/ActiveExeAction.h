#pragma once
#include "ActiveAction.h"

class ActiveExeAction :
  public ActiveAction
{
public:
  ActiveExeAction(IApplication& application, const IAction& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActiveExeAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;
};