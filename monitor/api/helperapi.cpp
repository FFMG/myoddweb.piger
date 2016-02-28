#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "helperapi.h"
#include "../actionmonitor/ActionMonitor.h"
#include "../actionmonitor/ActionMonitorDlg.h"

//  the clipboard code
#include "../common/clipboard.h"

/**
 * Todo
 * @param void
 * @return void
 */
helperapi::helperapi(void)
{
}

/**
 * Todo
 * @param void
 * @return void 
 */
helperapi::~helperapi(void)
{
}

/**
 * Display a message on the string.
 * @param LPCSTR the message we want to display.
 * @param UINT how long, (in ms), we are displaying the message for.
 * @param UINT how fast we want to fade out.
 * @return bool if the message was displayed properly.
 */
bool helperapi::say( LPCSTR msg, UINT nElapse, UINT nFadeOut)
{
  USES_CONVERSION;
  return say( A2T(msg), nElapse, nFadeOut );
}

/**
 * Display a message on the string.
 * @param LPCSTR the message we want to display.
 * @param UINT how long, (in ms), we are displaying the message for.
 * @param UINT how fast we want to fade out.
 * @return bool if the message was displayed properly.
 */
bool helperapi::say( LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  if( NULL == msg )
  {
    return false;
  }

  if( 0 == nElapse )
  {
    return false;
  }

  // it is possible for a rogue thread to try and display a message
  // even after we have shutdown everything.
  // if the dlg is no longer active then we will not display anything.
  CActionMonitorDlg* pThis = (CActionMonitorDlg*)App().GetMainWnd();
  if( !CActionMonitorDlg::IsRunning() )
  {
    return false;
  }

  // simply display the message.
  pThis->DisplayMessage( msg, nElapse, nFadeOut );
  return true;
}

/**
 * Get a command by index
 * @param void
 * @param void
 * @param void
 * @return boolean false if it does not exist
 */
bool helperapi::getCommand( UINT idx, STD_TSTRING& sValue )
{
  try
  {
    STD_TSTRING szCommandLine = _T( "" );
    const Action& action = App().PossibleActions().getCommand( &szCommandLine );

    // if the user wants command 0 then we want the full name
    // this is more consistent with the get commands normally work.
    // 
    // argument 0 in our world is really the full name of the action.
    if( idx == 0 )
    {
      sValue = action.CommandToFile( );
    }
    else
    {
      // get the number of elements.
      std::vector<STD_TSTRING> params;
      myodd::strings::explode( params, szCommandLine , _T(' '), -1, false );

      // because the std::vector is 0 based
      // we must step the index back once to get the right number
      --idx;

      // if the number that the user wants is within our limits then we will add it.
      if( idx >= params.size() )
      {
        return false;
      }

      sValue = params[ idx ].c_str();
      return true;
    }
  }
  catch(...)
  {
    //  we don't have a command.
    sValue = _T("");
    return false;
  }
  return true;
}

/**
 * Todo
 * @param void
 * @return void
 */
bool helperapi::getAction( STD_TSTRING& sValue )
{
  try
  {
    const Action& action = App().PossibleActions().getCommand();
    sValue = action.toChar( );
  }
  catch(...)
  {
    //  we don't have a command.
    sValue = _T("");
    return false;
  }

  return true;
}

/**
 * Get the number of arguments that the user entered.
 * If the Action is "Google Earth" and the user typed in "Goo Ear Home" then there is only
 * one command count, ( 'Home' );
 * @return void
 */
size_t helperapi::getCommandCount()
{
  try
  {
    STD_TSTRING szCommandLine = _T( "" );
    const Action& action = App().PossibleActions().getCommand( &szCommandLine );

    if( 0 == szCommandLine.length() )
    {
      return 0; //  we have no arguments.
    }

    // get the action commands and get the number of argument .
    std::vector<STD_TSTRING> params;
    size_t nSize = myodd::strings::explode(  params, szCommandLine, _T(' '), -1, false );
    
    // return the number of arguments we have.
    return nSize;
  }
  catch( ... )
  {
    //  we have no command.
    return 0;
  }
}

