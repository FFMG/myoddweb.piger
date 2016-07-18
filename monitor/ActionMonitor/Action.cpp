// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Action.h"
#include "ActionMonitor.h"
#include "activeaction.h"
#include "activepythonaction.h"
#include "activeluaaction.h"
#include "activepluginaction.h"
#include "activedefaultaction.h"
#include "activebatchaction.h"

#include "os\os.h"
#include "ActivePowershellAction.h"

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
 * @param LPCTSTR szCommand the name of the action, (the the user will enter)
 * @param LPCTSTR the full path of the action that we will execute.
 * @return none
 */
Action::Action( LPCTSTR szCommand, LPCTSTR szPath /* = nullptr  */)
{
  if(nullptr == szCommand)
  {
    //  we cannot have nullptr commands.
    throw -1;
  }

  //  reset everything
  Reset();

  // set the command and make sure it is valid.
  _szCommand = szCommand;
  myodd::strings::Trim(_szCommand);

  //  make sure that the command is valid.
  if ( 0 == Len() )
  {
    //  we cannot have nullptr commands.
    throw - 1;
  }

  // expand the path
  if( szPath )
  {
    LPTSTR lpExpandPath = nullptr;
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
  }// if we have an szPath != nullptr

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
    _szCommand = action._szCommand; 
    m_szFile   = action.m_szFile;
    m_szExt    = action.m_szExt;
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
  m_szExt = m_szFile = _szCommand = _T("");
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
 * @param CWnd* pWnd the last forground window.
 * @param const MYODD_STRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* Action::CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  not sure how to do that...
  if ( Len() == 0)
  {
    return nullptr;
  }

  // we are about to execute a command, we don't know how long the command will last
  // a badly written plugin could take forever to return.
  BOOL bThen = hook_RejectKeyboad( FALSE );

  //  if we are here then we are going to load a user command
  //
  //  If the user did not pass any arguments/command line then we must get them from the clipboard.
  ActiveAction* aa;
  if( szCommandLine.length() == 0 )
  {
    aa = CreateActiveActionWithNoCommandLine( pWnd, isPrivileged);
  }
  else
  {
    //  so now, at last we can call the command line
    aa = CreateActiveActionDirect( pWnd, szCommandLine.c_str(), isPrivileged);
  }

  // now that we are back from calling the plugin, restore the keyboard state.
  hook_RejectKeyboad( bThen );

  return aa;
}

/**
 * Try and do it when we have no command line
 * @param CWnd* pWnd the last forground window.
 * @param bool isPrivileged if this action is privileged or not.
 * @return bool success or not.
 */
ActiveAction* Action::CreateActiveActionWithNoCommandLine(CWnd* pWnd, bool isPrivileged ) const
{
  //  the command line we will try and make.
  MYODD_STRING szCommandLine = _T("");

  size_t maxClipboardMemory = CActionMonitorApp::GetMaxClipboardMemory();

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
    Clipboard clipboard( cwnd, maxClipboardMemory );

    //  any other values are rejected, (bitmaps and so on).
    MYODD_STRING sText = _T("");
    if (clipboard.GetTextFromClipboard(sText, true ))
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
    myodd::log::LogError(_T("Critical error while trying to run an action, [%s]."), Command() );
    _ASSERT(0);         //  the main reason for failure is probably because  
                        //  there is a format in the Clipboard that I am not handling properly
                        //  there should be a way of sending me a mail when this happens so we can look into fixing it.
  }

  // we can now do it direct.
  return CreateActiveActionDirect( pWnd, szCommandLine.c_str(), isPrivileged);
}

/**
 * Get a text an remove the single lines out of it/
 * TODO: Move this to the am_string namespace?
 * @param const MYODD_STRING& szCommandLine the text that we would like to enforce into a single line.
 * @return MYODD_STRING a single line of text.
 */
