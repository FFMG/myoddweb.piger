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
#include "stdafx.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"
#include "ActionsImmediate.h"

#include "ActionBye.h"
#include "ActionLoad.h"
#include "ActionVersion.h"

BEGIN_MESSAGE_MAP(CActionMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CActionMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/**
 *The constructor.
 */
CActionMonitorApp::CActionMonitorApp() :
  m_hMutex(nullptr),
  _cwndLastForegroundWindow(nullptr),
  _maxClipboardSize( NULL )
#ifdef ACTIONMONITOR_API_LUA
  , _possibleActions(nullptr), 
  _lvm(nullptr)
#endif
#ifdef ACTIONMONITOR_API_PY
  , _pvm(nullptr)
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  , _plugvm(nullptr)
#endif
#ifdef ACTIONMONITOR_PS_PLUGIN
  , _psvm(nullptr)
#endif
#ifdef ACTIONMONITOR_S_PLUGIN
  , _svm(nullptr)
#endif
#ifdef ACTIONMONITOR_CS_PLUGIN
  , _csvm(nullptr)
#endif
{
}

/**
 *  \brief destruction
 */
CActionMonitorApp::~CActionMonitorApp()
{
#ifdef ACTIONMONITOR_API_LUA
  delete _lvm;
  _lvm = nullptr;
#endif
#ifdef ACTIONMONITOR_API_PY
  delete _pvm;
  _pvm = nullptr;
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  delete _plugvm;
  _plugvm = nullptr;
#endif
#ifdef ACTIONMONITOR_PS_PLUGIN
  delete _psvm;
  _psvm = nullptr;
#endif
#ifdef ACTIONMONITOR_S_PLUGIN
  delete _svm;
  _svm = nullptr;
#endif
#ifdef ACTIONMONITOR_CS_PLUGIN
  delete _csvm;
  _csvm = nullptr;
#endif
}

#ifdef ACTIONMONITOR_API_LUA
LuaVirtualMachine* CActionMonitorApp::GetLuaVirtualMachine()
{
  if (_lvm == nullptr)
  {
    _lvm = new LuaVirtualMachine();
  }
  return _lvm;
}
#endif

#ifdef ACTIONMONITOR_API_PY
PythonVirtualMachine* CActionMonitorApp::GetPythonVirtualMachine()
{
  if (_pvm == nullptr)
  {
    _pvm = new PythonVirtualMachine();
  }
  return _pvm;
}
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
PluginVirtualMachine* CActionMonitorApp::GetPluginVirtualMachine()
{
  if (_plugvm == nullptr)
  {
    _plugvm = new PluginVirtualMachine();
  }
  return _plugvm;
}
#endif

#ifdef ACTIONMONITOR_PS_PLUGIN
PowershellVirtualMachine* CActionMonitorApp::GetPowershellVirtualMachine()
{
  if (_psvm == nullptr)
  {
    _psvm = new PowershellVirtualMachine();
  }
  return _psvm;
}
#endif

#ifdef ACTIONMONITOR_S_PLUGIN
ShellVirtualMachine* CActionMonitorApp::GetShellVirtualMachine()
{
  if (_svm == nullptr)
  {
    _svm = new ShellVirtualMachine();
  }
  return _svm;
}
#endif

#ifdef ACTIONMONITOR_CS_PLUGIN
CsVirtualMachine* CActionMonitorApp::GetCsVirtualMachine()
{
  if (_csvm == nullptr)
  {
    _csvm = new CsVirtualMachine();
  }
  return _csvm;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// The one and only CActionMonitorApp object
CActionMonitorApp theApp;

/**
 * \brief Get the current CActionMonitorApp reference
 * \return the CActionMonitorApp reference.
 */
CActionMonitorApp& App()
{ 
  return theApp; 
};

/**
 * \brief get the last foregound window.
 * \return void
 */
CWnd* CActionMonitorApp::GetLastForegroundWindow()
{
  return App()._cwndLastForegroundWindow;
}

/**
 * Get the max clipboard size.
 */
size_t CActionMonitorApp::GetMaxClipboardMemory()
{
  return App()._maxClipboardSize;
}

/**
 * Set the value of the last window.
 * \param window the new 'last' window.
 * \return void
 */
void CActionMonitorApp::SetLastForegroundWindow( CWnd* window )
{
  App()._cwndLastForegroundWindow = window;
}

/**
 * Check if we can start the app or not.
 * if the app is already running then we will not be able to get that mutex.
 * @return bool if we can/cannot start the app.
 */
bool CActionMonitorApp::CanStartApp()
{
  m_hMutex = CreateMutex(nullptr,   // default security attributes
                          FALSE,  // initially not owned
                          CONF_MUTEXT 
                         );

  if (m_hMutex == nullptr)
  {
    TRACE("CreateMutex error: %d\n", GetLastError());
    return false;
  }

  //
  DWORD lErr = GetLastError();
  switch (lErr)
  {
  case ERROR_ALREADY_EXISTS:
    CloseHandle(m_hMutex);
    m_hMutex = nullptr;
    return false;

  case ERROR_SUCCESS:
  default:
    break; 
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
#else
  if (myodd::os::IsElevated())
  {
    //  already elevated.
    return;
  }

  TCHAR szPath[MAX_PATH];
  if (0 == GetModuleFileName(nullptr, szPath, ARRAYSIZE(szPath)))
  {
    //  could not get our own path!
    return;
  }

  // Launch itself as admin
  SHELLEXECUTEINFO sei = { sizeof(sei) };
  sei.lpVerb = _T("runas");
  sei.lpFile = szPath;
  sei.hwnd = nullptr;
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
#endif
}

/**
 * \brief initialise 
 * \return success or not
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
    ("c", myodd::po::wvalue<std::wstring>(), "the path of the config file.")
    ("d", myodd::po::wvalue<std::wstring>(), "the full path of the root commands.")
#else
    ("c", myodd::po::wvalue<std::string>(), "the oath of the config file.")
    ("d", myodd::po::value<std::string>(), "the full path of the root commands.")
#endif
    ;

#ifdef UNICODE
  TCHAR** args = __wargv;
#else
  TCHAR** args = __argv;
#endif
  myodd::variables vm( __argc, args, desc );

  // We need to init the registry so that everybody can use it.
  if (!InitConfig(vm))
  {
    // we could not load the config, this is critical, we can go no further.
    return FALSE;
  }

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
    myodd::config::Set( L"paths\\commands", lpPath );
  }

  // setup the log
  InitLog();
  
  // we now need to add the default reserved paths.
  InitReservedPaths();

  // set the cliboard size.
  InitMaxClipboardSize();

  auto noTaskBar = new CFrameWnd();
  noTaskBar->Create(nullptr, nullptr,WS_OVERLAPPEDWINDOW);

	CActionMonitorDlg dlg( noTaskBar );
	m_pMainWnd = &dlg;

  // create the possible actions
  BuildActionsList( );

  auto nResponse = dlg.DoModal();
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
 * \brief (Re)build a list of possible actions.
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
  _possibleActions->Add( new ActionBye() );
  _possibleActions->Add( new ActionLoad() );
  _possibleActions->Add( new ActionVersion() );
}

/**
 * \brief Execute all the actions that are exectuted at the start of this app.
 *        Note that the Dialog box is alredy created so we can show all the messages, (if any).
 * \return void
 */
void CActionMonitorApp::DoStartActionsList()
{
  ActionsImmediate aI( AM_DIRECTORY_IN );
  aI.Init();
}

/**
 * \brief Exectue all the actions that are executed at just before we end the app.
 *        Note that the dialog should still be alive so we can display the messages.
 */
void CActionMonitorApp::DoEndActionsList( )
{
  ActionsImmediate aI( AM_DIRECTORY_OUT );
  aI.Init();
}

/**
 * Set the max cliboard size that we don't want to use more of.
 * depending on the windows version/memory available.
 */
void CActionMonitorApp::InitMaxClipboardSize()
{
  //  reset the value.
  _maxClipboardSize = 0;

  // the path
  const MYODD_STRING path = _T("clipboard\\maxmemory");

  //  does the value exist?
  if (!myodd::config::Contains(path))
  {
    //  yes, it does, simply use the value and return.
    _maxClipboardSize = 0;
    return;
  }

  // do we have a valid value in the config?
  auto maxClipboardSize = static_cast<size_t>(::myodd::config::Get( path, 1024 ));
  _maxClipboardSize = maxClipboardSize;
}

/**
 * \brief Initialize the path of the reserved paths.
 *        Make sure that the values exist and are set properly.
 */
void CActionMonitorApp::InitReservedPaths()
{
  const auto sPath = ::myodd::config::Get( L"paths\\commands", L"");

  MYODD_STRING sPathIn;
  myodd::files::Join( sPathIn, sPath, AM_DIRECTORY_IN );
  myodd::files::CreateFullDirectory( sPathIn, false );
  myodd::config::Set( L"paths\\in", sPathIn );

  MYODD_STRING sPathOut;
  myodd::files::Join( sPathOut, sPath, AM_DIRECTORY_OUT );
  myodd::files::CreateFullDirectory( sPathOut, false );
  myodd::config::Set( L"paths\\out", sPathOut );

  MYODD_STRING sPathTmp;
  myodd::files::Join( sPathTmp, sPath, AM_DIRECTORY_TMP );
  myodd::files::CreateFullDirectory( sPathTmp, false );
  myodd::config::Set( L"paths\\tmp", sPathTmp );

  MYODD_STRING sPathPlugin;
  myodd::files::Join( sPathPlugin, sPath, AM_DIRECTORY_PLUGIN );
  myodd::files::CreateFullDirectory( sPathPlugin, false );
  myodd::config::Set( L"paths\\plugin", sPathPlugin );
}

/**
 * \brief Initialize the path of the configurations file.
 * \param variables the given variables.
 * \return Success or not.
 */
bool CActionMonitorApp::InitConfig( const myodd::variables& variables )
{
#ifdef UNICODE
  std::wstring sAPath = CONF_FULLPATH;
#else
  std::string sAPath = CONF_FULLPATH;
#endif

  if (1 == variables.count("c"))
  {
    // get the path
#ifdef UNICODE
    sAPath = variables["c"].as< std::wstring >();
#else
    sAPath = vm["c"].as< std::string >();
#endif
  }

  // does the file exist?
  if (!myodd::files::FileExists(sAPath))
  {
    MessageBox(nullptr, _T("I was unable to locate the configuration file, this is a critical problem!"), _T("Cannot find config"), MB_ICONERROR | MB_OK);
    return false;
  }

  // remember that calling ::init(...) will cause the current values to be saved.
  // so witting to the xml while the app is running will have no effect.
  if (!myodd::config::FromXMLFile(sAPath))
  {
    MessageBox(nullptr, _T("There was a problem with the given config, is it valid XML?"), _T("Cannot load config"), MB_OK);
    return false;
  }
  return true;
}

/**
 * Initialise the log path and values.
 */
void CActionMonitorApp::InitLog()
{
  // check if enabled
  if (::myodd::config::Get(_T("log\\file\\enable"), 1 ) != 1)
  {
    return;
  }

  //  the directory we will be logging to.
  const std::wstring logPath = ::myodd::config::Get(L"log\\file\\path", LOG_PATH );

  // the prefix of the filename.
  const std::wstring logPrefix = ::myodd::config::Get(L"log\\file\\prefix", L"myodd");

  // the file extension
  const std::wstring logExtension = ::myodd::config::Get(L"log\\file\\extension", L"log" );

  //  get the max file size.
  size_t maxFileSize = ::myodd::config::Get(L"log\\file\\maxFileSize", 10 );

  //  sanity check
  if (maxFileSize > 10 || maxFileSize < 1)
  {
    myodd::log::LogError(_T("The given max log size, (%d), is invalid"), maxFileSize);
    maxFileSize = 10;
  }

  // we can now try and initialise the log.
  myodd::log::Initialise(logPath, logPrefix, logExtension, maxFileSize );
}

/**
 * \brief close the application
 * \return the result.
 */
int CActionMonitorApp::ExitInstance()
{
  //  free/save the configuration file
  myodd::config::Close();

  //  this should never not be null.
  if (m_hMutex)
  {
    // releaste the mutex
    ReleaseMutex(m_hMutex);

    //  close the mutext
    CloseHandle(m_hMutex);
  }
  m_hMutex = nullptr;

  return CWinApp::ExitInstance();
}

/**
 * Destroy the active actions.
 * @return non.
 */
void CActionMonitorApp::DestroyActiveActions()
{
#ifdef ACTIONMONITOR_API_PLUGIN
  // We have to kill all the API plugins.
  // they should be all done and completed.
  auto pg = GetPluginVirtualMachine();
  pg->DestroyPlugins();
#endif // ACTIONMONITOR_API_PLUGIN

#ifdef ACTIONMONITOR_PS_PLUGIN
  // We have to kill all the API plugins.
  // they should be all done and completed.
  auto ps = GetPowershellVirtualMachine();
  ps->DestroyScripts();
#endif // ACTIONMONITOR_PS_PLUGIN

#ifdef ACTIONMONITOR_S_PLUGIN
  // We have to kill all the API plugins.
  // they should be all done and completed.
  auto s = GetShellVirtualMachine();
  s->DestroyScripts();
#endif // ACTIONMONITOR_S_PLUGIN

#ifdef ACTIONMONITOR_CS_PLUGIN
  // We have to kill all the API plugins.
  // they should be all done and completed.
  auto cs = GetCsVirtualMachine();
  cs->DestroyScripts();
#endif // ACTIONMONITOR_CS_PLUGIN
}