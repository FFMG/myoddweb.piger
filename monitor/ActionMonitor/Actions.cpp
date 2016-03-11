// Actions.cpp: implementation of the Actions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Actions.h"
#include "ActionMonitor.h"
#include <io.h>

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

//  the Dll hooks, (for the pause)
#include "../hook/hook.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------
Actions::Actions() : 
  m_uCommand( 0 ),
  m_tmpAction( 0 ),
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
  //  delete all the pointers.
  for (array_of_actions::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
  {
    delete *it;
  }
  m_Actions.clear();

  //  clear all the searches.
  ClearSearch();
}

/**
 * Find a certain action given the name and index number.
 * This is because we can have more than one action with the same name.
 * @param UINT the index number we are looking for.
 * @param LPCTSTR the action name we are after.
 * @param STD_TSTRING& if found, the path of the action.
 * @return bool if the action was found or not.
 */
bool Actions::Find( UINT idx, LPCTSTR szText, STD_TSTRING& stdPath )
{
  if( NULL == szText )
  {
    // this is not valid
    return false;
  }

  UINT currentIdx = 0;
  size_t size = m_Actions.size();
  for( array_of_actions_it i = m_Actions.begin(); i < m_Actions.end(); i++ )
  {
    const Action &a = *(*i);
    // is it the command we were after?
    if( _tcsicmp( a.toChar(), szText ) == 0 )
    {
      // but is it the right index?
      if( idx == currentIdx )
      {
        // this is what we were looking for.
        LPCTSTR p = a.CommandToFile();
        stdPath = (p?p:_T(""));
        return true;
      }

      // look for the next index.
      ++currentIdx;
    }
  }

  // if we are here, we did not find the right action.
  return false;
}

