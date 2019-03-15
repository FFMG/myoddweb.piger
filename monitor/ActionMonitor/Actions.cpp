#include "stdafx.h"
#include "Actions.h"
#include <io.h>
#include <threads/lock.h>

Actions::Actions() : 
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
  const auto size = _actions.size();
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
  return false;
}

/**
 * Look for a command/path in the list of actions.
 * @param const MYODD_STRING& szText the command we are looking for.
 * @param const MYODD_STRING& szPath the path for that command.
 * @return Actions::array_of_actions_it the iterator for that command, (if there is one).
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
  auto size = _actions.size();
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
    return false;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  const auto it = Find(action->Command(), action->File() );
  if( it != _actions.end() )
  {
    // this item already exists
    return false;
  }
  
  // we can add the new item now
  _actions.push_back( action );

  // make sure we have not reached our limit this can be changed in the config file
  // but if you have more than 2048 then we might have problems with loading speed, memory and so forth.
  ASSERT( _actions.size() <= static_cast<size_t>(::myodd::config::Get(L"paths\\maxcommand", 2048)) );
  
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
    // this item does not exits
    return false;
  }

  // delete the no longer used action.
  delete *it;

  // ajust erase it
  _actions.erase( it );

  // all good.
  return true;
}

// ------------------------------------------------------------------------------------
std::wstring Actions::toChar(  const std::wstring& s, const COMMANDS_VALUE& cv )
{
  if( s.length() == 0 )
  {
    return _T("");
  }

  std::wstring sBold     = _T("");
  std::wstring sItalic   = _T("");
  std::wstring sColor    = _T("");
  
  std::wstring sBold_c   = _T("");
  std::wstring sItalic_c = _T("");
  
  sBold     = cv.bBold? _T("<b>"): _T("");
  sBold_c   = cv.bBold? _T("</b>"): _T("");
  
  sItalic   = cv.bItalic?_T("<i>"): _T("");
  sItalic_c = cv.bItalic?_T("</i>"): _T("");
  
  if( cv.color.length() > 0 )
  {
    sColor    = _T(" color='#") + cv.color + _T("'");;
  }
  
  std::wstring ret = _T("");
  //ret += _T("<font") + sColor + _T(">");
  ret +=  sBold;
  ret +=    sItalic;
  ret +=    s;
  ret +=    sItalic_c;
  ret +=  sBold_c;
  //ret +=  _T("</font>");

  return ret;
}

// -------------------------------------------------------------
void Actions::GetCommandValue(const std::wstring& lpName, COMMANDS_VALUE& cv ) const
{
  // get the path
  const auto fullPath = ::myodd::strings::Format( L"commands\\%s", lpName.c_str() );

  // get the value
  const auto get = ::myodd::config::Get(fullPath + L".color", cv.color);
  cv.color    = static_cast<const wchar_t*>(get);
  cv.bBold    = ::myodd::config::Get( fullPath + L".bold", cv.bBold);
  cv.bItalic  = ::myodd::config::Get( fullPath + L".italic", cv.bItalic );
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
  //  get the current command, colors and style
  COMMANDS_VALUE cv( _T("000000"), 0, 0 );
  GetCommandValue( _T("current"), cv );

  // the default item
  COMMANDS_VALUE cvDef( _T("000000"), 0, 0 );
  GetCommandValue( _T("default"), cvDef );

  // and the selected item
  COMMANDS_VALUE cvSel( _T("808000"), 1, 1 );
  GetCommandValue( _T("selected"), cvSel );

  myodd::threads::Lock guard(_mutexActionsMatch );

  //  the return string
  //  we keep it as global 'cause we are returning it.
  auto szCurrentView = toChar( getActionAsTyped(), cv );
  size_t count =  0;
  for ( auto it =  _actionsMatch.begin(); it != _actionsMatch.end(); ++it )
  {
    //  we need a break after the previous line, (even for the current action)
    szCurrentView += _T("<br>");
    const auto& acc = *(*it);

    szCurrentView += toChar( acc.Command(), (count==m_uCommand) ? cvSel:cvDef );
    ++count;
  }
  return szCurrentView;
}

/**
 * Clear all the strings/actions matches.
 * @param none
 * @return none
 */
