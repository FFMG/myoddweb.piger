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
#include "StdAfx.h"
#include "helperapi.h"

#include <locale>
#include <sstream>

#include "../ActionMonitor/ActionMonitor.h"
#include "../ActionMonitor/ActionMonitorDlg.h"

//  the clipboard code
#include "../common/Clipboard.h"

/**
 * \brief the constructor
 * \param action the action being called
 * \param actions all the actions
 * \param messagesHandler the interface to show messages
 */
HelperApi::HelperApi(const ActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler ) :
  _action( action ),
  _actions( actions),
  _messagesHandler(messagesHandler)
{
}

/**
 * \brief Get the current clipboard data for this active action.
 * \return const Clipboard& the current clipboard.
 */
const Clipboard& HelperApi::GetClipboard() const
{
  return _action.GetClipboard();
}

/**
 * \brief Display a message on the string.
 * \param sText the message we want to display.
 * \param elapseMiliSecondsBeforeFadeOut how long, (in ms), we are displaying the message for.
 * \param totalMilisecondsToShowMessage how fast we want to fade out.
 * \return bool if the message was displayed properly or if there was an error.
 */
bool HelperApi::Say(const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const
{
  return _messagesHandler.Show(sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage);
}

/**
 * \brief Get a command by index, so if the user pressed "Hello World" get each word per index.
 *        Index 0 is the action itself 
 * \param idx the command index
 * \param sValue the returned value.
 * \return boolean false if it does not exist
 */
bool HelperApi::GetCommand(const unsigned int idx, MYODD_STRING& sValue ) const
{
  try
  {
    const auto& action = &_action;
    if (nullptr == action)
    {
      //  we don't have a command.
      return false;
    }
    const auto& szCommandLine = action->CommandLine();

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
      const auto actual_idx = idx-1;

      // if the number that the user wants is within our limits then we will add it.
      if(actual_idx >= params.size() )
      {
        return false;
      }

      // set the value
      sValue = params[actual_idx].c_str();
      return true;
    }
  }
  catch(...)
  {
    // something did not work.
    sValue = _T("");
    return false;
  }
  return true;
}

/**
 * \brief Get the command line that the user tried to use
 *        for example if the command is "lean" and the user entered "Lea" 
 *        this function will return "lean"
 * \param sValue the value will be put here.
 * \return bool if we were able to get the command or not.
 */
