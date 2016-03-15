#pragma once
#include "ActiveAction.h"
class ActivePythonAction :
  public ActiveAction
{
public:
  ActivePythonAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

  virtual void ExecuteInThread();
  
protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
};