void Actions::ClearSearch()
{
  //  get the lock
  myodd::threads::Lock guard(_mutexActionsMatch);

  // reset the posible matches
  // we don't delete all the pointers.
  // because they do no belong to it.
  _actionsMatch.clear();

  // and the currently selected command
  m_uCommand = 0;
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
 * \return size_t the number of items that we matched.
 */
size_t Actions::BuildMatchList( )
{
  // save our current position
  // so that if the user has selected the second item in the list
  // we still show the second item, even if the list has changed.
  size_t uSave = m_uCommand;

  //  remove the previous list of items.
  ClearSearch();

  //  we need some basic string to search
  // if we have nothing to search, then there is no point in going further.
  if( getActionAsTyped().length() == 0 )
  {
    return 0;
  }

  // Prepare the regex string.
  const auto regEx( getActionAsTyped() );

  //  explode all the 'words'
  std::vector<std::wstring> exploded;
  myodd::strings::Explode(  exploded, regEx, _T(' ') );

  // go back and look for as many matches as possible.
  // so if the command is "Google Earth" and we type "Goo Ear" we don't return commands with only "Goo" in them.
  size_t matchSize = 0;

  while( matchSize == 0 && exploded.size() >= 1 )
  {
    matchSize = BuildMatchList( exploded );
    if( 0 == matchSize )
    {
      exploded.erase( exploded.end() -1 );
    }
  }

  // make sure that we are withing our new limits.
  while( uSave > matchSize )
  {
    --uSave;
  }

  // and set it.
  m_uCommand = uSave;
  return matchSize;
}

/**
 * \brief Given a number of (partial)words find a list of actions that would match.
 * \param exploded the list of partial|full words.
 * \return the number of items we matched.
 */
size_t Actions::BuildMatchList( std::vector<std::wstring>& exploded )
{
  // we only want to check the first word in the command
  // so if the user passes
  // 'go french victories' then only the command 'google' should match
  // otherwise we might match other commands, 'french' or 'victories', (there are hundreds!)
  std::wstring wildAction = L"";
  std::vector<std::wstring> wildActions;
  for( auto it = exploded.begin(); it != exploded.end(); ++it )
  {
    wildActions.push_back( _T("(") + *it + _T("\\S*)") );
  }
  
  wildAction = myodd::strings::implode( wildActions, _T( "\\s+" ) );
  // make sure it is a the begining of the file.
  wildAction = _T( "^" ) + wildAction + _T( ".*" );

  auto size = 0;
  //  get the lock
  myodd::threads::Lock guard(_mutexActions);

  //  the regex _should_ be {aaa}|{bbb} 
  //  but what we will do is search each exploded items one by one
  for (auto it = _actions.begin(); it != _actions.end(); ++it)
  {
    ++size;

    //  get the possible command.
    const auto& acc = *(*it);

    //  look for that one item.
    if (myodd::strings::wildcmp((wildAction), acc.Command()))
    {
      myodd::threads::Lock guardMatch(_mutexActionsMatch);
      _actionsMatch.push_back(*it);
    }
  }
  return size;
}

// -------------------------------------------------------------
void Actions::SetAction( IAction* tmpAction )
{
  // Reset the commands so there is no confusion
  _sActionAsTyped = L"";
  BuildMatchList( );

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
  //  do we have a posible action?
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
  const auto nSize = myodd::strings::Explode( exploded, _sActionAsTyped, L' ' );

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
    wildActions.push_back( _T("(") + *it + _T("\\S*)") );
    wildAction = myodd::strings::implode( wildActions, _T( "\\s+" ) );
    wildAction = _T( "^" ) + wildAction + _T( ".*" );

    if( !myodd::strings::wildcmp( (wildAction).c_str(), lpAction ) )
    {
      // this no longer match, so any words afterward make up the command.
      commandLine = wildAction = myodd::strings::implode( _T( " " ), it, exploded.end() );
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
    ParseDirectory(commandsPath.c_str(), _T("") );
  }
}

// -------------------------------------------------------------
void Actions::ParseDirectory( LPCTSTR rootPath, LPCTSTR extentionPath  )
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
  ASSERT( _tcslen( rootPath ) > 0 );
  if( _tcslen( rootPath ) == 0 )
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
  auto sPath = directory + _T("*.*");

  LPTSTR fullPath = NULL;
  if( !myodd::files::ExpandEnvironment( sPath.c_str(), fullPath ) )
  {
    return;
  }
  
  _tfinddata_t fdata;
  intptr_t ffhandle = _tfindfirst( fullPath, &fdata );
  if( ffhandle != -1 )
  {
    do
    {
      if( _tcscmp(fdata.name, _T(".") ) == 0 || _tcscmp(fdata.name, _T("..")) == 0)
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
        
        std::wstring subPath( extentionPath );
        subPath += fdata.name;
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

      Add( new Action( szName, szFullPath));
    }while( _tfindnext( ffhandle, &fdata ) == 0 );

    _findclose( ffhandle );
  }

  delete [] fullPath;
}

// -------------------------------------------------------------
void Actions::down()
{
  myodd::threads::Lock guard(_mutexActionsMatch);
  m_uCommand++;
  if(m_uCommand>=_actionsMatch.size())
  {
    m_uCommand = 0;
  }
}

// -------------------------------------------------------------
void Actions::up()
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
  else if( _tcscmp(AM_DIRECTORY_OUT, name ) == 0 )  //  out directory, items that are automatically run at close time
  {
    return true;
  }
  else if( _tcscmp(AM_DIRECTORY_TMP, name ) == 0 )  //  temp directory
  {
    return true;
  }
  else if( _tcscmp(AM_DIRECTORY_PLUGIN, name ) == 0 )  //  plugin directory
  {
    return true;
  }
  return false;
}

/**
 * Add a single character to the current command.
 * @param TCHAR the character we are adding.
 * @return none
 */
void Actions::CurrentActionAdd( TCHAR c )
{
  _sActionAsTyped += c;

  //  look for possible matches
  BuildMatchList( );
}

/**
 * Step back/remove one single character.
 * @param none
 * @return none
 */
void Actions::CurrentActionBack( )
{
  // don't step back too much.
  if( _sActionAsTyped.length() > 0 )
  {
    _sActionAsTyped = _sActionAsTyped.substr( 0, _sActionAsTyped.length()-1 );

    // now that we made a small change look for possible matches
    BuildMatchList( );
  }
}

// ------------------------------------------------------------------------------------
void Actions::CurrentActionReset()
{
  _sActionAsTyped = _T("");

  //  look for possible matches
  BuildMatchList( );
}
