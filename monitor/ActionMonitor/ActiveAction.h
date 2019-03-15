//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once

// the parent action.
#include "Action.h"
#include "IActiveAction.h"

// the clipboard data at the time we createed it.
#include "../common/clipboard.h"

class ActiveAction : public Action, public IActiveAction
{
public:
  ActiveAction(const Action& src, HWND hTopHWnd, const std::wstring& szCommandLine, bool isPrivileged);
	virtual ~ActiveAction();

  ActiveAction(const ActiveAction&) = delete;
  void operator=(const ActiveAction&) = delete;

  // ----------------------------
  const Clipboard& GetClipboard() const { return *_clipboard; }

  // ----------------------------
  void ExecuteInThread() override;
  bool Initialize() override;
  bool DeInitialize() override;

  /**
   * this is the command line arguments as given by the user.
   * So if the action is ""learn" and the user typed "Lea aaaa bbbb"
   * the command line is aaaa bbbb and the command is "learn"
   * \return the command line.
   */
  const std::wstring& CommandLine() const {
    return _szCommandLine;
  }

  /**
   * check if this is a privileged command
   * \return if the running action is privileged, (admin), or not.
   */
  bool IsPrivileged() const {
    return _isPrivileged;
  }

  /** 
   * Get the window that is/was the top most at the time the command was enteered.
   * return the handle of window at the time the call was made.
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
  // the current clipboard.
  Clipboard* _clipboard;
  std::wstring _szCommandLine;
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

  void UpdateEnvironmentVariables() const;
  void UpdateEnvironmentValue( VariableType variableType ) const;
  void UpdateEnvironmentPath() const;
  void UpdateEnvironmentTmp() const;
  void UpdateEnvironmentTemp() const;
  void UpdateEnvironmentPathExt() const;
};
