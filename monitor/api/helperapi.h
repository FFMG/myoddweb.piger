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

#include "IActiveAction.h"
#include "IMessagesHandler.h"
#include "IApplication.h"
#include "IAction.h"
#include "../common/clipboard.h"

/**
 * Helper function used to make new APIs easier to create and most of them more standard.
 * So if one value is returned for one API the same should happen for another API
 * This will also remove the dependencies for APIS on the main app.
 *
 * All they need is to call this helper class.
 **/
class HelperApi
{
public:
  explicit HelperApi( const IActiveAction& action, IApplication& application, IMessagesHandler& messages );
  virtual ~HelperApi() = default;

private:
  const Clipboard& GetClipboard() const;

  /**
   * \brief the action that we are running
   */
  const IActiveAction& _action;

  /**
   * \brief the application.
   */
  IApplication& _application;

  /**
   * \brief the messages handler
   */
  IMessagesHandler& _messagesHandler;

protected:
  // helper function
  static std::wstring Widen(const std::string& str);
  
  // display a message onto the screen.
  virtual bool Say(const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const;
  
  // get a command by index
  // return false if it does not exist
  bool GetCommand( unsigned int idx, std::wstring& sValue ) const;

  // get the action given by the user
  // this is useful in case a plugin creates more than one action.
  bool GetAction( std::wstring& sValue ) const;

  // get the number of commands.
  unsigned int GetCommandCount() const;

  // execute a module and a command line
  virtual bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const;

  // Get the last foreground window.
  // this is the window that was in the foreground at the time the call was made.
  virtual HWND GetForegroundWindow() const;

  // get the currently selected text, or false if none.
  virtual bool GetString (std::wstring& sValue, bool bQuote) const;
  
  // get one of x _file_ currently selected by index.
  // if 3 files and 4 folders are selected, only the files are counted.
  virtual bool GetFile( unsigned int idx, std::wstring& sValue, bool bQuote ) const;

  // get one of x _folders_ currently selected by index.
  // if 3 files and 4 folders are selected, only the folders are counted.
  virtual bool GetFolder ( unsigned int idx, std::wstring& sValue, bool bQuote) const;

  // get one of x _URL_ currently selected by index.
  // note that this is only syntax check, we don't actually check if the URL resolves.
  virtual bool GetUrl ( unsigned int idx, std::wstring& sValue, bool bQuote) const;

  // get the action monitor version number.
  static bool GetVersion(std::wstring& sValue);

  // add a set of command to the list of commands.
  // note that we do hardly any checks to see of the command already exists
  bool AddAction(const std::wstring& szText, const std::wstring& szPath ) const;

  // remove an action, if more than one action is found
  // then the path will be compared against.
  bool RemoveAction(const std::wstring& szText, const std::wstring& szPath ) const;

  // find an action, we return true when we find it
  // there can be more than one action with the same name.
  const IAction* FindAction( unsigned int idx, const std::wstring& szText ) const;

  // log a message to the log file.
  static void Log(unsigned int logType, const wchar_t* lpText );
};
