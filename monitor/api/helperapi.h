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

  // display a message onto the screen.
  static bool say( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  static bool say( LPCSTR msg, UINT nElapse, UINT nFadeOut);

  // get a command by index
  // return false if it does not exist
  static bool getCommand( UINT idx, STD_TSTRING& sValue );

  // get the action given by the user
  // this is useful in case a plugin creates more than one action.
  static bool getAction( STD_TSTRING& sValue );

  // get the number of commands.
  static int getCommandCount();

  // execute a module and a command line
  static bool execute( LPCWSTR module, LPCTSTR cmdLine);
  static bool execute( LPCSTR module, LPCSTR cmdLine);

  // get the currently selected text, or false if none.
  static bool getString (STD_TSTRING& sValue );

  // get the action monitor version number.
  static bool getVersion (STD_TSTRING& sValue );
  
  // get one of x _file_ currently selected by index.
  // if 3 files and 4 folders are selected, only the files are counted.
  static bool getFile   (UINT idx, STD_TSTRING& sValue );

  // get one of x _folders_ currently selected by index.
  // if 3 files and 4 folders are selected, only the folders are counted.
  static bool getFolder (UINT idx, STD_TSTRING& sValue );

  // get one of x _URL_ currently selected by index.
  // note that this is only syntax check, we don't actually check if the URL resolves.
  static bool getURL (UINT idx, STD_TSTRING& sValue );

  // add a set of command to the list of commands.
  // note that we do hardly any checks to see of the command already exists
  static bool addAction( LPCWSTR szText, LPCWSTR szPath );
  static bool addAction( LPCSTR szText, LPCSTR szPath );

  // remove an action, if more than one action is found
  // then the path will be compared against.
  static bool removeAction( LPCWSTR szText, LPCWSTR szPath );
  static bool removeAction( LPCSTR szText, LPCSTR szPath );

  // find an action, we return true when we find it
  // there can be more than one action with the same name.
  static bool findAction( UINT idx, LPCWSTR szText, STD_TSTRING& stdPath );
  static bool findAction( UINT idx, LPCSTR szText, STD_TSTRING& stdPath );
};
