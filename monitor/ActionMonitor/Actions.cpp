#include "stdafx.h"
#include "Actions.h"
#include <io.h>
#include <threads/lock.h>
#include "Action.h"

Actions::Actions( IApplication& application ) :
  _application( application ),
  m_uCommand( 0 ),
  _actionCurrentlySelected( nullptr ),
  _sActionAsTyped( L"" ),
  _mutexActions( L"Actions"),
  _mutexActionTemp( L"Action - Temp"),
  _mutexActionsMatch( L"Actions - Match")
{
}

// -------------------------------------------------------------
Actions::~Actions()
{
  ClearAll();
}

// -------------------------------------------------------------
void Actions::ClearAll()
{
  {
    //  get the lock
    myodd::threads::Lock guard( _mutexActions );

    //  delete all the pointers.
    for (auto it = _actions.begin(); it != _actions.end(); ++it)
    {
      delete *it;
    }
    _actions.clear();
  }

  //  clear all the searches.
  ClearSearch();
}

/**
 * \brief Find a certain action given the name and index number.
 *        This is because we can have more than one action with the same name.
 * \param szText the action name we are after.
 * \param idx the index number we are looking for.
 * \return Action or nullptr if the action was found or not.
 */
const IAction* Actions::Find(const std::wstring& szText, unsigned int idx )
{
  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  UINT currentIdx = 0;
  for( auto it = _actions.begin(); it < _actions.end(); ++it )
  {
    const auto& action = *(*it);
    // is it the command we were after?
    if( myodd::strings::Compare(action.Command(), szText, false ) == 0 )
    {
      // but is it the right index?
      if( idx == currentIdx )
      {
        // this is what we were looking for.
        return *it;
      }

      // look for the next index.
      ++currentIdx;
    }
  }

  // if we are here, we did not find the right action.
  return nullptr;
}

/**
 * \brief ook for a command/path in the list of actions.
 * \param szText the command we are looking for.
 * \param szPath the path for that command.
 * \return Actions::array_of_actions_it the iterator for that command, (if there is one).
 */
