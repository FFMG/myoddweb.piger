#pragma once
#include "ActiveAction.h"
class ActiveByeAction :
  public ActiveAction
{
public:
  ActiveByeAction(const Action& src );
  virtual ~ActiveByeAction();

  virtual void ExecuteInThread();
};

