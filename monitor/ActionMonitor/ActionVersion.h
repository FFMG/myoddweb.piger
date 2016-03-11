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

  virtual bool DoIt( const STD_TSTRING& szCommandLine, bool isPrivileged);
};

#endif // __Action_h__
