// Actions.cpp: implementation of the Actions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Actions.h"
#include <io.h>

//  the Dll hooks, (for the pause)
#include "threads/lock.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------
Actions::Actions() : 
  m_uCommand( 0 ),
  m_tmpAction( nullptr ),
  m_sActionAsTyped( _T("" ) )
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
    myodd::threads::Lock guard(_mutex);

    //  delete all the pointers.
    for (array_of_actions::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
      delete *it;
    }
    m_Actions.clear();
  }

  //  clear all the searches.
  ClearSearch();
}

/**
 * Find a certain action given the name and index number.
 * This is because we can have more than one action with the same name.
 * @param UINT the index number we are looking for.
 * @param LPCTSTR the action name we are after.
 * @param MYODD_STRING& if found, the path of the action.
 * @return bool if the action was found or not.
 */
bool Actions::Find( UINT idx, LPCTSTR szText, MYODD_STRING& stdPath )
{
  if( NULL == szText )
  {
    // this is not valid
    return false;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutex);

  UINT currentIdx = 0;
  size_t size = m_Actions.size();
  for( array_of_actions_it i = m_Actions.begin(); i < m_Actions.end(); i++ )
  {
    const Action &a = *(*i);
    // is it the command we were after?
    if( myodd::strings::Compare( a.Command(), szText, false ) == 0 )
    {
      // but is it the right index?
      if( idx == currentIdx )
      {
        // this is what we were looking for.
        stdPath = a.File();
        return true;
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
Actions::array_of_actions_it Actions::Find(const MYODD_STRING& szText, const MYODD_STRING& szPath )
{
  MYODD_STRING stdExpendedPath;
  if (!myodd::files::ExpandEnvironment(szPath, stdExpendedPath))
  {
    //  probably not a valid file for us to work with.
    //  so make the 2 values the same so we can use it.
    stdExpendedPath = szPath;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutex);

  // loop around looking for the matching action.
  size_t size = m_Actions.size();
  for( array_of_actions_it i = m_Actions.begin(); i < m_Actions.end(); i++ )
  {
    const Action &a = *(*i);

    //  the command cannot be NULL
    if( myodd::strings::Compare( a.Command(), szText, false ) == 0 )
    {
      if( myodd::strings::Compare(a.File(), stdExpendedPath, false) == 0 )
      {
        return i;
      }
    }
  }
  
  //  not found
  return m_Actions.end();
}

/**
 * Add an action name and path to the vector
 * @param LPCTSTR the name of the action the user will enter.
 * @param LPCTSTR the path of the file that will be executed.
 * @return bool success or not.
 */
bool Actions::Add(LPCTSTR szText, LPCTSTR szPath)
{
  //  sanity check
  if (szText == NULL || _tcslen(szText) == 0)
  {
    return false;
  }

  Action* action = new Action(szText, szPath);
  if (!Add(action))
  {
    delete action;
    return false;
  }
  return true;
}

/**
 * Add an action name and path to the vector
 * @param const Action& the action we want to add.
 * @return bool success or not.
 */
bool Actions::Add( Action* action )
{
  if (NULL == action)
  {
    return false;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutex);

  array_of_actions_it it = Find(action->Command(), action->File() );
  if( it != m_Actions.end() )
  {
    // this item already exists
    return false;
  }
  
  // we can add the new item now
  m_Actions.push_back( action );

  // make sure we have not reached our limit this can be changed in the config file
  // but if you have more than 2048 then we might have problems with loading speed, memory and so forth.
  ASSERT( m_Actions.size() <= (size_t)::myodd::config::Get( L"paths\\maxcommand", 2048 ) );
  
  // the action was added.
  return true;
}

/**
 * Remove a single command
 * @param LPCTSTR the name of the action the user will enter.
 * @param LPCTSTR the path of the file that will be executed.
 * @return bool success or not, if it does not exit we return false.
 */
bool Actions::Remove( LPCTSTR szText, LPCTSTR szPath )
{
  //  look for this path and this command.
  array_of_actions_it it = Find( szText, szPath );
  if( it == m_Actions.end() )
  {
    // this item does not exits
    return false;
  }

  //  get the lock
  myodd::threads::Lock guard(_mutex);

  // delete the no longer used action.
  delete *it;

  // ajust erase it
  m_Actions.erase( it );

  // all good.
  return true;
}

// ------------------------------------------------------------------------------------
MYODD_STRING Actions::toChar(  const MYODD_STRING& s, const COMMANDS_VALUE& cv )
{
  if( s.length() == 0 )
  {
    return _T("");
  }

  MYODD_STRING sBold     = _T("");
  MYODD_STRING sItalic   = _T("");
  MYODD_STRING sColor    = _T("");
  
  MYODD_STRING sBold_c   = _T("");
  MYODD_STRING sItalic_c = _T("");
  
  sBold     = cv.bBold? _T("<b>"): _T("");
  sBold_c   = cv.bBold? _T("</b>"): _T("");
  
  sItalic   = cv.bItalic?_T("<i>"): _T("");
  sItalic_c = cv.bItalic?_T("</i>"): _T("");
  
  if( cv.color.length() > 0 )
  {
    sColor    = _T(" color='#") + cv.color + _T("'");;
  }
  
  MYODD_STRING ret = _T("");
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
  std::wstring fullPath = ::myodd::strings::Format( L"commands\\%s", lpName.c_str() );

  // get the value
  auto get = ::myodd::config::Get(fullPath + L".color", cv.color);
  cv.color    = static_cast<const wchar_t*>(get);
  cv.bBold    = ::myodd::config::Get( fullPath + L".bold", cv.bBold);
  cv.bItalic  = ::myodd::config::Get( fullPath + L".italic", cv.bItalic );
}

// -------------------------------------------------------------
/**
 * Get all the possible commands and match them with the string given
 * We can use common regexs to match what we are looking for
 * (.*)sometext(.*)that(.*)ignores(.*)spaces
 *
 * remember that we need to escape all the '.' that are in the commands, (file names can have special chars).
 * If a user is looking for "google.bat" are they looking for "google\.bat" or "google(.*)bat" or "google(.)bat"?
 */
MYODD_STRING Actions::toChar( ) const
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

  //  the return string
  //  we keep it as global 'cause we are returning it.
  auto szCurrentView = toChar( getActionAsTyped(), cv );
  size_t count =  0;
  for ( auto it =  m_ActionsMatch.begin(); it != m_ActionsMatch.end(); ++it )
  {
    //  we need a break after the previous line, (even for the current action)
    szCurrentView += _T("<br>");
    const Action &acc = *(*it);

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
  myodd::threads::Lock guard(_mutex);

  // reset the posible matches
  // we don't delete all the pointers.
  // because they do no belong to it.
  m_ActionsMatch.clear();

  // and the currently selected command
  m_uCommand = 0;
}

/**
 * Create a list of possible commands
 * the problem is when the user enters arguments.
 *
 * for a google command - 
 * 'goog' - would match.
 * But
 * 'goog french victories' - would not macth.
 * we would need to do a regex like {goog.*}|{french*victories}
 *
 * @param  none.
 * @return size_t the number of items that we matched.
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
  MYODD_STRING regEx( getActionAsTyped() );

  //  explode all the 'words'
  std::vector<MYODD_STRING> exploded;
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
 * Given a number of (partial)words find a list of actions that would match.
 *
 * @param std::vector<MYODD_STRING>& the list of partial|full words.
 * @return size_t the number of items we matched.
 */
size_t Actions::BuildMatchList( std::vector<MYODD_STRING>& exploded )
{
  //  get the lock
  myodd::threads::Lock guard(_mutex);

  // we only want to check the first word in the command
  // so if the user passes
  // 'go french victories' then only the command 'google' should match
  // otherwise we might match other commands, 'french' or 'victories', (there are hundreds!)
  MYODD_STRING wildAction = _T( "" );
  std::vector<MYODD_STRING> wildActions;
  for( std::vector<MYODD_STRING>::const_iterator it = exploded.begin(); it != exploded.end(); ++it )
  {
    wildActions.push_back( _T("(") + *it + _T("\\S*)") );
  }
  
  wildAction = myodd::strings::implode( wildActions, _T( "\\s+" ) );
  // make sure it is a the begining of the file.
  wildAction = _T( "^" ) + wildAction + _T( ".*" );

  //  the regex _should_ be {aaa}|{bbb} 
  //  but what we will do is search each exploded items one by one
  for ( array_of_actions::const_iterator it = m_Actions.begin(); it != m_Actions.end(); ++it )
  {
    //  get the possible command.
    const Action& acc = *(*it);
    
    //  look for that one item.
    if( myodd::strings::wildcmp( (wildAction), acc.Command()) )
    {
      m_ActionsMatch.push_back( *it );
    }
  }

  // re-order them?
  // Steve : I guess they are already in some order since they are housed
  //         in a windows folder, so perhaps no need to do any further ordering
  return m_ActionsMatch.size();
}

// -------------------------------------------------------------
void Actions::SetAction( Action* tmpAction )
{
  // Reset the commands so there is no confusion
  m_sActionAsTyped = _T("");
  BuildMatchList( );

  m_tmpAction = tmpAction;
  if( tmpAction )
  {
    m_sActionAsTyped = tmpAction->Command();
  }
}

// -------------------------------------------------------------
//  get the currently selected command
const Action* Actions::GetCommand( MYODD_STRING* cmdLine /*= NULL*/ ) const
{
  //  do we have a posible action?
  if( m_tmpAction )
  {
    return m_tmpAction;
  }

  //  do we have a potential action?
  // if not, then return NULL
  if( m_uCommand >= m_ActionsMatch.size() )
  {
    return NULL;
  }

  //  get the current action.
  const Action* action = m_ActionsMatch[ m_uCommand ];

  // Add the command line if we have anything.
  if (cmdLine == NULL)
  {
    return action;
  }
  (*cmdLine) = _T("");

  // how many words are in the command?
  LPCTSTR lpAction = action->Command().c_str();
  std::vector<MYODD_STRING> exploded;
  size_t nSize = myodd::strings::Explode( exploded, m_sActionAsTyped, _T(' ') );

  // we now need to see how many of those words are present in the select
  // so if the actual command is 
  //
  // 'google earth'
  // but the user selected the command and typed 
  // 'goo home'
  // then we need to remove only 'goo' as the other character make part of the command line.
  MYODD_STRING wildAction = _T( "" );
  std::vector<MYODD_STRING> wildActions;
  for( std::vector<MYODD_STRING>::const_iterator it = exploded.begin(); it != exploded.end(); ++it )
  {
    wildActions.push_back( _T("(") + *it + _T("\\S*)") );
    wildAction = myodd::strings::implode( wildActions, _T( "\\s+" ) );
    wildAction = _T( "^" ) + wildAction + _T( ".*" );

    if( !myodd::strings::wildcmp( (wildAction).c_str(), lpAction ) )
    {
      // this no longer match, so any words afterward make up the command.
      (*cmdLine) = wildAction = myodd::strings::implode( _T( " " ), it, exploded.end() );
      break;
    }
  }

  // trim the unneeded chars.
  myodd::strings::Trim( (*cmdLine) );

  // return what we have.
  return action;
}

// -------------------------------------------------------------
void Actions::Init()
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
  if( m_Actions.size() > (size_t)::myodd::config::Get( L"paths\\maxcommand", 2048 ) )
  {
    ASSERT( 0 ); // you have reached the limit!
                 // it is pointless to try and add more command.
    return;
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
        
        MYODD_STRING subPath( extentionPath );
        subPath += fdata.name;
        myodd::files::AddTrailingBackSlash( subPath );

        ParseDirectory( rootPath, subPath.c_str() );
        continue;
      }

      //  ok add this command
      MYODD_STRING szFullPath( directory);
      szFullPath += fdata.name;

      MYODD_STRING szName( fdata.name);
      szName = myodd::strings::lower(szName );
      
      //  if we don't want to show the extension then strip it.
      if( ::myodd::config::Get( L"commands\\show.extentions", 0 ) == false )
      {
        myodd::files::StripExtension( szName );
      }        

      Add( szName.c_str(), szFullPath.c_str() );
    }while( _tfindnext( ffhandle, &fdata ) == 0 );

    _findclose( ffhandle );
  }

  delete [] fullPath;
}

// -------------------------------------------------------------
void Actions::down()
{
  m_uCommand++;
  if(m_uCommand>=m_ActionsMatch.size())
  {
    m_uCommand = 0;
  }
}

// -------------------------------------------------------------
void Actions::up()
{
  if (m_uCommand == 0)
  {
    m_uCommand = (m_ActionsMatch.size());
  }
  m_uCommand--;
}

/**
 * Return true if this is one of the reserved directory.
 * A reserved directory is not parsed in the normal sense of the term. 
 * @param LPCTSTR the name of the directory.
 * @return bool if this is a reserved directory or not.
 */
bool Actions::IsReservedDir( LPCTSTR lp ) const
{
  if( _tcscmp(AM_DIRECTORY_IN, lp ) == 0 )        //  out directory, items that are automatically run at start time
  {
    return true;
  }
  else if( _tcscmp(AM_DIRECTORY_OUT, lp ) == 0 )  //  out directory, items that are automatically run at close time
  {
    return true;
  }
  else if( _tcscmp(AM_DIRECTORY_TMP, lp ) == 0 )  //  temp directory
  {
    return true;
  }
  else if( _tcscmp(AM_DIRECTORY_PLUGIN, lp ) == 0 )  //  plugin directory
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
  m_sActionAsTyped += c;

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
  if( m_sActionAsTyped.length() > 0 )
  {
    m_sActionAsTyped = m_sActionAsTyped.substr( 0, m_sActionAsTyped.length()-1 );

    // now that we made a small change look for possible matches
    BuildMatchList( );
  }
}

// ------------------------------------------------------------------------------------
void Actions::CurrentActionReset()
{
  m_sActionAsTyped = _T("");

  //  look for possible matches
  BuildMatchList( );
}
