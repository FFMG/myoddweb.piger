#pragma once
#include "ActiveAction.h"
class ActivePythonAction final :
  public ActiveAction
{
public:
  ActivePythonAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;
};

