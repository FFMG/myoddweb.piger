// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ActionLoad_h__
#define __ActionLoad_h__
#pragma once

#include "Action.h"

class ActionLoad : public Action
{
public:
  ActionLoad();
	virtual ~ActionLoad();

  ActiveAction* CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const override;
};

#endif // __Action_h__