/**
 * Execute a module and a command line if the module is NULL then we try
 * and run the command line arguments only.
 *
 * @param LPCTSTR | NULL the name of the module/dll/exe we are trying to run
 * @param LPCTSTR | NULL the command line arguments we want to run.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return bool success or not
 */
bool helperapi::execute( LPCSTR module, LPCSTR cmdLine, bool isPrivileged)
{
  USES_CONVERSION;
  return execute( A2T(module), A2T(cmdLine), isPrivileged );
}

/**
 * Execute a module and a command line if the module is NULL then we try
 * and run the command line arguments only.
 *
 * @param LPCTSTR | NULL the name of the module/dll/exe we are trying to run
 * @param LPCTSTR | NULL the command line arguments we want to run.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return bool success or not
 */
bool helperapi::execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged )
{
  if( NULL == module && NULL == cmdLine )
  {
    return false;
  }

  // prepare each items to be returned.
  // 
  std::vector<STD_TSTRING> argv;

  // we must have at least the module
  if( NULL == module )
  {
    argv.push_back( _T("") );
  }
  else
  {
    argv.push_back( module );
  }

  if( NULL != cmdLine )
  {
    argv.push_back( cmdLine );
  }
  
  // Execute the command +  module
  return Action::Execute( argv, isPrivileged );
}

/**
 * Get the version number of the action monitor.
 * @param STD_TSTRING& the return string that will contain the FULL version number.
 * @return bool true|false
 */
bool helperapi::getVersion (STD_TSTRING& sValue )
{
  myodd::files::Version _ver;
  sValue = myodd::strings::ToStringFmt( _T("%d.%d.%d.%d"),
                                        _ver.GetFileVersionMajor(),
                                        _ver.GetFileVersionMinor(),
                                        _ver.GetFileVersionMaintenance(),
                                        _ver.GetFileVersionBuild() );
  return true;
}

/**
 * Get the string that is currently selected when the action was called.
 * @param STD_TSTRING& the return value.
 * @return bool if we have a string selected or not.
 */
bool helperapi::getString (STD_TSTRING& sValue )
{
  try
  {
    Action& aCommand = App().PossibleActions().getCommand();
    const Clipboard& clipBoard = aCommand.GetClipBoard( );
    STD_TSTRING sClipBoard = _T("");
    if( !clipBoard.GetText( sClipBoard ) )
    {
      // we have nothing.
      return false;
    }
  
    if( sClipBoard.length() == 0 )
    {
      // we have something but the size is 0
      return false;
    }

    // we do have something.
    sValue = sClipBoard;
    return true;
  }
  catch(... )
  {
    return false;
  }
}

/**
 * Get a currently selected file in the clipboard.
 * Used by plugins who want to behave a certain way for files.
 * @param UINT the file number we are after
 * @param STD_TSTRING& the return value
 * @return bool success or not if there are no more files
 */
