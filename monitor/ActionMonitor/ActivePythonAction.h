#pragma once
#include "ActiveAction.h"
class ActivePythonAction :
  public ActiveAction
{
public:
  ActivePythonAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged);
  virtual ~ActivePythonAction();

  virtual void ExecuteInThread();
  virtual bool Initialize();
  virtual bool DeInitialize();

protected:
  bool ReadFile(LPCTSTR pyFile, std::string& script) const;
};

