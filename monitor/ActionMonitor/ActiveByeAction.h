#pragma once
#include "ActiveAction.h"
#include "IApplication.h";

class ActiveByeAction final : public ActiveAction
{
public:
  ActiveByeAction(IApplication& application, const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd );
  virtual ~ActiveByeAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;

private:
  IApplication& _application;
};

