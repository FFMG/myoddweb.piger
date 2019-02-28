// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ActionBye_h__
#define __ActionBye_h__
#pragma once

#include "Action.h"

class ActionBye : public Action
{
public:
  ActionBye();
	virtual ~ActionBye();

  ActiveAction* CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const override;
};

#endif // __Action_h__
