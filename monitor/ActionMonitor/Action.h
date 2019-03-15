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

#include <vector>
#include "../api/IVirtualMachines.h"
#include "IAction.h"
#include "IActiveAction.h"

class Action : public IAction
{
  /**
   * \brief default constructor is private
   */
  Action();

public:
  const Action& operator=(const Action& );
  Action( const Action&);
  Action( const std::wstring& szCommand, const std::wstring& szPath );

public:
	virtual ~Action() = default;

  void Reset();
  void SetCommandPath(const std::wstring& szPath );

  // Do that action with the arguments passed
  // if we have no argument then we look in the clipboard
  IActiveAction* CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, bool isPrivileged) const override;

  // Same as CreateActiveAction( ... ) but we don't get anything from the clipboard
  // only will use what was given to us without further checks.
  IActiveAction* CreateActiveActionDirect( IVirtualMachines& virtualMachines, CWnd* pWnd, const std::wstring& szCommandLine, bool isPrivileged ) const override;

  /**
   * \brief get the command string
   */
  const std::wstring& Command() const override;

  /**
   * \brief the full filename/path and extension.
   */
  const std::wstring& File() const override { return _szFile; }

  /**
   * \brief the length of the command.
   */
  size_t Len() const { return _szCommand.length();}

protected:
  IActiveAction* CreateActiveActionWithNoCommandLine(IVirtualMachines& virtualMachines, CWnd* pWnd, bool isPrivileged ) const;

  /**
   * \brief convert a multi line piece of text to a single line.
   * \param text the multi line text
   * \return the converted text
   */
  std::wstring ToSingleLine( const wchar_t* text  ) const;

public:

  static bool Execute( const std::vector<std::wstring>& argv, bool isPrivileged, HANDLE* hProcess );

private:  
  std::wstring _szCommand;  //  the command line only.
  std::wstring _szFile;    //  the full path+extension
};