bool HelperApi::GetAction( MYODD_STRING& sValue ) const
{
  try
  {
    //  we only need the action and not the 'active' action.
    const auto& action = &_action;
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
 * \brief Get the number of arguments that the user entered.
 *        If the Action is "Google Earth" and the user typed in "Goo Ear Home" then there is only
 * one command count, ( 'Home' );
 * @return the number of arguments.
 */
size_t HelperApi::GetCommandCount() const
{
  try
  {
    const auto& action = &_action;
    if (nullptr == action)
    {
      //  we don't have a command.
      return false;
    }

    const auto& szCommandLine = action->CommandLine();
    if( 0 == szCommandLine.length() )
    {
      return 0; //  we have no arguments.
    }

    // get the action commands and get the number of argument .
    std::vector<MYODD_STRING> params;
    return myodd::strings::Explode(  params, szCommandLine, _T(' '), MYODD_MAX_INT32, false );
  }
  catch( ... )
  {
    //  we have no command.
    return 0;
  }
}

/**
 * \brief Execute a module and a command line if the module is NULL then we try
 *        and run the command line arguments only.
 * \param module the name of the module/dll/exe we are trying to run, (can't be null)
 * \param cmdLine the command line arguments we want to run, (can't be null).
 * \param isPrivileged if we need administrator privilege to run this.
 * \param hProcess the return handle, (so we can keep track of it)
 * \return bool success or not
 */
bool HelperApi::Execute(const wchar_t* module, const wchar_t* cmdLine, const bool isPrivileged, HANDLE* hProcess) const
{
  if( nullptr == module && nullptr == cmdLine )
  {
    return false;
  }

  // prepare each items to be returned.
  // 
  std::vector<MYODD_STRING> argv;

  // we must have at least the module
  if( nullptr == module )
  {
    argv.push_back( _T("") );
  }
  else
  {
    argv.push_back( module );
  }

  if(nullptr != cmdLine )
  {
    argv.push_back( cmdLine );
  }
  
  // Execute the command +  module
  return Action::Execute( argv, isPrivileged, hProcess );
}

/**
 * \brief Get the version number of the action monitor.
 * \param sValue the return string that will contain the FULL version number.
 * \return bool true|false
 */
bool HelperApi::GetVersion (MYODD_STRING& sValue )
{
  myodd::files::Version ver;
  sValue = myodd::strings::Format( _T("%d.%d.%d.%d"),
                                   ver.GetFileVersionMajor(),
                                   ver.GetFileVersionMinor(),
                                   ver.GetFileVersionMaintenance(),
                                   ver.GetFileVersionBuild() );
  return true;
}

/**
 * \brief Get the string that is currently selected when the action was called.
 * \param sValue the return value.
 * \param bQuote if we want to quote the text or not.
 * \return bool if we have a string selected or not.
 */
bool HelperApi::GetString (MYODD_STRING& sValue, const bool bQuote) const
{
  try
  {
    const auto& clipBoard = GetClipboard( );
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
 * \brief Get a currently selected file in the clipboard.
 *        Used by plugins who want to behave a certain way for files.
 * \param idx the file number we are after
 * \param sValue the return value
 * \param bQuote if we want to quote or not.
 * \return bool success or not if there are no more files
 */
bool HelperApi::GetFile(const unsigned int idx, MYODD_STRING& sValue, const bool bQuote) const
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
 * \brief Get a currently selected URL in the clipboard.
 *        Used by plugins who want to behave a certain way for URLs.
 * \param idx the URL number we are after
 * \param sValue the return value
 * \param bQuote if we want to quote the string or not.
 * \return bool success or not if there are no more URLs
 */
bool HelperApi::GetUrl (const unsigned int idx, MYODD_STRING& sValue, const bool bQuote) const
{
  try
  {
    const auto& clipBoard = GetClipboard( );

    MYODD_STRING sClipBoard = _T("");
    if( !clipBoard.GetUrl( sClipBoard, idx, bQuote ) )
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
 * \brief Get the currently selected folder, (if any)
 *        This is used when plugins want to behave a certain way depending
 *        on the currently selected folder.
 * \param idx the folder number we are getting.
 * \param sValue the value we are after.
 * \param bQuote if we want to quote the string or not.
 * \return bool success or not, we return false when there are no more folders.
 */
bool HelperApi::GetFolder (const unsigned int idx, MYODD_STRING& sValue, const bool bQuote) const
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
 * @param szText the name of the command we want to add.
 * @param szPath the full path of the command that will be executed.
 * @return bool if the action was added properly or not.
 */
bool HelperApi::AddAction(const wchar_t* szText, const wchar_t* szPath ) const
{
  if( nullptr == szText || _tcslen(szText) == 0 )
  {
    //  this cannot be valid
    return false;
  }
  if( nullptr == szPath || _tcslen(szPath) == 0 )
  {
    //  this cannot be valid
    // only internal commands have null paths
    return false;
  }
  return _actions.Add( new Action(szText, szPath ) );
}

/**
 * \brief Remove an action, if more than one action is found
 *        Then the path will be compared against.
 * \param szText the action we want to remove
 * \param szPath the path of the action we are removing.
 * \return bool if the action was removed or not.
 */
bool HelperApi::RemoveAction(const wchar_t* szText, const wchar_t* szPath ) const
{
  if( nullptr == szText || _tcslen(szText) == 0 )
  {
    //  this cannot be valid
    return false;
  }
  if(nullptr == szPath || _tcslen(szPath) == 0 )
  {
    //  this cannot be valid
    // only internal commands have null paths
    return false;
  }
  return _actions.Remove( szText, szPath );
}

/**
 * \brief Find an action to see if it exists already
 * \param idx the index of the action we are looking for.
 * \param szText the name of the command we want to find
 * \param stdPath if the action exists, return the path for it.
 * \return if the action exits or not.
 */
bool HelperApi::FindAction(const unsigned int idx, const wchar_t* szText, std::wstring& stdPath ) const
{
  if( nullptr == szText )
  {
    return false;
  }
  const auto action = _actions.Find( szText, idx );
  if( nullptr == action )
  {
    return false;
  }

  stdPath = action->File();
  return true;
}

/** 
 * \brief Get the last foreground window. This is the window that was last on top.
 *        It is possible to return NULL if the window is not 
 * \return HWND the last top window.
 */
HWND HelperApi::GetForegroundWindow() const
{
  //  is it NULL?
  if (nullptr == _action.TopHWnd())
  {
    return nullptr;
  }

  // is it still valid?
  if ( !::IsWindow(_action.TopHWnd() ))
  {
    return nullptr;
  }

  // looks good, so we can return it.
  return _action.TopHWnd();
}

/**
 * \brief Widen a string, (from std::string to std::wstring)
 * \param str the std string
 * \return the widened string
 */
std::wstring HelperApi::Widen(const std::string& str)
{
  std::wostringstream wstm;
  const auto& ctfacet = std::use_facet< std::ctype<wchar_t> >(wstm.getloc());
  for (size_t i = 0; i<str.size(); ++i)
    wstm << ctfacet.widen(str[i]);
  return wstm.str();
}

/**
 * \brief Log a message.
 * \param logType the message type we are logging.
 * \param lpText the text we wish to log.
 * \return none.
 */
void HelperApi::Log(unsigned int logType, const wchar_t* lpText)
{
  switch ( static_cast<myodd::log::LogType>(logType) )
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