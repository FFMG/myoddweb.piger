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
  ActiveAction(const ActiveAction& rhs);
  ActiveAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged);
	virtual ~ActiveAction();

  // ----------------------------
  const Clipboard& GetClipboard() const { return *_clipboard; }

  void operator()();
  const ActiveAction& operator=(const ActiveAction& rhs);

protected:
  // we cannot call this directly.
  ActiveAction();

protected:
  // the current clipboard.
  Clipboard* _clipboard;
  STD_TSTRING _szCommandLine;
  bool _isPrivileged;

  void ClearClipboard();
  void CreateClipboard();

#ifdef ACTIONMONITOR_API_PLUGIN
  void DoItDirectPlugin() const;
#endif

#ifdef ACTIONMONITOR_API_PY
  void DoItDirectPython() const;
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_API_LUA
  void DoItDirectLua() const; 
#endif // ACTIONMONITOR_API_LUA
};

#endif // __ActiveAction_h__
