#pragma once
#include "ActiveAction.h"

class ActivePluginAction :
  public ActiveAction
{
public:
  ActivePluginAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePluginAction();

  virtual void ExecuteInThread();
  
protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();
};

