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
  ActiveAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged);
	virtual ~ActiveAction();

  // ----------------------------
  const Clipboard& GetClipboard() const { return *_clipboard; }

  // ----------------------------
  // @todo those should be pure virtual.
  virtual void ExecuteInThread();
  bool Initialize();
  bool DeInitialize();

protected:
  virtual bool OnInitialize();
  virtual bool OnDeInitialize();

private:
  ActiveAction(const ActiveAction& rhs);
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

protected:
  bool ReadFile(LPCTSTR pyFile, std::string& script) const;
};

#endif // __ActiveAction_h__
