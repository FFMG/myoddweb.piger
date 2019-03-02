#pragma once

#include "Action.h"
#include "IApplication.h"

class ActionBye : public Action
{
public:
  ActionBye(IApplication& application);
	virtual ~ActionBye();

  ActiveAction* CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const override;

private:
  IApplication& _application;
};
