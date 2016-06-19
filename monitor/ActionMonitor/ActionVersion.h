// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ActionVersion_h__
#define __ActionVersion_h__
#pragma once

#include "Action.h"

class ActionVersion : public Action
{
public:
  ActionVersion();
	virtual ~ActionVersion();

  virtual ActiveAction* CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const;
};

#endif // __Action_h__
