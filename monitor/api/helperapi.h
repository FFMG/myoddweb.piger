#pragma once

#include "ActiveAction.h"

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
  HelperApi(const ActiveAction& action );
  virtual ~HelperApi();

private:
  const Clipboard& GetClipboard() const;

  /**
   * \brief the action that we are running
   */
  const ActiveAction& _action;

protected:
  // helper function
  static std::wstring Widen(const std::string& str);
  
  // display a message onto the screen.
  virtual bool Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const;
  
  // get a command by index
  // return false if it does not exist
  bool GetCommand(const unsigned int idx, MYODD_STRING& sValue ) const;

  // get the action given by the user
  // this is useful in case a plugin creates more than one action.
  bool GetAction( MYODD_STRING& sValue ) const;

  // get the number of commands.
  size_t GetCommandCount() const;

  // execute a module and a command line
  virtual bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const;

  // get the currently selected text, or false if none.
  bool GetString (MYODD_STRING& sValue, bool bQuote) const;

  // get the action monitor version number.
  static bool GetVersion (MYODD_STRING& sValue );
  
  // get one of x _file_ currently selected by index.
  // if 3 files and 4 folders are selected, only the files are counted.
  bool GetFile( unsigned int idx, MYODD_STRING& sValue, bool bQuote ) const;

  // get one of x _folders_ currently selected by index.
  // if 3 files and 4 folders are selected, only the folders are counted.
  bool GetFolder (const unsigned int idx, MYODD_STRING& sValue, bool bQuote) const;

  // get one of x _URL_ currently selected by index.
  // note that this is only syntax check, we don't actually check if the URL resolves.
  bool GetUrl (const unsigned int idx, MYODD_STRING& sValue, bool bQuote) const;

  // add a set of command to the list of commands.
  // note that we do hardly any checks to see of the command already exists
  static bool AddAction(const wchar_t* szText, const wchar_t* szPath );

  // remove an action, if more than one action is found
  // then the path will be compared against.
  static bool RemoveAction(const wchar_t* szText, const wchar_t* szPath );

  // find an action, we return true when we find it
  // there can be more than one action with the same name.
  static bool FindAction(const unsigned int idx, const wchar_t* szText, MYODD_STRING& stdPath );

  // Get the last foreground window.
  // this is the window that was in the foreground at the time the call was made.
  HWND GetForegroundWindow() const;

  // log a message to the log file.
  static void Log(unsigned int logType, const wchar_t* lpText);
};