// -------------------------------------------------------------
Actions::array_of_actions_it Actions::Find( LPCTSTR szText, LPCTSTR szPath )
{
  if( NULL == szText )
  {
    // this is not valid
    return m_Actions.end();
  }

  size_t size = m_Actions.size();
  for( array_of_actions_it i = m_Actions.begin(); i < m_Actions.end(); i++ )
  {
    const Action &a = *(*i);

    //  the command cannot be NULL
    if( _tcsicmp( a.toChar(), szText ) == 0 )
    {
      // But the path can be
      LPCTSTR p = a.CommandToFile();
      if( NULL == p && NULL == szPath )
      {
        return i;
      }
      else if( NULL != p && NULL != szPath && _tcsicmp( p, szPath ) == 0 )
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

  array_of_actions_it it = Find(action->toChar(), action->CommandToFile() );
  if( it != m_Actions.end() )
  {
    // this item already exists
    return false;
  }
  
  // we can add the new item now
  m_Actions.push_back( action );

  // make sure we have not reached our limit this can be changed in the config file
  // but if you have more than 2048 then we might have problems with loading speed, memory and so forth.
  ASSERT( m_Actions.size() <= (size_t)myodd::config::get( _T("paths\\maxcommand"), 2048 ) );
  
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
  array_of_actions_it it = Find( szText, szPath );
  if( it == m_Actions.end() )
  {
    // this item does not exits
    return false;
  }

  // just erase it
  m_Actions.erase( it );
  return true;
}

// ------------------------------------------------------------------------------------
STD_TSTRING Actions::toChar(  const STD_TSTRING& s, const COMMANDS_VALUE& cv )
{
  if( s.length() == 0 )
  {
    return _T("");
  }

  STD_TSTRING sBold     = _T("");
  STD_TSTRING sItalic   = _T("");
  STD_TSTRING sColor    = _T("");
  
  STD_TSTRING sBold_c   = _T("");
  STD_TSTRING sItalic_c = _T("");
  
  sBold     = cv.bBold? _T("<b>"): _T("");
  sBold_c   = cv.bBold? _T("</b>"): _T("");
  
  sItalic   = cv.bItalic?_T("<i>"): _T("");
  sItalic_c = cv.bItalic?_T("</i>"): _T("");
  
  if( cv.color.length() > 0 )
  {
    sColor    = _T(" color='#") + cv.color + _T("'");;
  }
  
  STD_TSTRING ret = _T("");
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
void Actions::GetCommandValue( LPCTSTR lpName, COMMANDS_VALUE& cv ) const
{
  STD_TSTRING sFullName = _T("commands\\");
  sFullName += lpName;

  LPCTSTR x = (LPCTSTR)myodd::config::get(sFullName + _T(".color"), cv.color);
  cv.color = x;
  cv.bBold    = myodd::config::get( sFullName+ _T(".bold"), cv.bBold);
  cv.bItalic  = myodd::config::get( sFullName+ _T(".italic"), cv.bItalic );
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
STD_TSTRING Actions::toChar( ) const
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
  STD_TSTRING szCurrentView = toChar( getActionAsTyped(), cv );
  size_t count =  0;
  for ( array_of_actions::const_iterator it =  m_ActionsMatch.begin(); it != m_ActionsMatch.end(); it++ )
  {
    //  we need a break after the previous line, (even for the current action)
    szCurrentView += _T("<br>");
    const Action &acc = *(*it);

    szCurrentView += toChar( acc.toChar(), (count==m_uCommand) ? cvSel:cvDef );
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
  STD_TSTRING regEx( getActionAsTyped() );

  //  explode all the 'words'
  std::vector<STD_TSTRING> exploded;
  myodd::strings::explode(  exploded, regEx, _T(' ') );

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
 * @param std::vector<STD_TSTRING>& the list of partial|full words.
 * @return size_t the number of items we matched.
 */
size_t Actions::BuildMatchList( std::vector<STD_TSTRING>& exploded )
{
  // we only want to check the first word in the command
  // so if the user passes
  // 'go french victories' then only the command 'google' should match
  // otherwise we might match other commands, 'french' or 'victories', (there are hundreds!)
  STD_TSTRING wildAction = _T( "" );
  std::vector<STD_TSTRING> wildActions;
  for( std::vector<STD_TSTRING>::const_iterator it = exploded.begin(); it != exploded.end(); ++it )
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
    if( myodd::strings::wildcmp( (wildAction).c_str(), acc.toChar()) )
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
    m_sActionAsTyped = tmpAction->toChar();
  }
}

// -------------------------------------------------------------
//  get the currently selected command
Action* Actions::GetCommand( STD_TSTRING* cmdLine /*= NULL*/ ) const
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
  Action* action = m_ActionsMatch[ m_uCommand ];

  // Add the command line if we have anything.
  if (cmdLine == NULL)
  {
    return action;
  }
  (*cmdLine) = _T("");

  // how many words are in the command?
  LPCTSTR lpAction = action->toChar();
  std::vector<STD_TSTRING> exploded;
  int nSize = myodd::strings::explode( exploded, m_sActionAsTyped, _T(' ') );

  // we now need to see how many of those words are present in the select
  // so if the actual command is 
  //
  // 'google earth'
  // but the user selected the command and typed 
  // 'goo home'
  // then we need to remove only 'goo' as the other character make part of the command line.
  STD_TSTRING wildAction = _T( "" );
  std::vector<STD_TSTRING> wildActions;
  for( std::vector<STD_TSTRING>::const_iterator it = exploded.begin(); it != exploded.end(); ++it )
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
  STD_TSTRING sPath = myodd::config::get( _T("paths\\commands") );
  if( myodd::files::ExpandEnvironment( sPath, sPath ) )
  {
    ParseDirectory( sPath.c_str(), _T("") );
  }
}

// -------------------------------------------------------------
void Actions::ParseDirectory( LPCTSTR rootPath, LPCTSTR extentionPath  )
{
  // make sure that we do not have too many actions
  // this is a sign that something is broken
  // or that we are reading the root directory
  if( m_Actions.size() > (size_t)myodd::config::get( _T("paths\\maxcommand"), 2048 ) )
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

  TCHAR* directory = new TCHAR[T_MAX_PATH+1];
  memset( directory, 0, T_MAX_PATH+1);

  //  copy the directory
  _tcscpy_s( directory, T_MAX_PATH+1, rootPath );  
  myodd::files::AddTrailingBackSlash( directory, T_MAX_PATH );

  // add the extension
  _tcscat_s( directory, T_MAX_PATH+1, extentionPath);
  myodd::files::AddTrailingBackSlash( directory, T_MAX_PATH );

  //
  //  we could refine the search to
  //  *.bat, *.exe, *.pl
  //  but I am not sure we really need to restrict anything
  // it is up to the user to ensure that they have 
  STD_TSTRING sPath = STD_TSTRING(directory) + STD_TSTRING( _T("*.*") );

  LPTSTR fullPath = NULL;
  if( !myodd::files::ExpandEnvironment( sPath.c_str(), fullPath ) )
  {
    return;
  }
  
  _tfinddata_t fdata;
  long ffhandle = _tfindfirst( fullPath, &fdata );
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
        
        STD_TSTRING subPath( extentionPath );
        subPath += fdata.name;
        myodd::files::AddTrailingBackSlash( subPath );

        ParseDirectory( rootPath, subPath.c_str() );
        continue;
      }

      //  ok add this command
      STD_TSTRING szFullPath( directory);
      szFullPath += fdata.name;

      STD_TSTRING szName( fdata.name);
      szName = myodd::strings::lower(szName );
      
      //  if we don't want to show the extension then strip it.
      if( myodd::config::get( _T("commands\\show.extentions"), 0 ) == false )
      {
        myodd::files::strip_extension( szName );
      }        

      Add( szName.c_str(), szFullPath.c_str() );
    }while( _tfindnext( ffhandle, &fdata ) == 0 );

    _findclose( ffhandle );
  }

  delete [] fullPath;
  delete [] directory;
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
