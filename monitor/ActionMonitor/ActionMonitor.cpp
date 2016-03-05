#include "stdafx.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"
#include "ActionsImmediate.h"
#include "os/os.h"

BEGIN_MESSAGE_MAP(CActionMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CActionMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/**
 * todo
 * @param void
 * @return void
 */
CActionMonitorApp::CActionMonitorApp() :
  m_hMutex( NULL )
#ifdef ACTIONMONITOR_API_LUA
  , _lvm(NULL)
#endif
#ifdef ACTIONMONITOR_API_PY
  , _pvm(NULL)
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  , _plugvm(NULL)
#endif
{
}

/**
 * todo
 * @param void
 * @return void
 */
CActionMonitorApp::~CActionMonitorApp()
{
#ifdef ACTIONMONITOR_API_LUA
  delete _lvm;
  _lvm = NULL;
#endif
#ifdef ACTIONMONITOR_API_PY
  delete _pvm;
  _pvm = NULL;
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  delete _plugvm;
  _plugvm = NULL;
#endif
}

#ifdef ACTIONMONITOR_API_LUA
LuaVirtualMachine* CActionMonitorApp::GetLuaVirtualMachine()
{
  if (_lvm == NULL)
  {
    _lvm = new LuaVirtualMachine();
  }
  return _lvm;
}
#endif

#ifdef ACTIONMONITOR_API_PY
PythonVirtualMachine* CActionMonitorApp::GetPythonVirtualMachine()
{
  if (_pvm == NULL)
  {
    _pvm = new PythonVirtualMachine();
  }
  return _pvm;
}
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
PluginVirtualMachine* CActionMonitorApp::GetPluginVirtualMachine()
{
  if (_plugvm == NULL)
  {
    _plugvm = new PluginVirtualMachine();
  }
  return _plugvm;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// The one and only CActionMonitorApp object
CActionMonitorApp theApp;

/**
 * Get the current CActionMonitorApp reference
 * @param nonce
 * @return the CActionMonitorApp reference.
 */
CActionMonitorApp& App()
{ 
  return theApp; 
};

/**
 * @param CWnd* The window that last had the focus when we pressed the special key.
 */
static CWnd* cwndLastForegroundWindow = NULL;

/**
 * todo
 * @param void
 * @return void
 */
CWnd* CActionMonitorApp::GetLastForegroundWindow()
{
  return cwndLastForegroundWindow;
}

/**
 * todo
 * @param void
 * @return void
 */
void CActionMonitorApp::SetLastForegroundWindow( CWnd* w )
{
  cwndLastForegroundWindow = w;
}

bool CActionMonitorApp::CanStartApp()
{
  m_hMutex = CreateMutex( NULL,   // default security attributes
                          FALSE,  // initially not owned
                          CONF_MUTEXT 
                         );

  if (m_hMutex == NULL) 
  {
    TRACE("CreateMutex error: %d\n", GetLastError());
    return false;
  }

  //  if we are here we can start.
  return true;
}

/**
 * If needed we restart ourselves and run as administrator.
 * @return none
 */
void CActionMonitorApp::SelfElavate()
{
#ifdef _DEBUG
  // if we are in debug we cannot elevate ourselve.
  // otherwise we will never be able to debug...
  return;
#endif

  if (myodd::os::IsElevated())
  {
    //  already elevated.
    return;
  }

  TCHAR szPath[MAX_PATH];
  if (0 == GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
  {
    //  could not get our own path!
    return;
  }

  // Launch itself as admin
  SHELLEXECUTEINFO sei = { sizeof(sei) };
  sei.lpVerb = _T("runas");
  sei.lpFile = szPath;
  sei.hwnd = NULL;
  sei.nShow = SW_NORMAL;
  if (!ShellExecuteEx(&sei))
  {
    DWORD dwError = GetLastError();
    if (dwError == ERROR_CANCELLED)
    {
      // The user refused to allow privileges elevation.
      // continue unelevated.
    }
  }
  else
  {
    _exit(1);  // Quit itself
  }
}

/**
 * todo
 * @param void
 * @return void
 */
BOOL CActionMonitorApp::InitInstance()
{
  // elevate ourselves to admin if need be.
  SelfElavate();

  // can we start running now?
  // if not then there is a problem.
  if( !CanStartApp( ) )
  {
    return FALSE;
  }

  //
  // Run some of the template tests
  // If any of them fires we changed something in 
  // the template that is no longer as expected.
  //
  myodd::strings::Test();
  myodd::geo::Test();
  myodd::math::Test();
  myodd::files::Test();
  myodd::idx_map<int,int>::Test();
  
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

  //  load the command line
  //  --d xxxxxxx    : path of all the root comands
  //  /opt xxxxxxx  : path of the option(s) files
  myodd::desc desc;
  desc.add_options()
#ifdef UNICODE
    ("d", myodd::po::wvalue<std::wstring>(), "the full path of the root commands.")
    ("opt", myodd::po::wvalue<std::wstring>(), "the path of the options file.")
#else
    ("d", myodd::po::value<std::string>(), "the full path of the root commands.")
    ("opt", myodd::po::value<std::string>(), "the path of the options file.")
#endif
    ;

#ifdef UNICODE
  TCHAR** args = __wargv;
#else
  TCHAR** args = __argv;
#endif
  myodd::variables vm( __argc, args, desc );

  // We need to init the registry so that everybody can use it.
  InitConfig();

  // if the user passed something then it will override what we have
  if( 1 == vm.count( "d") )
  {
    // get the path
#ifdef UNICODE
    std::wstring sAPath = vm["d"].as< std::wstring >();
#else
    std::string sAPath = vm["d"].as< std::string >();
#endif
    LPCTSTR lpPath = sAPath.c_str();

    //  set it in case next time we have nothing
    myodd::config::set( _T("paths\\commands"), lpPath );
  }

  // we now need to add the default reserved paths.
  InitReservedPaths();

  CFrameWnd *noTaskBar=new CFrameWnd();
  noTaskBar->Create(0,0,WS_OVERLAPPEDWINDOW);
	CActionMonitorDlg dlg( noTaskBar );
	m_pMainWnd = &dlg;

  // create the possible actions
  BuildActionsList( );
  
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

  //  clean up the window.
  noTaskBar->DestroyWindow();

  // remove the actions
  // we are about to close, we are no longer monitoring anything
  delete _possibleActions;

	//  Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pupszCmdLine.
	return FALSE;
}

/**
 * (Re)build a list of possible actions.
 * @param void
 * @return void
 */
void CActionMonitorApp::BuildActionsList()
{
  //  remove the old one
  delete _possibleActions;

  //  create a new one.
  _possibleActions = new Actions( );

  //  parse the directory for all possible files.
  _possibleActions->Init();

  //TODO these really need to move out of here
  //  add the default commands
  _possibleActions->Add( ACTION_CORE_BYE, NULL );
  _possibleActions->Add( ACTION_CORE_LOAD, NULL);
  _possibleActions->Add( ACTION_CORE_VERSION, NULL);
}

/**
 * Execute all the actions that are exectuted at the start of this app.
 * Note that the Dialog box is alredy created so we can show all the messages, (if any).
 * @param void
 * @return void
 */
void CActionMonitorApp::DoStartActionsList()
{
  ActionsImmediate aI( AM_DIRECTORY_IN );
  aI.Init();
}

/**
 * Exectue all the actions that are executed at just before we end the app.
 * Note that the dialog should still be alive so we can display the messages.
 * @param void
 * @return void
 */
void CActionMonitorApp::DoEndActionsList()
{
  ActionsImmediate aI( AM_DIRECTORY_OUT );
  aI.Init();
}

/**
 * Initialize the path of the reserved paths.
 * Make sure that the values exist and are set properly.
 * @param void
 * @return void
 */
void CActionMonitorApp::InitReservedPaths()
{
  STD_TSTRING sPath = myodd::config::get( _T("paths\\commands") );

  STD_TSTRING sPathIn;
  myodd::files::Join( sPathIn, sPath, AM_DIRECTORY_IN );
  myodd::files::CreateFullDirectory( sPathIn, false );
  myodd::config::set( _T("paths\\in"), sPathIn );

  STD_TSTRING sPathOut;
  myodd::files::Join( sPathOut, sPath, AM_DIRECTORY_OUT );
  myodd::files::CreateFullDirectory( sPathOut, false );
  myodd::config::set( _T("paths\\out"), sPathOut );

  STD_TSTRING sPathTmp;
  myodd::files::Join( sPathTmp, sPath, AM_DIRECTORY_TMP );
  myodd::files::CreateFullDirectory( sPathTmp, false );
  myodd::config::set( _T("paths\\tmp"), sPathTmp );

  STD_TSTRING sPathPlugin;
  myodd::files::Join( sPathPlugin, sPath, AM_DIRECTORY_PLUGIN );
  myodd::files::CreateFullDirectory( sPathPlugin, false );
  myodd::config::set( _T("paths\\plugin"), sPathPlugin );
}

/**
 * Initialize the path of the configurations file.
 * @param void
 * @return void
 */
void CActionMonitorApp::InitConfig()
{
  // remember that calling ::init(...) will cause the current values to be saved.
  // so witting to the xml while the app is running will have no effect.
  myodd::config::init( CONF_FULLPATH );
}

/**
 * Todo
 * @param void
 * @return int
 */
int CActionMonitorApp::ExitInstance()
{
  //  free/save the configuration file
  myodd::config::free();

  //  close the mutext
  CloseHandle(m_hMutex);
  m_hMutex = NULL;

  return CWinApp::ExitInstance();
}
