#pragma once

// the parent action.
#include "Action.h"

// the clipboard data at the time we createed it.
#include "../common/clipboard.h"
#include "../api/IVirtualMachines.h"

class ActiveAction : public Action
{
public:
  ActiveAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);
	virtual ~ActiveAction();

  DISALLOW_COPY_AND_ASSIGN(ActiveAction);

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
   * \return const MYODD_STRING& the command line.
   */
  const MYODD_STRING& CommandLine() const {
    return _szCommandLine;
  }

  /**
   * check if this is a privileged command
   * \return bool
   */
  bool IsPrivileged() const {
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

protected:
  IVirtualMachines& _virtualMachines;

private:
  // the current clipboard.
  Clipboard* _clipboard;
  MYODD_STRING _szCommandLine;
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
  void UpdateEnvironmentValue( const VariableType variableType ) const;
  void UpdateEnvironmentPath();
  void UpdateEnvironmentTmp();
  void UpdateEnvironmentTemp();
  void UpdateEnvironmentPathExt() const;
};
