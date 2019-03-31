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
#include "../common/clipboard.h"

class IActiveAction
{
public:
  IActiveAction();

  void operator=(const IActiveAction& ) = delete;
  IActiveAction( const IActiveAction&) = delete;

	virtual ~IActiveAction() = default;

  virtual void ExecuteInThread() = 0;
  virtual bool Initialize() = 0;
  virtual bool DeInitialize() = 0;

  /**
   * \brief get the command string
   */
  virtual const std::wstring& Command() const = 0;

  /**
   * \brief the clipboard, as it was, when the action became active.
   */
  virtual const Clipboard& GetClipboard() const = 0;

  /**
   * \brief this is the command line arguments as given by the user.
   * So if the action is ""learn" and the user typed "Lea aaaa bbbb"
   * the command line is aaaa bbbb and the command is "learn"
   * \return the command line.
   */
  virtual const std::wstring& CommandLine() const = 0;

  /**
   * \brief the full filename/path and extension.
   */
  virtual const std::wstring& File() const = 0;

  /**
   * Get the window that is/was the top most at the time the command was entered.
   * return the handle of window at the time the call was made.
   */
  virtual HWND TopHWnd() const = 0;
};