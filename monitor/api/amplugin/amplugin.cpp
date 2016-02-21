// amplugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "amplugin.h"

#ifdef _DEBUG
# define new DEBUG_NEW
#endif

typedef bool (*PLUGIN_FNCSAY)(LPCWSTR, UINT, UINT );
typedef double (*PLUGIN_FNCVERSION)( );
typedef int (*PLUGIN_FNCGETS)( UINT, DWORD, LPTSTR );
typedef int (*PLUGIN_FNCGET)( DWORD, LPTSTR );
typedef int (*PLUGIN_FNCGETCOUNT)(  );
typedef bool (*PLUGIN_FNCEXECUTE)( LPCWSTR, LPCWSTR );
typedef bool (*PLUGIN_FNCOMMAND)( LPCWSTR, LPCWSTR );
typedef bool (*PLUGIN_FINDCOMMAND)( UINT, LPCWSTR, DWORD, LPWSTR );

/**
 * Todo
 * @param void
 * @return void
 */
amplugin::amplugin()
{
}

/**
 * Todo
 * @param void
 * @return void
 */
amplugin::~amplugin()
{
}

/**
 * Add a 
 * @param void
 * @return void
 */
void amplugin::Add( LPCWSTR name, void* pFunc )
{
  m_pFunction[ name ] = pFunc;
}

/**
 * Todo
 * @param void
 * @return void
 */
void* amplugin::get( LPCWSTR name )
{
  FNC_CONTAINER::const_iterator iter = m_pFunction.find( name );
  if( iter == m_pFunction.end() )
  {
    return NULL;
  }
  return iter->second;
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
bool amplugin::say( LPCWSTR msg, UINT nElapse, UINT nFadeOut)
{
  void* pFunc = get( _T("say") );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCSAY)(pFunc))( msg, nElapse, nFadeOut );
}

/**
 * Todo
 * @param void
 * @return void
 */
double amplugin::version ()
{
  void* pFunc = get( _T("version") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCVERSION)(pFunc))();
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
int amplugin::getCommand( UINT idx, DWORD nBufferLength, LPTSTR lpBuffer )
{
  void* pFunc = get( _T("getCommand") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGETS)(pFunc))( idx, nBufferLength, lpBuffer );
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
int amplugin::getFile( UINT idx, DWORD nBufferLength, LPTSTR lpBuffer)
{
  void* pFunc = get( _T("getFile") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGETS)(pFunc))( idx, nBufferLength, lpBuffer );
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
int amplugin::getFolder( UINT idx, DWORD nBufferLength, LPTSTR lpBuffer)
{
  void* pFunc = get( _T("getFolder") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGETS)(pFunc))( idx, nBufferLength, lpBuffer );
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
int amplugin::getURL( UINT idx, DWORD nBufferLength, LPTSTR lpBuffer)
{
  void* pFunc = get( _T("getURL") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGETS)(pFunc))( idx, nBufferLength, lpBuffer );
}

/**
 * Todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
int amplugin::getString(DWORD nBufferLength, LPTSTR lpBuffer)
{
  void* pFunc = get( _T("getString") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGET)(pFunc))( nBufferLength, lpBuffer);
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
int amplugin::getAction(DWORD nBufferLength, LPTSTR lpBuffer)
{
  void* pFunc = get( _T("getAction") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGET)(pFunc))( nBufferLength, lpBuffer);
}

/**
 * Todo
 * @param void
 * @return void
 */
int amplugin::getCommandCount()
{
  void* pFunc = get( _T("getCommandCount") );
  if( !pFunc )
  {
    return 0;
  }
  return ((PLUGIN_FNCGETCOUNT)(pFunc))( );
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
bool amplugin::execute( LPCWSTR module, LPCWSTR cmdLine)
{
  void* pFunc = get( _T("execute") );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCEXECUTE)(pFunc))( module, cmdLine );
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
bool amplugin::addAction( LPCWSTR szText, LPCWSTR szPath )
{
  void* pFunc = get( _T("addAction") );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCOMMAND)(pFunc))( szText, szPath );
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
bool amplugin::removeAction( LPCWSTR szText, LPCWSTR szPath )
{
  void* pFunc = get( _T("removeAction") );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCOMMAND)(pFunc))( szText, szPath );
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param UINT the index of the action we are looking for.
 * @param LPCTSTR the name of the command we want to find
 * @param DWORD the max size of the buffer.
 * @param LPWSTR& if the action exists, return the path for it.
 * @return bool if the action exits or not.
 */
bool amplugin::findAction( UINT idx, LPCWSTR szText, DWORD nBufferLength, LPWSTR lpBuffer)
{
  void* pFunc = get( _T("findAction") );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FINDCOMMAND)(pFunc))( idx, szText, nBufferLength, lpBuffer );
}