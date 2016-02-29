#pragma once
/**
 * Helper function used to make new APIs easier to create and most of them more standard.
 * So if one value is returned for one API the same should happen for another API
 * This will also remove the dependencies for APIS on the main app.
 *
 * All they need is to call this helper class.
 **/
class helperapi
{
public:
  helperapi(void);
  ~helperapi(void);

public:
  // display a message onto the screen.
  bool say( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  bool say( LPCSTR msg, UINT nElapse, UINT nFadeOut);

  // get a command by index
  // return false if it does not exist
  bool getCommand( UINT idx, STD_TSTRING& sValue );

  // get the action given by the user
  // this is useful in case a plugin creates more than one action.
  bool getAction( STD_TSTRING& sValue );

  // get the number of commands.
  size_t getCommandCount();

  // execute a module and a command line
  bool execute( LPCWSTR module, LPCTSTR cmdLine, bool isPrivileged);
  bool execute( LPCSTR module, LPCSTR cmdLine, bool isPrivileged);

  // get the currently selected text, or false if none.
  bool getString (STD_TSTRING& sValue );

  // get the action monitor version number.
  bool getVersion (STD_TSTRING& sValue );
  
  // get one of x _file_ currently selected by index.
  // if 3 files and 4 folders are selected, only the files are counted.
  bool getFile   (UINT idx, STD_TSTRING& sValue );

  // get one of x _folders_ currently selected by index.
  // if 3 files and 4 folders are selected, only the folders are counted.
  bool getFolder (UINT idx, STD_TSTRING& sValue );

  // get one of x _URL_ currently selected by index.
  // note that this is only syntax check, we don't actually check if the URL resolves.
  bool getURL (UINT idx, STD_TSTRING& sValue );

  // add a set of command to the list of commands.
  // note that we do hardly any checks to see of the command already exists
  bool addAction( LPCWSTR szText, LPCWSTR szPath );
  bool addAction( LPCSTR szText, LPCSTR szPath );

  // remove an action, if more than one action is found
  // then the path will be compared against.
  bool removeAction( LPCWSTR szText, LPCWSTR szPath );
  bool removeAction( LPCSTR szText, LPCSTR szPath );

  // find an action, we return true when we find it
  // there can be more than one action with the same name.
  bool findAction( UINT idx, LPCWSTR szText, STD_TSTRING& stdPath );
  bool findAction( UINT idx, LPCSTR szText, STD_TSTRING& stdPath );
};
