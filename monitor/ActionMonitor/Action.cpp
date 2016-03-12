// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Action.h"
#include "ActionMonitor.h"

#include "os\os.h"

/**
 * todo
 * @param void
 * @return void
 */
Action::Action()
{
  Reset();
}

/**
 * constructor
 * @param LPCTSTR the name of the action, (the the user will enter)
 * @param LPCTSTR the full path of the action that we will execute.
 * @return none
 */
Action::Action( LPCTSTR szText, LPCTSTR szPath /* = NULL  */)
{
  if( NULL == szText )
  {
    //  we cannot have NULL commands.
    throw -1;
  }

  Reset();
  m_szCommand = szText;
  
  // expand the path
  if( szPath )
  {
    LPTSTR lpExpandPath = NULL;
    if( myodd::files::ExpandEnvironment( szPath, lpExpandPath ) )
    {
      m_szFile = lpExpandPath;
      delete [] lpExpandPath;
    }
    else
    {
      // keep whatever value was given to us.
      m_szFile = szPath;
    }
  }// if we have an szPath != NULL

  // and the extension
  m_szExt = myodd::files::get_extension( m_szFile );
}

/**
 * copy constructor
 *
 * @version 0.1
 * @return none
 */
Action::Action( const Action&action)
{
  Reset();
  *this = action;
}

/**
 * copy constructor
 *
 * @version 0.1
 * @return none
 */
const Action& Action::operator=(const Action& action)
{
  if( this != &action )
  {
    m_szCommand = action.m_szCommand; 
    m_szFile    = action.m_szFile;
    m_szExt     = action.m_szExt;
  }
  return *this;
}

/**
 * Destructor
 * @param void
 * @return void
 */
Action::~Action()
{
}

/**
 * Reset the path and the command name.
 * @param void
 * @return void
 */
void Action::Reset()
{
  m_szExt = m_szFile = m_szCommand = _T("");
}

/**
 * Set the path for this action.
 * @param LPCTSTR set the path for this command.
 * @return void
 */
void Action::SetCommandPath( LPCTSTR szPath )
{
  m_szFile = szPath;
}

/**
 * Run the command, we take into account the current selection and command parameters given.
 * @param const STD_TSTRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
bool Action::DoIt(const STD_TSTRING& szCommandLine, bool isPrivileged)
{
  // this is the full command passed by the user.
  // so even if the user only typed "goo" we will return google.
  LPCTSTR command = toChar();
  
  //  not sure how to do that...
  if ( NULL == command || _tcslen(command) == 0)
  {
    return false;
  }

  // we are about to execute a command, we don't know how long the command will last
  // a badly written plugin could take forever to return.
  BOOL bThen = hook_RejectKeyboad( FALSE );

  //  if we are here then we are going to load a user command
  //
  //  If the user did not pass any arguments/command line then we must get them from the clipboard.
  bool bResult = false;
  if( szCommandLine.length() == 0 )
  {
    bResult = DoItWithNoCommandLine(isPrivileged);
  }
  else
  {
    //  so now, at last we can call the command line
    bResult = DoItDirect(szCommandLine.c_str(), isPrivileged);
  }

  // now that we are back from calling the plugin, restore the keyboard state.
  hook_RejectKeyboad( bThen );

  // return the result.
  return bResult;
}

/**
 * Try and do it when we have no command line
 * @param bool isPrivileged if this action is privileged or not.
 * @return bool success or not.
 */
bool Action::DoItWithNoCommandLine( bool isPrivileged )
{
  //  the command line we will try and make.
  STD_TSTRING szCommandLine = _T("");

  //
  //  we need to wrap the whole clipboard around try/catch as not all clipboard cases have been tested
  //  I try to restore data  to the clipboard without really knowing if the data itself is valid
  //  so that can cause some kind of problem(s)
  try
  {
    //  ask the system if anything was copied.
    //  the clipboard function will ask us to get the most likely text
    //  there will probably only be a conflict with explorer, (of any flavor)
    //  that could copy text and/or file names.
    //
    CWnd* cwnd = CActionMonitorApp::GetLastForegroundWindow();
    Clipboard clipboard( cwnd );

    //  any other values are rejected, (bitmaps and so on).
    STD_TSTRING sText = _T("");
    if (clipboard.GetTextFromClipboard(sText))
    {
      //  we need to trim all the items into one single line
      //  because command lines cannot accept multiple lines
      //
      //  or we might need to replace certain characters.
      szCommandLine = toSingleLine(sText.c_str());
    }
  }
  catch (...)
  {
    szCommandLine = _T("");
    myodd::log::LogError(_T("Critical error while trying to run an action, [%s]."), toChar() );
    _ASSERT(0);         //  the main reason for failure is probably because  
                        //  there is a format in the Clipboard that I am not handling properly
                        //  there should be a way of sending me a mail when this happens so we can look into fixing it.
  }

  // we can now do it direct.
  return DoItDirect(szCommandLine.c_str(), isPrivileged);
}

/**
 * Get a text an remove the single lines out of it/
 * TODO: Move this to the am_string namespace?
 * @param const STD_TSTRING& szCommandLine the text that we would like to enforce into a single line.
 * @return STD_TSTRING a single line of text.
 */