bool helperapi::getFile(UINT idx, STD_TSTRING& sValue )
{
  try
  {
    Action& aCommand = App().PossibleActions().getCommand();
    const Clipboard& clipBoard = aCommand.GetClipBoard( );

    STD_TSTRING sClipBoard = _T("");
    if( !clipBoard.GetFileNames( sClipBoard, idx ) )
    {
      // could not find anything
      return false;
    }

    // we found one string.
    sValue = sClipBoard;
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

/**
 * Get a currently selected URL in the clipboard.
 * Used by plugins who want to behave a certain way for URLs.
 * @param UINT the URL number we are after
 * @param STD_TSTRING& the return value
 * @return bool success or not if there are no more URLs
 */
bool helperapi::getURL (UINT idx, STD_TSTRING& sValue )
{
  try
  {
    Action& aCommand = App().PossibleActions().getCommand();
    const Clipboard& clipBoard = aCommand.GetClipBoard( );

    STD_TSTRING sClipBoard = _T("");
    if( !clipBoard.GetURL( sClipBoard, idx ) )
    {
      // could not find anything
      return false;
    }

    // otherwise push the string
    sValue = sClipBoard;

    // we have one item
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

/**
 * Get the currently selected folder, (if any)
 * This is used when plugins want to behave a certain way depending
 * on the currently selected folder.
 * @param UINT the folder number we are getting.
 * @param STD_TSTRING& the value we are after.
 * @return bool success or not, we return false when there are no more folders.
 */
bool helperapi::getFolder (UINT idx, STD_TSTRING& sValue )
{
  try
  {
    Action& aCommand = App().PossibleActions().getCommand();
    const Clipboard& clipBoard = aCommand.GetClipBoard( );

    STD_TSTRING sClipBoard = _T("");
    if( !clipBoard.GetFolder( sClipBoard, idx ) )
    {
      // could not find anything
      return false;
    }

    // otherwise push the string
    sValue = sClipBoard;

    // we have one item
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param LPCTSTR the name of the command we want to add.
 * @param LPCTSTR the full path of the command that will be executed.
 * @return bool if the action was added properly or not.
 */
bool helperapi::addAction( LPCSTR szText, LPCSTR szPath )
{
  USES_CONVERSION;
  return addAction( A2T(szText), A2T(szPath) );
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param LPCTSTR the name of the command we want to add.
 * @param LPCTSTR the full path of the command that will be executed.
 * @return bool if the action was added properly or not.
 */
bool helperapi::addAction( LPCWSTR szText, LPCWSTR szPath )
{
  if( NULL == szText || _tcslen(szText) == 0 )
  {
    //  this cannot be valid
    return false;
  }
  if( NULL == szPath || _tcslen(szPath) == 0 )
  {
    //  this cannot be valid
    // only internal commands have null paths
    return false;
  }
  return App().PossibleActions().Add( szText, szPath );
}

/**
 * Remove an action, if more than one action is found
 * Then the path will be compared against.
 * @param LPCWSTR the action we want to remove
 * @param LPCWSTR the path of the action we are removing.
 * @return bool if the action was removed or not.
 */
bool helperapi::removeAction( LPCSTR szText, LPCSTR szPath )
{
  USES_CONVERSION;
  return removeAction( A2T(szText), A2T(szPath) );
}

/**
 * Remove an action, if more than one action is found
 * Then the path will be compared against.
 * @param LPCWSTR the action we want to remove
 * @param LPCWSTR the path of the action we are removing.
 * @return bool if the action was removed or not.
 */
bool helperapi::removeAction( LPCWSTR szText, LPCWSTR szPath )
{
  if( NULL == szText || _tcslen(szText) == 0 )
  {
    //  this cannot be valid
    return false;
  }
  if( NULL == szPath || _tcslen(szPath) == 0 )
  {
    //  this cannot be valid
    // only internal commands have null paths
    return false;
  }
  return App().PossibleActions().Remove( szText, szPath );
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param UINT the index of the action we are looking for.
 * @param LPCTSTR the name of the command we want to find
 * @param STD_TSTRING& if the action exists, return the path for it.
 * @return bool if the action exits or not.
 */
bool helperapi::findAction( UINT idx, LPCSTR szText, STD_TSTRING& stdPath )
{
  USES_CONVERSION;
  return findAction( idx, T_A2T(szText), stdPath );
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param UINT the index of the action we are looking for.
 * @param LPCTSTR the name of the command we want to find
 * @param STD_TSTRING& if the action exists, return the path for it.
 * @return bool if the action exits or not.
 */
bool helperapi::findAction( UINT idx, LPCWSTR szText, STD_TSTRING& stdPath )
{
  if( NULL == szText )
  {
    return false;
  }
  return App().PossibleActions().Find( idx, szText, stdPath );
}
