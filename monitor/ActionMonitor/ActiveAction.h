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
  ActiveAction(const Action& src, HWND hTopHWnd, const STD_TSTRING& szCommandLine, bool isPrivileged);
	virtual ~ActiveAction();

  // ----------------------------
  const Clipboard& GetClipboard() const { return *_clipboard; }

  // ----------------------------
  void ExecuteInThread();
  bool Initialize();
  bool DeInitialize();

  /**
   * this is the command line arguments as given by the user.
   * So if the action is ""learn" and the user typed "Lea aaaa bbbb"
   * the command line is aaaa bbbb and the command is "learn"
   * @return const STD_TSTRING& the command line.
   */
  const STD_TSTRING& CommandLine() const {
    return _szCommandLine;
  }

  /**
   * check if this is a priviledged command
   * @return bool
   */
  const bool IsPrivileged() const {
    return _isPrivileged;
  }

  /** 
   * Get the window that is/was the top most at the time the command was enteered.
   * return HWND the window at the time the call was made.
   */
  HWND TopHWnd() const
  {
    return _hTopHWnd;
  }

protected:
  virtual void OnExecuteInThread() = 0;
  virtual bool OnInitialize() = 0;
  virtual bool OnDeInitialize() = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(ActiveAction);

private:
  // the current clipboard.
  Clipboard* _clipboard;
  STD_TSTRING _szCommandLine;
  bool _isPrivileged;
  HWND _hTopHWnd;

  void ClearClipboard();
  void CreateClipboard();

protected:
  bool ReadFile(LPCTSTR pyFile, std::string& script) const;

private:
  enum VariableType {
    Path,
    PathExt,
    Tmp,
    Temp
  };

  void UpdateEnvironmentVariables();
  void UpdateEnvironmentValue( const VariableType variableType );
  void UpdateEnvironmentPath();
  void UpdateEnvironmentTmp();
  void UpdateEnvironmentTemp();
  void UpdateEnvironmentPathExt();
};

#endif // __ActiveAction_h__
