// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ActiveAction_h__
#define __ActiveAction_h__
#pragma once

// the parent action.
#include "Action.h"

// the clipboard data at the time we createed it.
#include "../common/clipboard.h"


class ActiveAction : public Action
{
public:
  ActiveAction(const Action& src );
	virtual ~ActiveAction();

  // ----------------------------
  const Clipboard& GetClipboard() const { return *_clipboard; }

protected:
  // the active action can only be created with an action.
  ActiveAction();

protected:
  // the current clipboard.
  Clipboard* _clipboard;

  void ClearClipboard();
  void CreateClipboard();

};

#endif // __ActiveAction_h__