MYODD_STRING Action::toSingleLine( LPCTSTR sText ) const
{
  // Sanity checks.
  if(nullptr == sText )
  {
    return _T("");
  }

  LPCTSTR pdest = _tcschr( sText, '\n' );
  size_t  result;

  MYODD_STRING ret( sText );
  if( pdest != nullptr)
  {
    result = pdest - sText;
    ret = ret.substr( 0, result );
    return toSingleLine( ret.c_str() );
  }

  pdest = _tcschr( sText, '\r' );
  if( pdest != nullptr)
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
const MYODD_STRING& Action::Command() const
{
  return _szCommand;
}

/**
 * Execute a file.
 * We will expend all the environment variables as needed.
 * @param const std::vector<MYODD_STRING> [0] the file path, [1] the arguments to launch with, (optional).
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @param HANDLE hProcess if this value is not nullptr, we will return the handle of the started process.
 *               it is then up to the calling application to close this handle when done with it...
 * @return bool true|false success or not.
 */
bool Action::Execute(const std::vector<MYODD_STRING>& argv, bool isPrivileged, HANDLE* hProcess)
{
  // get the number of arguments.
  auto argc = argv.size();

  // sanity check
  if( argc < 1 || argc > 2 )
  {
    ASSERT ( 0 ); //  wrong number of arguments.
    return false;
  }

  LPTSTR argvModule = nullptr;
  LPTSTR argvCmd = nullptr;

  // get the module name, (what we are running).
  // Expand the values that might have been passed.
  if( !myodd::files::ExpandEnvironment( argv[ 0 ].c_str(), argvModule ) )
  {
    myodd::log::LogError(_T("Could not execute statement: Unable to expand command line '%s'"), argv[0].c_str());
    return false;
  }

  // But we might also have a command line item.
  if( 2 == argc )
  {
    // Expand the values that might have been passed.
    if( !myodd::files::ExpandEnvironment( argv[ 1 ].c_str(), argvCmd ) )
    {
      myodd::log::LogError(_T("Could not execute statement: Unable to expand arguments '%s'"), argv[1].c_str());
      delete [] argvModule;
      return false;
    }
  }

  //
  // ShellExecuteEx
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb759784(v=vs.85).aspx 
  //
  SHELLEXECUTEINFO sei = {};
  sei.cbSize = sizeof(sei);     // in, required, sizeof of this structure
  sei.fMask = SEE_MASK_DEFAULT; // in, SEE_MASK_XXX values
  sei.hwnd = nullptr;           // in, optional
  sei.lpFile = argvModule;      // in, either this value or lpIDList must be specified
  sei.lpParameters = argvCmd;   // in, optional
  sei.lpDirectory = nullptr;    // in, optional
  sei.nShow = SW_NORMAL;        // in, required
  //sei.hInstApp;               // out when SEE_MASK_NOCLOSEPROCESS is specified
  //sei.lpIDList;               // in, valid when SEE_MASK_IDLIST is specified, PCIDLIST_ABSOLUTE, for use with SEE_MASK_IDLIST & SEE_MASK_INVOKEIDLIST
  //sei.lpClass;                // in, valid when SEE_MASK_CLASSNAME is specified
  //sei.hkeyClass;              // in, valid when SEE_MASK_CLASSKEY is specified
  //sei.dwHotKey;               // in, valid when SEE_MASK_HOTKEY is specified
  if (isPrivileged == true && !myodd::os::IsElevated() )
  {
    sei.lpVerb = _T("runas"); // in, optional when unspecified the default verb is choosen
  }
  else
  {
    //  launch as a normal file.
    sei.lpVerb = _T("open");  // in, optional when unspecified the default verb is choosen
  }

  // did the user pass a handle?
  // if they did then they want to take ownership of the process and close the handle.
  if (hProcess != nullptr )
  {
    sei.fMask = SEE_MASK_NOCLOSEPROCESS; // in, SEE_MASK_XXX values
  }

  // Assume error
  auto result = false;
  if (!ShellExecuteEx(&sei))
  {
    myodd::log::LogError(_T("Could not execute statement: could not execute '%s'"), argvModule);
    myodd::log::LogError(_T("Could not execute statement: Last error '%d'"), ::GetLastError());
  }
  else
  {
    result = true;
    if (hProcess != nullptr)
    {
      // return the handle.
      *hProcess = sei.hProcess;
    }
  }

  // clean up the expended variable.
  delete [] argvCmd;
  delete [] argvModule;

  // return what we found
  return result;
}

/**
 * Launch a single action with all the command line arguments.
 * TODO : The API calls ignore the values been passed to them, so we should first check that we have all the values.
 * @param CWnd* pWnd the last forground window.
 * @param const MYODD_STRING& szCommandLine the command and the arguments we are launching this file with.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL TRUE|FALSE success or not.
 */
ActiveAction* Action::CreateActiveActionDirect(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  // sanity check
  if (0 == m_szFile.length())
  {
    return nullptr;
  }

  //  get the last forground window handle
  auto hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;

  const auto& szExt = Extension();
#ifdef ACTIONMONITOR_API_LUA
  // Do the API calls.
  //
  if (LuaVirtualMachine::IsExt(szExt.c_str()))
  {
    auto ala = new ActiveLuaAction(*this, hTopHWnd, szCommandLine, isPrivileged);
    if (ala->Initialize())
    {
      return ala;
    }

    // did not work, try the default way...
    delete ala;
  }
#endif // ACTIONMONITOR_API_LUA

#ifdef ACTIONMONITOR_API_PY
  // Do the API calls.
  //
  if (PythonVirtualMachine::IsExt(szExt.c_str()))
  {
    auto apa = new ActivePythonAction(*this, hTopHWnd, szCommandLine, isPrivileged);
    if(apa->Initialize() )
    { 
      return apa;
    }

    // did not work, try the default way...
    delete apa;
  }
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_PS_PLUGIN
  // Do the API calls.
  //
  if (PowershellVirtualMachine::IsExt(szExt.c_str()))
  {
    auto apa = new ActivePowershellAction(*this, hTopHWnd, szCommandLine, isPrivileged);
    if (apa->Initialize())
    {
      return apa;
    }

    // did not work, try the default way...
    delete apa;
  }
#endif // ACTIONMONITOR_API_PLUGIN

#ifdef ACTIONMONITOR_API_PLUGIN
  // Do the API calls.
  //
  if (PluginVirtualMachine::IsExt(szExt.c_str()))
  {
    auto apa = new ActivePluginAction(*this, hTopHWnd, szCommandLine, isPrivileged);
    if (apa->Initialize())
    {
      return apa;
    }

    // did not work, try the default way...
    delete apa;
  }
#endif // ACTIONMONITOR_API_PLUGIN

  // Batch files...
  if( _tcsicmp(szExt.c_str(), _T("bat")) == 0)
  {
    return new ActiveBatchAction(*this, hTopHWnd, szCommandLine );
  }

  // run the default action.
  return new ActiveDefaultAction( *this, hTopHWnd, szCommandLine, isPrivileged );
}