STD_TSTRING Action::toSingleLine( LPCTSTR sText ) const
{
  // Sanity checks.
  if( NULL == sText )
  {
    return _T("");
  }

  LPCTSTR pdest = _tcschr( sText, '\n' );
  size_t  result;

  STD_TSTRING ret( sText );
  if( pdest != NULL )
  {
    result = pdest - sText;
    ret = ret.substr( 0, result );
    return toSingleLine( ret.c_str() );
  }

  pdest = _tcschr( sText, '\r' );
  if( pdest != NULL )
  {
    result = pdest - sText;
    ret = ret.substr( 0, result );
    return toSingleLine( ret.c_str() );
  }
  return ret;
}

/**
 * Return the command as a LPCTSTR
 * @param void
 * @return LPCTSTR the command, (the name the user has to enter).
 */
LPCTSTR Action::toChar() const
{
  return m_szCommand.c_str();
}

/**
 * Execute a file.
 * We will expend all the environment variables as needed.
 * @param const std::vector<STD_TSTRING> [0] the file path, [1] the arguments to launch with, (optional).
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return bool true|false success or not.
 */
bool Action::Execute( const std::vector<STD_TSTRING>& argv, bool isPrivileged )
{
  // get the number of arguments.
  size_t argc = argv.size();

  // sanity check
  if( argc < 1 || argc > 2 )
  {
    ASSERT ( 0 ); //  wrong number of arguments.
    return false;
  }

  LPTSTR argvModule = NULL;
  LPTSTR argvCmd = NULL;

  // get the module name, (what we are running).
  // Expand the values that might have been passed.
  if( !myodd::files::ExpandEnvironment( argv[ 0 ].c_str(), argvModule ) )
  {
    return false;
  }

  // But we might also have a command line item.
  if( 2 == argc )
  {
    // Expand the values that might have been passed.
    if( !myodd::files::ExpandEnvironment( argv[ 1 ].c_str(), argvCmd ) )
    {
      delete [] argvModule;
      return false;
    }
  }

  // Assume error
  bool result = false;

  //
  // ShellExec
  //
  HINSTANCE hHinstance = NULL;
  if (isPrivileged == true && !myodd::os::IsElevated() )
  {
    hHinstance = ShellExecute( NULL,
                               _T("runas"),  //  elevate
                               argvModule,
                               argvCmd,
                               _T(""),
                               SW_SHOW
                              );
  }
  else
  {
    //  launch as a normal file.
    hHinstance = ShellExecute( GetDesktopWindow(), 
                               _T("open"), 
                               argvModule, 
                               argvCmd, 
                               _T(""),   
                               SW_SHOW
                              );
  }

  if( hHinstance > (HINSTANCE)32 )
  {
    result = true;
  }
  else
  {
    result = false;
  }

  // clean up the expended variable.
  delete [] argvCmd;
  delete [] argvModule;

  return result;
}

/**
 * Launch a single action with all the command line arguments.
 * TODO : The API calls ignore the values been passed to them, so we should first check that we have all the values.
 * @param const STD_TSTRING& szCommandLine the command and the arguments we are launching this file with.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL TRUE|FALSE success or not.
 */
bool Action::DoItDirect(const STD_TSTRING& szCommandLine, bool isPrivileged) const
{
  // sanity check
  if( 0 == m_szFile.length() )
  {
    return false;
  }
#ifdef ACTIONMONITOR_API_PLUGIN
  // Do the API calls.
  //
  if(PluginVirtualMachine::IsPluginExt( m_szExt.c_str() ) )
  {
    return DoItDirectPlugin( isPrivileged );
  }
#endif // ACTIONMONITOR_API_PLUGIN

#ifdef ACTIONMONITOR_API_LUA
  // Do the API calls.
  //
  if(LuaVirtualMachine::IsLuaExt( m_szExt.c_str() ) )
  {
    return DoItDirectLua( isPrivileged );
  }
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
  // Do the API calls.
  //
  if(PythonVirtualMachine::IsPyExt( m_szExt.c_str() ) )
  {
    return DoItDirectPython( isPrivileged );
  }
#endif // ACTIONMONITOR_API_PY

  LPCTSTR cmdLine = CommandToFile( );
  if( NULL == cmdLine )
  {
    return false;
  }

  //  join the two items together.
  std::vector<STD_TSTRING> argv;
  argv.push_back( cmdLine );
  argv.push_back( szCommandLine );
  bool bResult = Action::Execute( argv, isPrivileged);

  return bResult;
}

#ifdef ACTIONMONITOR_API_LUA
bool Action::DoItDirectLua( bool isPrivileged) const
{
  ActiveAction* activeAction = new ActiveAction(*this);
  LuaVirtualMachine* lua = App().GetLuaVirtualMachine();
  int s = lua->LoadFile(m_szFile.c_str(), activeAction);
  delete activeAction;
  return (s == 0);
}
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
bool Action::DoItDirectPython( bool isPrivileged ) const
{
  ActiveAction* activeAction = new ActiveAction(*this);
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  int s = py->LoadFile(m_szFile.c_str(), activeAction);
  delete activeAction;
  return (s == 0);
}
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_API_PLUGIN
bool Action::DoItDirectPlugin( bool isPrivileged ) const
{
  ActiveAction* activeAction = new ActiveAction(*this);
  PluginVirtualMachine* pg = App().GetPluginVirtualMachine();
  int s = pg->LoadFile(m_szFile.c_str(), activeAction);
  delete activeAction;
  return (s == 0);
}
#endif // ACTIONMONITOR_API_PLUGIN
