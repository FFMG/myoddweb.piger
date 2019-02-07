#include "StdAfx.h"
#include "helperapi.h"

#include <locale>
#include <iostream>
#include <sstream>

#include "../actionmonitor/ActionMonitor.h"
#include "../actionmonitor/ActionMonitorDlg.h"

//  the clipboard code
#include "../common/clipboard.h"

/**
 * Todo
 * @param void
 * @return void
 */
HelperApi::HelperApi(const ActiveAction& action) : _action( action )
{
}

/**
 * Todo
 * @param void
 * @return void 
 */
HelperApi::~HelperApi()
{
}

/**
 * Get the current clipboard for this active action.
 * @return const Clipboard& the current clipboard.
 */
const Clipboard& HelperApi::GetClipboard() const
{
  return _action.GetClipboard();
}

/**
 * Display a message on the string.
 * @param LPCSTR the message we want to display.
 * @param const unsigned int how long, (in ms), we are displaying the message for.
 * @param const unsigned int how fast we want to fade out.
 * @return bool if the message was displayed properly.
 */
bool HelperApi::Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const
{
  if( nullptr == msg )
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
  CActionMonitorDlg* pThis = static_cast<CActionMonitorDlg*>(App().GetMainWnd());
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
bool HelperApi::GetCommand(const unsigned int idx, MYODD_STRING& sValue )
{
  try
  {
    const ActiveAction* action = &_action;
    if (NULL == action)
    {
      //  we don't have a command.
      return false;
    }
    const MYODD_STRING& szCommandLine = action->CommandLine();

    // if the user wants command 0 then we want the full name
    // this is more consistent with the get commands normally work.
    // 
    // argument 0 in our world is really the full name of the action.
    if( idx == 0 )
    {
      sValue = action->File();
    }
    else
    {
      // get the number of elements.
      std::vector<MYODD_STRING> params;
      myodd::strings::Explode( params, szCommandLine , _T(' '), MYODD_MAX_INT32, false );

      // because the std::vector is 0 based
      // we must step the index back once to get the right number
      unsigned int actual_idx = idx-1;

      // if the number that the user wants is within our limits then we will add it.
      if(actual_idx >= params.size() )
      {
        return false;
      }

      sValue = params[actual_idx].c_str();
      return true;
    }
  }
  catch(...)
  {
    //  smething broke...
    sValue = _T("");
    return false;
  }
  return true;
}

/**
 * Get the command line that the user tried to use
 * for example if the command is "lean" and the user entered "Lea" 
 * this function will return "lean"
 * @param MYODD_STRING& sValue the value will be put here.
 * @return bool if we were able to get the command or not.
 */
bool HelperApi::GetAction( MYODD_STRING& sValue )
{
  try
  {
    //  we only need the action and not the 'active' action.
    const Action* action = &_action;
    if (nullptr == action)
    {
      //  we don't have a command.
      return false;
    }
    sValue = action->Command( );
  }
  catch(...)
  {
    // something broke...
    sValue = _T("");
    return false;
  }

  //  it worked.
  return true;
}

/**
 * Get the number of arguments that the user entered.
 * If the Action is "Google Earth" and the user typed in "Goo Ear Home" then there is only
 * one command count, ( 'Home' );
 * @return void
 */
size_t HelperApi::GetCommandCount()
{
  try
  {
    const ActiveAction* action = &_action;
    if (NULL == action)
    {
      //  we don't have a command.
      return false;
    }

    const MYODD_STRING& szCommandLine = action->CommandLine();
    if( 0 == szCommandLine.length() )
    {
      return 0; //  we have no arguments.
    }

    // get the action commands and get the number of argument .
    std::vector<MYODD_STRING> params;
    size_t nSize = myodd::strings::Explode(  params, szCommandLine, _T(' '), MYODD_MAX_INT32, false );
    
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
bool HelperApi::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const
{
  if( NULL == module && NULL == cmdLine )
  {
    return false;
  }

  // prepare each items to be returned.
  // 
  std::vector<MYODD_STRING> argv;

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
  return Action::Execute( argv, isPrivileged, hProcess );
}

/**
 * Get the version number of the action monitor.
 * @param MYODD_STRING& the return string that will contain the FULL version number.
 * @return bool true|false
 */
bool HelperApi::GetVersion (MYODD_STRING& sValue )
{
  myodd::files::Version _ver;
  sValue = myodd::strings::Format( _T("%d.%d.%d.%d"),
                                   _ver.GetFileVersionMajor(),
                                   _ver.GetFileVersionMinor(),
                                   _ver.GetFileVersionMaintenance(),
                                   _ver.GetFileVersionBuild() );
  return true;
}

/**
 * Get the string that is currently selected when the action was called.
 * @param MYODD_STRING& the return value.
 * @param bool bQuote if we want to quote the text or not.
 * @return bool if we have a string selected or not.
 */
bool HelperApi::GetString (MYODD_STRING& sValue, bool bQuote)
{
  try
  {
    const Clipboard& clipBoard = GetClipboard( );
    MYODD_STRING sClipBoard = _T("");
    if( !clipBoard.GetText( sClipBoard, bQuote ) )
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
 * @param const unsigned int the file number we are after
 * @param MYODD_STRING& the return value
 * @param bool bQuote if we want to quote or not.
 * @return bool success or not if there are no more files
 */
bool HelperApi::GetFile(const unsigned int idx, MYODD_STRING& sValue, bool bQuote)
{
  try
  {
    const Clipboard& clipBoard = GetClipboard( );

    MYODD_STRING sClipBoard = _T("");
    if( !clipBoard.GetFile( sClipBoard, idx, bQuote ) )
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
 * @param const unsigned int the URL number we are after
 * @param MYODD_STRING& the return value
 * @param bool bQuote if we want to quote the string or not.
 * @return bool success or not if there are no more URLs
 */
bool HelperApi::GetURL (const unsigned int idx, MYODD_STRING& sValue, bool bQuote)
{
  try
  {
    const Clipboard& clipBoard = GetClipboard( );

    MYODD_STRING sClipBoard = _T("");
    if( !clipBoard.GetURL( sClipBoard, idx, bQuote ) )
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
 * @param const unsigned int the folder number we are getting.
 * @param MYODD_STRING& the value we are after.
 * @param bool bQuote if we want to quote the string or not.
 * @return bool success or not, we return false when there are no more folders.
 */
bool HelperApi::GetFolder (const unsigned int idx, MYODD_STRING& sValue, bool bQuote)
{
  try
  {
    const Clipboard& clipBoard = GetClipboard( );

    MYODD_STRING sClipBoard = _T("");
    if( !clipBoard.GetFolder( sClipBoard, idx, bQuote ) )
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
bool HelperApi::AddAction(const wchar_t* szText, const wchar_t* szPath )
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
 * @param const wchar_t* the action we want to remove
 * @param const wchar_t* the path of the action we are removing.
 * @return bool if the action was removed or not.
 */
bool HelperApi::RemoveAction(const wchar_t* szText, const wchar_t* szPath )
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
 * @param const unsigned int the index of the action we are looking for.
 * @param LPCTSTR the name of the command we want to find
 * @param MYODD_STRING& if the action exists, return the path for it.
 * @return bool if the action exits or not.
 */
bool HelperApi::FindAction(const unsigned int idx, const wchar_t* szText, MYODD_STRING& stdPath )
{
  if( NULL == szText )
  {
    return false;
  }
  return App().PossibleActions().Find( idx, szText, stdPath );
}

/** 
 * Get the last foreground window. This is the window that was last on top.
 * It is posible to return NULL if the window is not 
 * @return HWND the last top window.
 */
HWND HelperApi::GetForegroundWindow() const
{
  //  is it NULL?
  if (NULL == _action.TopHWnd())
  {
    return NULL;
  }

  // is it still valid?
  if ( !::IsWindow(_action.TopHWnd() ))
  {
    return NULL;
  }

  // looks good, so we can return it.
  return _action.TopHWnd();
}

std::wstring HelperApi::widen(const std::string& str)
{
  std::wostringstream wstm;
  const std::ctype<wchar_t>& ctfacet = std::use_facet< std::ctype<wchar_t> >(wstm.getloc());
  for (size_t i = 0; i<str.size(); ++i)
    wstm << ctfacet.widen(str[i]);
  return wstm.str();
}

/**
 * Log a message.
 * @param unsigned int logType the message type we are logging.
 * @param const wchar_t* lpText the text we wish to log.
 * @return none.
 */
void HelperApi::Log(unsigned int logType, const wchar_t* lpText)
{
  switch ( (myodd::log::LogType)logType )
  {
  case myodd::log::LogType::Success:
    myodd::log::LogSuccess( L"%s", lpText);
    break;

  case myodd::log::LogType::Error:
    myodd::log::LogError(L"%s", lpText);
    break;

  case myodd::log::LogType::Warning:
    myodd::log::LogWarning(L"%s", lpText);
    break;

  case myodd::log::LogType::Message:
    myodd::log::LogMessage(L"%s", lpText);
    break;

  case myodd::log::LogType::System:
    myodd::log::LogSystem(L"%s", lpText);
    break;

  default:
    myodd::log::LogError( _T("Unknown log type: %s"), lpText );
    break;
  }
}