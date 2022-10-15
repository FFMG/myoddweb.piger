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
#include "ActionsImmediate.h"
#include "Application.h"

BEGIN_MESSAGE_MAP(CActionMonitorApp, CWinApp)
END_MESSAGE_MAP()

/**
 * \brief The constructor.
 */
CActionMonitorApp::CActionMonitorApp() :
  _mutex(nullptr),
  _maxClipboardSize(NULL), 
_application(nullptr)
{
}

/**
 *  \brief destruction
 */
CActionMonitorApp::~CActionMonitorApp()
{
}

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
 * Get the max clipboard size.
 */
size_t CActionMonitorApp::GetMaxClipboardMemory()
{
  return App()._maxClipboardSize;
}

/**
 * Check if we can start the app or not.
 * if the app is already running then we will not be able to get that mutex.
 * @return bool if we can/cannot start the app.
 */
bool CActionMonitorApp::CanStartApp()
{
  _mutex = CreateMutex(nullptr,   // default security attributes
                          FALSE,  // initially not owned
                          CONF_MUTEXT 
                         );

  if (_mutex == nullptr)
  {
    TRACE("CreateMutex error: %d\n", GetLastError());
    return false;
  }

  //
  const auto lErr = GetLastError();
  switch (lErr)
  {
  case ERROR_ALREADY_EXISTS:
    CloseHandle(_mutex);
    _mutex = nullptr;
    return false;

  case ERROR_SUCCESS:
  default:
    break; 
  }
 
  //  if we are here we can start.
  return true;
}

/**
 * \brief If needed we restart ourselves and run as administrator.
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
  if (!CanStartApp())
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
  // ("c", myodd::po::wvalue<std::wstring>(), "the path of the config file.")
  // ("d", myodd::po::wvalue<std::wstring>(), "the full path of the root commands.");

  myodd::commandline_variables vm(__argc, __wargv);

  // We need to init the registry so that everybody can use it.
  if (!InitConfig(vm))
  {
    // we could not load the config, this is critical, we can go no further.
    return FALSE;
  }

  // if the user passed something then it will override what we have
  if (vm.contains(L"d"))
  {
    // get the path
    const auto lpPath = vm.get( L"d");

    //  set it in case next time we have nothing
    myodd::config::Set(L"paths\\commands", lpPath);
  }

  // setup the log
  InitLog();

  // we now need to add the default reserved paths.
  InitReservedPaths();

  // set the cliboard size.
  InitMaxClipboardSize();

  CreateAndShowActionDialog();

  // always return false
  return FALSE;
}

bool CActionMonitorApp::CreateAndShowActionDialog()
{
  delete _application;
  _application = new Application();

  _application->Show();
  delete _application;
  _application = nullptr;
  return true;
}

/**
 * \brief Set the max clipboard size that we don't want to use more of.
 *        depending on the windows version/memory available.
 */
void CActionMonitorApp::InitMaxClipboardSize()
{
  //  reset the value.
  _maxClipboardSize = 0;

  // the path
  const auto path = L"clipboard\\maxmemory";

  //  does the value exist?
  if (!myodd::config::Contains(path))
  {
    //  yes, it does, simply use the value and return.
    _maxClipboardSize = 0;
    return;
  }

  // do we have a valid value in the config?
  const auto maxClipboardSize = static_cast<size_t>(::myodd::config::Get( path, 1024 ));
  _maxClipboardSize = maxClipboardSize;
}

/**
 * \brief Initialize the path of the reserved paths.
 *        Make sure that the values exist and are set properly.
 */
void CActionMonitorApp::InitReservedPaths()
{
  const std::wstring sPath = ::myodd::config::Get( L"paths\\commands", L"");

  auto sPathIn = myodd::files::Join( sPath, AM_DIRECTORY_IN );
  myodd::files::CreateFullDirectory( sPathIn, false );
  myodd::config::Set( L"paths\\in", sPathIn );

  auto sPathOut = myodd::files::Join( sPath, AM_DIRECTORY_OUT );
  myodd::files::CreateFullDirectory( sPathOut, false );
  myodd::config::Set( L"paths\\out", sPathOut );

  auto sPathTmp = myodd::files::Join( sPath, AM_DIRECTORY_TMP );
  myodd::files::CreateFullDirectory( sPathTmp, false );
  myodd::config::Set( L"paths\\tmp", sPathTmp );

  auto sPathPlugin = myodd::files::Join( sPath, AM_DIRECTORY_PLUGIN );
  myodd::files::CreateFullDirectory( sPathPlugin, false );
  myodd::config::Set( L"paths\\plugin", sPathPlugin );
}

/**
 * \brief Initialize the path of the configurations file.
 * \param variables the given variables.
 * \return Success or not.
 */
bool CActionMonitorApp::InitConfig( const myodd::commandline_variables& variables )
{
  std::wstring sAPath = CONF_FULLPATH;

  if (variables.contains(L"c"))
  {
    // get the path
    sAPath = variables.get(L"c" );
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
  if (_mutex)
  {
    // releaste the mutex
    ReleaseMutex(_mutex);

    //  close the mutext
    CloseHandle(_mutex);
  }
  _mutex = nullptr;

  return CWinApp::ExitInstance();
}
