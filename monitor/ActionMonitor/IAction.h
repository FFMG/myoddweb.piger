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
#include <string>
#include "IActiveAction.h"
#include "../api/IVirtualMachines.h"

class IAction
{
public:
  IAction();

  void operator=(const IAction& ) = delete;
  IAction( const IAction&) = delete;

	virtual ~IAction() = default;

  /**
   * \brief get the command string
   */
  virtual const std::wstring& Command() const = 0;

  /**
   * \brief the full filename/path and extension.
   */
  virtual const std::wstring& File() const = 0;

  // Do that action with the arguments passed
  // if we have no argument then we look in the clipboard
  virtual IActiveAction* CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, bool isPrivileged) const = 0;

  // Same as CreateActiveAction( ... ) but we don't get anything from the clipboard
  // only will use what was given to us without further checks.
  virtual IActiveAction* CreateActiveActionDirect(IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, bool isPrivileged) const = 0;
};
