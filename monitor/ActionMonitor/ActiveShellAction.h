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
#include "ActiveAction.h"

class ActiveShellAction final : public ActiveAction
{
public:
  /**
   * \brief The Shell constructor
   * \param src the action that is now active.
   * \param hTopHWnd the window that was on top at the time the command was given.
   * \param szCommandLine the given command line that is, the words after the command itself
   * \param isPrivileged if this action is privileged or not.
   */
  ActiveShellAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged);

  /**
   * \brief default destructor
   */
  virtual ~ActiveShellAction() = default;

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;
};