Actions::array_of_actions_it Actions::Find(const std::wstring& szText, const std::wstring& szPath )
{
  std::wstring stdExpendedPath;
  if (!myodd::files::ExpandEnvironment(szPath, stdExpendedPath))
  {
    //  probably not a valid file for us to work with.
    //  so make the 2 values the same so we can use it.
    stdExpendedPath = szPath;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  // loop around looking for the matching action.
  for( auto it = _actions.begin(); it < _actions.end(); ++it )
  {
    const auto& a = *(*it);

    //  the command cannot be NULL
    if( myodd::strings::Compare( a.Command(), szText, false ) == 0 )
    {
      if( myodd::strings::Compare(a.File(), stdExpendedPath, false) == 0 )
      {
        return it;
      }
    }
  }
  
  //  not found
  return _actions.end();
}

/**
 * \brief Add an action name and path to the vector
 * \param action the action we want to add.
 * \return bool success or not.
 */
bool Actions::Add( IAction* action )
{
  if (nullptr == action)
  {
    myodd::log::LogError(L"Could not add action as the system passed an empty action.");
    return false;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  const auto it = Find(action->Command(), action->File() );
  if( it != _actions.end() )
  {
    // this item already exists
    myodd::log::LogWarning( L"Could not add action '%s' as it already exists.", action->Command().c_str() );
    return false;
  }
  
  // we can add the new item now
  _actions.push_back( action );

  // make sure we have not reached our limit this can be changed in the config file
  // but if you have more than 2048 then we might have problems with loading speed, memory and so forth.
  ASSERT( _actions.size() <= static_cast<size_t>(::myodd::config::Get(L"paths\\maxcommand", 2048)) );

  // all good
  myodd::log::LogSuccess( L"Added action '%s' (%s). You now have %d action(s) listed.", 
    action->Command().c_str(), 
    action->File().length() > 0 ? action->File().c_str() : L"n/a",
    _actions.size());
  
  // the action was added.
  return true;
}

/**
 * \brief try and remove a single command
 * \param szText the name of the action the user will enter.
 * \param szPath the path of the file that will be executed.
 * \return success or not, if it does not exit we return false.
 */
bool Actions::Remove(const std::wstring& szText, const std::wstring& szPath )
{
  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  //  look for this path and this command.
  const auto it = Find( szText, szPath );
  if( it == _actions.end() )
  {
    myodd::log::LogWarning(L"Tried to remove action '%s' (%s). But it is not listed.", szText.c_str(), szPath.c_str() );

    // this item does not exits
    return false;
  }

  // delete the no longer used action.
  delete *it;

  // ajust erase it
  _actions.erase( it );

  // all good
  myodd::log::LogSuccess(L"Removed action '%s' (%s). You now have %d action(s) listed.", szText.c_str(), szPath.c_str(), _actions.size());

  // all good.
  return true;
}

// ------------------------------------------------------------------------------------
const std::wstring Actions::ToChar( const IAction& givenAction, const CommandsValue& cv )
{
  if(givenAction.Command().length() == 0)
  {
    return L"";
  }
  
  const auto sBold     = cv.IsBold() ? L"<b>" : L"";
  const auto sBoldC    = cv.IsBold() ? L"</b>": L"";
  
  const auto sItalic   = cv.IsItalic() ? L"<i>" : L"";
  const auto sItalicC  = cv.IsItalic() ? L"</i>": L"";
    
  std::wstring ret = L"";
  ret += sItalic;
  ret += sBold;
  ret += givenAction.Command();
  ret += sBoldC;
  if (givenAction.Extention().length() > 0)
  {
    ret += myodd::strings::Format(L"<small style='color:#a69914'>(%s)</small>", givenAction.Extention().c_str());
  }
  ret += sItalicC;

  // add the color
  ret = myodd::strings::Format(L"<span style='color:%s'>%s</span>", cv.Color().c_str(), ret.c_str());

  return ret;
}

// ------------------------------------------------------------------------------------
const std::wstring Actions::ToChar(const std::wstring& givenAction )
{
  if (givenAction.length() == 0)
  {
    return L"";
  }

  // get the current command, colors and style
  const auto cv = GetCommandValue(L"current", L"000000", false, false);

  const auto sBold = cv.IsBold() ? L"<b>" : L"";
  const auto sBoldC = cv.IsBold() ? L"</b>" : L"";

  const auto sItalic = cv.IsItalic() ? L"<i>" : L"";
  const auto sItalicC = cv.IsItalic() ? L"</i>" : L"";

  std::wstring ret = L"";
  ret += sBold;
  ret += sItalic;
  ret += givenAction;
  ret += sItalicC;
  ret += sBoldC;

  return ret;
}

// -------------------------------------------------------------
const Actions::CommandsValue Actions::GetCommandValue(const std::wstring& lpName, const std::wstring& color, bool bold, bool italic)
{
  // get the path
  const auto fullPath = ::myodd::strings::Format( L"commands\\%s", lpName.c_str() );

  // get the value
  return CommandsValue(
    ::myodd::config::Get(fullPath + L".color", color),
    ::myodd::config::Get(fullPath + L".bold", bold ),
    ::myodd::config::Get( fullPath + L".italic", italic ));
}

// -------------------------------------------------------------
/**
 * \brief Get all the possible commands and match them with the string given
 *        We can use common regexs to match what we are looking for
 *        (.*)sometext(.*)that(.*)ignores(.*)spaces
 *
 *        remember that we need to escape all the '.' that are in the commands, (file names can have special chars).
 *        If a user is looking for "google.bat" are they looking for "google\.bat" or "google(.*)bat" or "google(.)bat"?
 */
std::wstring Actions::ToChar()
{
  // the default item
  const auto cvDef = GetCommandValue( L"default", L"#000000", false, false);

  // and the selected item
  const auto cvSel = GetCommandValue( L"selected", L"#808000", true, true );

  myodd::threads::Lock guard(_mutexActionsMatch );

  //  the return string
  //  we keep it as global 'cause we are returning it.
  auto szCurrentView = ToChar( GetActionAsTyped() );
  size_t count =  0;
  for ( const auto action : _actionsMatch )
  {
    //  we need a break after the previous line, (even for the current action)
    szCurrentView += L"<br>";
    szCurrentView += ToChar( *action, (count==m_uCommand) ? cvSel:cvDef );
    ++count;
  }
  return szCurrentView;
}

/**
 * \brief Clear all the strings/actions matches.
 */
void Actions::ClearSearch()
{
  //  get the lock
  myodd::threads::Lock guard(_mutexActionsMatch);

  // reset the possible matches
  // we don't delete all the pointers.
  // because they do no belong to it.
  _actionsMatch.clear();

  // and the currently selected command
  m_uCommand = 0;
}

/**
 * \brief Get the current action as it was entered by the user.
 *        This is the action that the user typed in.
 *        So if the action is "Google" but the user typed if "Goo" then we will only return "Goo"
 *
 * \return The current action as typed by the user.
 */
const std::wstring& Actions::GetActionAsTyped() const
{
  return _sActionAsTyped;
}

/**
 * \brief Create a list of possible commands
 *        the problem is when the user enters arguments.
 *
 *        for a google command - 
 *        'goog' - would match.
 *        But
 *        'goog french victories' - would not macth.
 *        we would need to do a regex like {goog.*}|{french*victories}
 *
 * \return the possible actions.
 */
Actions::array_of_actions Actions::GetListOfPossibleCommands( )
{
  //  we need some basic string to search
  // if we have nothing to search, then there is no point in going further.
  if( GetActionAsTyped().length() == 0 )
  {
    return array_of_actions();
  }

  //  explode all the 'words'
  std::vector<std::wstring> wordsTyped;
  myodd::strings::Explode(wordsTyped, GetActionAsTyped(), L' ' );

  // go back and look for as many matches as possible.
  // so if the command is "Google Earth" and we type "Goo Ear" we don't return commands with only "Goo" in them.
  array_of_actions aoa;
  while(wordsTyped.size() >= 1 )
  {
    // get the posible actions for the words.
    const auto possibleCommands = GetListOfPossibleCommands(wordsTyped);
    for ( auto possibleCommand = possibleCommands.begin(); possibleCommand != possibleCommands.end(); ++ possibleCommand )
    {
      if( aoa.end() != std::find( aoa.begin(), aoa.end(), *possibleCommand))
      {
        continue;
      }
      aoa.push_back( *possibleCommand );
    }

    // remove one word and find what else _could_ match.
    wordsTyped.erase(wordsTyped.end() -1 );
  }
  return aoa;
}

void Actions::RebuildPosibleListOfActions()
{
  // save our current position
// so that if the user has selected the second item in the list
// we still show the second item, even if the list has changed.
  auto uSave = m_uCommand;

  //  remove the previous list of items.
  ClearSearch();

  // get the possible list of commands.
  const auto aoa = GetListOfPossibleCommands();

  // make sure that we are withing our new limits.
  const auto matchSize = aoa.size();
  if( matchSize == 0 )
  {
    return;
  }

  uSave = uSave > matchSize ? matchSize : uSave;

  //  get the lock
  myodd::threads::Lock guard(_mutexActionsMatch);

  // reset the possible matches
  // we don't delete all the pointers.
  // because they do no belong to it.
  _actionsMatch = aoa;

  // and set it.
  m_uCommand = uSave;
}

/**
 * \brief Given a number of (partial)words find a list of actions that would/could match.
 * \param wordsTyped the list of partial|full words.
 * \return the list of actions that match our list.
 */
Actions::array_of_actions Actions::GetListOfPossibleCommands( std::vector<std::wstring>& wordsTyped)
{
  // we only want to check the first word in the command
  // so if the user passes
  // 'go french victories' then only the command 'google' should match
  // otherwise we might match other commands, 'french' or 'victories', (there are hundreds!)
  std::vector<std::wstring> wildActions;
  for(auto& wordTyped : wordsTyped)
  {
    wildActions.push_back( L"(" + wordTyped + L"\\S*)" );
  }

  // create the regex to look for the word.
  auto wildAction = myodd::strings::implode( wildActions, L"\\s+" );

  // make sure it is a the begining of the file.
  wildAction = L"^" + wildAction + L".*";

  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  //  the regex _should_ be {aaa}|{bbb} 
  //  but what we will do is search each exploded items one by one
  array_of_actions aoa;
  for (auto& action : _actions)
  {
    //  get the possible command.
    const auto& acc = *action;

    //  look for that one item.
    if (myodd::strings::wildcmp((wildAction), acc.Command()))
    {
      aoa.push_back(action);
    }
  }
  return aoa;
}

// -------------------------------------------------------------
void Actions::SetAction( IAction* tmpAction )
{
  // Reset the commands so there is no confusion
  _sActionAsTyped = L"";
  RebuildPosibleListOfActions();

  myodd::threads::Lock guardMatch(_mutexActionTemp);
  _actionCurrentlySelected = tmpAction;
  if( tmpAction )
  {
    _sActionAsTyped = tmpAction->Command();
  }
}

// -------------------------------------------------------------
//  get the currently selected command
const IAction* Actions::GetCommand()
{
  //  do we have a possible action?
  {
    myodd::threads::Lock guardMatch(_mutexActionTemp);
    if (_actionCurrentlySelected)
    {
      return _actionCurrentlySelected;
    }
  }
  myodd::threads::Lock guard(_mutexActionsMatch);

  //  do we have a potential action?
  // if not, then return NULL
  if (m_uCommand >= _actionsMatch.size())
  {
    return nullptr;
  }

  //  get the current action.
  const IAction* action = _actionsMatch[m_uCommand];
  return action;
}

std::wstring Actions::GetCommandLine()
{
  const auto action = GetCommand();
  if( nullptr == action )
  {
    return L"";
  }

  // how many words are in the command?
  const auto lpAction = action->Command().c_str();
  std::vector<std::wstring> exploded;
  myodd::strings::Explode( exploded, _sActionAsTyped, L' ' );

  // we now need to see how many of those words are present in the select
  // so if the actual command is 
  //
  // 'google earth'
  // but the user selected the command and typed 
  // 'goo home'
  // then we need to remove only 'goo' as the other character make part of the command line.
  std::wstring wildAction = L"";
  std::wstring commandLine = L"";
  std::vector<std::wstring> wildActions;
  for( auto it = exploded.begin(); it != exploded.end(); ++it )
  {
    wildActions.push_back( L"(" + *it + L"\\S*)" );
    wildAction = myodd::strings::implode( wildActions, L"\\s+" );
    wildAction = L"^" + wildAction + L".*";

    if( !myodd::strings::wildcmp( (wildAction).c_str(), lpAction ) )
    {
      // this no longer match, so any words afterward make up the command.
      commandLine = wildAction = myodd::strings::implode( L" ", it, exploded.end() );
      break;
    }
  }

  // trim the unneeded chars.
  myodd::strings::Trim( commandLine );
  return commandLine;
}

// -------------------------------------------------------------
void Actions::Initialize()
{
  //  get the root directory
  std::wstring commandsPath = ::myodd::config::Get( L"paths\\commands", L"" );
  if( myodd::files::ExpandEnvironment(commandsPath, commandsPath) )
  {
    ParseDirectory(commandsPath.c_str(), L"" );
  }
}

// -------------------------------------------------------------
void Actions::ParseDirectory(const std::wstring& rootPath, const std::wstring& extentionPath  )
{
  // make sure that we do not have too many actions
  // this is a sign that something is broken
  // or that we are reading the root directory
  {
    myodd::threads::Lock guard(_mutexActions);
    if (_actions.size() > static_cast<size_t>(::myodd::config::Get(L"paths\\maxcommand", 2048)))
    {
      ASSERT(0); // you have reached the limit!
                   // it is pointless to try and add more command.
      return;
    }
  }

  // make sure that the path that we have is valid.
  ASSERT( rootPath.length() > 0);
  if( rootPath.length() == 0)
  {
    return;
  }

  //  the end path
  std::wstring directory = rootPath;  

  // make sure it has a trailing back slash.
  myodd::files::AddTrailingBackSlash( directory );

  // add the extension
  directory += extentionPath;

  //  add a trailing back slash
  myodd::files::AddTrailingBackSlash( directory );

  //
  //  we could refine the search to
  //  *.bat, *.exe, *.pl
  //  but I am not sure we really need to restrict anything
  // it is up to the user to ensure that they have 
  const auto sPath = directory + L"*.*";

  std::wstring fullPath = L"";
  if( !myodd::files::ExpandEnvironment( sPath, fullPath ) )
  {
    return;
  }
  
  _tfinddata_t fdata;
  const auto ffhandle = _tfindfirst( fullPath.c_str(), &fdata);
  if( ffhandle != -1 )
  {
    do
    {
      if( _tcscmp(fdata.name, L"." ) == 0 || _tcscmp(fdata.name, L"..") == 0)
      {
        continue;
      }

      //  .svn directories are hidden so we might as well ignore all hidden file.
      if( (fdata.attrib & _A_HIDDEN) == _A_HIDDEN )
      {
        continue;
      }

      if( (fdata.attrib & _A_SUBDIR) == _A_SUBDIR )
      {
        if( IsReservedDir( fdata.name ) )
        {
          continue;
        }
        
        auto subPath( extentionPath + fdata.name);
        myodd::files::AddTrailingBackSlash( subPath );

        ParseDirectory( rootPath, subPath.c_str() );
        continue;
      }

      //  ok add this command
      auto szFullPath( directory);
      szFullPath += fdata.name;

      std::wstring szName( fdata.name);
      szName = myodd::strings::lower(szName );
      
      //  if we don't want to show the extension then strip it.
      if( ::myodd::config::Get( L"commands\\show.extentions", 0 ) == false )
      {
        myodd::files::StripExtension( szName );
      }        

      Add( new Action( _application ,szName, szFullPath, extentionPath));
    }while( _tfindnext( ffhandle, &fdata ) == 0 );

    _findclose( ffhandle );
  }
}

// -------------------------------------------------------------
void Actions::Down()
{
  myodd::threads::Lock guard(_mutexActionsMatch);
  m_uCommand++;
  if(m_uCommand>=_actionsMatch.size())
  {
    m_uCommand = 0;
  }
}

// -------------------------------------------------------------
void Actions::Up()
{
  myodd::threads::Lock guard(_mutexActionsMatch);
  if (m_uCommand == 0)
  {
    m_uCommand = (_actionsMatch.size());
  }
  m_uCommand--;
}

/**
 * \brief Return true if this is one of the reserved directory.
 *        A reserved directory is not parsed in the normal sense of the term. 
 * \param name the name of the directory we want to check.
 * @return bool if this is a reserved directory or not.
 */
bool Actions::IsReservedDir(const wchar_t* name ) const
{
  if( _tcscmp(AM_DIRECTORY_IN, name ) == 0 )        //  out directory, items that are automatically run at start time
  {
    return true;
  }

  if( _tcscmp(AM_DIRECTORY_OUT, name ) == 0 )  //  out directory, items that are automatically run at close time
  {
    return true;
  }

  if( _tcscmp(AM_DIRECTORY_TMP, name ) == 0 )  //  temp directory
  {
    return true;
  }
  
  if( _tcscmp(AM_DIRECTORY_PLUGIN, name ) == 0 )  //  plugin directory
  {
    return true;
  }

  return false;
}

/**
 * \brief Add a single character to the current command.
 * \param characterToAdd the character we are adding.
 */
void Actions::CurrentActionAdd(const wchar_t characterToAdd )
{
  _sActionAsTyped += characterToAdd;

  //  look for possible matches
  RebuildPosibleListOfActions();
}

/**
 * \brief Step back/remove one single character.
 */
void Actions::CurrentActionBack( )
{
  // don't step back too much.
  if( _sActionAsTyped.length() > 0 )
  {
    _sActionAsTyped = _sActionAsTyped.substr( 0, _sActionAsTyped.length()-1 );

    // now that we made a small change look for possible matches
    RebuildPosibleListOfActions( );
  }
}

// ------------------------------------------------------------------------------------
void Actions::CurrentActionReset()
{
  _sActionAsTyped = L"";

  //  look for possible matches
  RebuildPosibleListOfActions( );
}
