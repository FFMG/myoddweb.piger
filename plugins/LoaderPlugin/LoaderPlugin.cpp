// LoaderPlugin.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "LoaderPlugin.h"

// ---------------------------------------------------------------------------
// The one and only CLoaderPluginApp object
CLoaderPluginApp theApp;

PLUGIN_API
AM_RESPONSE am_Msg(AM_MSG msg, AM_UINT wParam, AM_INT lParam)
{
  switch( msg )
  {
  case AM_MSG_INIT:
    {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      theApp.InitPlugin( (amplugin*)(lParam) );
      return AM_RESP_TRUE;
    }
    break;

  case AM_MSG_DEINIT:
    {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      return (theApp.DestroyPlugIn() ? AM_RESP_TRUE : AM_RESP_FALSE);
    }
    break;

  case AM_MSG_NAME:
    {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());

      // if the user passed NULL then all they want is the len of the string
      LPCWSTR lpName = L"Loader Plugin";
      unsigned long nBufferLength = (unsigned long)wParam;
      const wchar_t* lpBuffer = (const wchar_t*)lParam;
      if( lpBuffer && nBufferLength > 0)
      {
        _tcsncpy_s( lpBuffer, nBufferLength, lpName, _TRUNCATE );
        return AM_RESP_TRUE;
      }
      return AM_RESP_BUFFER_SIZE;
    }
    break;

  case AM_MSG_MAIN:
    {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      return (theApp.Main( (amplugin*)(lParam) ) ? AM_RESP_TRUE : AM_RESP_FALSE);
    }
    break;

  case AM_MSG_PATH_CMD:
  case AM_MSG_PATH_IN:
  case AM_MSG_PATH_OUT:
  case AM_MSG_PATH_TMP:
    return AM_RESP_NOT_SUPPORTED;
    break;

  case AM_MSG_PATH_PLUGIN:
    {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      theApp.SetPluginPath( (const wchar_t*)lParam );
      return AM_RESP_TRUE;
    }
    break;

  default:
    break;
  }

  // in case new messages are introduce we don't assert
  // we simply don't know how to handle this particular message.
  // it probably was not intended for us.
  return AM_RESP_NOT_SUPPORTED;
}

// ---------------------------------------------------------------------------
// CLoaderPluginApp
BEGIN_MESSAGE_MAP(CLoaderPluginApp, CWinApp)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------
// CLoaderPluginApp construction
CLoaderPluginApp::CLoaderPluginApp() : 
  m_loadManager( NULL )
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// ---------------------------------------------------------------------------
CLoaderPluginApp::~CLoaderPluginApp()
{
  delete m_loadManager;
  m_loadManager = NULL;
}

// ---------------------------------------------------------------------------
// CLoaderPluginApp initialization
BOOL CLoaderPluginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/**
 * Set the user root command plugins path, (__plugins).
 * We will use this path to save the new commands.
 * @param LPCWSTR the temp path.
 * @return none.
 */
void CLoaderPluginApp::SetPluginPath( LPCWSTR lpPath )
{
  myodd::files::Join( m_pluginPath, lpPath, L"loader" );
}

// ---------------------------------------------------------------------------
void CLoaderPluginApp::InitPlugin( amplugin* p )
{
  delete m_loadManager;
  m_loadManager = new LoaderManager( m_pluginPath );
  m_loadManager->Init( p );
}

// ---------------------------------------------------------------------------
bool CLoaderPluginApp::DestroyPlugIn( )
{
  ASSERT( m_loadManager ); // was Init Never called?
  if( NULL == m_loadManager )
  {
    //  was the assert not called?
    return false;
  }

  m_loadManager->Exit();
  delete m_loadManager;
  m_loadManager = NULL;
  return true;
}

// ---------------------------------------------------------------------------
bool CLoaderPluginApp::Main( amplugin* p )
{
  if( NULL == m_loadManager )
  {
    ASSERT( 0 );  //  did we never initialize?
    return false;
  }
  
  // run the actual code that was just passed.
  m_loadManager->Main( p );
  return true;
}