// AmPlugin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AmPlugin.h"

typedef bool (*PLUGIN_FNCSAY)(const wchar_t*, unsigned int, unsigned int);
typedef double (*PLUGIN_FNCVERSION)( );
typedef size_t (*PLUGIN_FNCGETS)(unsigned int, unsigned int, wchar_t*);
typedef int (*PLUGIN_FNCGET)(unsigned int, wchar_t*);
typedef size_t (*PLUGIN_FNCGETCOUNT)(  );
typedef bool (*PLUGIN_FNCEXECUTE)(const wchar_t*, const wchar_t*, bool );
typedef bool (*PLUGIN_FNCOMMAND)(const wchar_t*, const wchar_t*);
typedef bool (*PLUGIN_FINDCOMMAND)(unsigned int, const wchar_t*, unsigned int, wchar_t*);
typedef void* (*PLUGIN_FOREGROUNDWINDOW)( );

/**
 * Todo
 * @param void
 * @return void
 */
AmPlugin::AmPlugin()
{
}

/**
 * Todo
 * @param void
 * @return void
 */
AmPlugin::~AmPlugin()
{
}

/**
 * Add a 
 * @param void
 * @return void
 */
void AmPlugin::Add(const wchar_t* name, void* pFunc )
{
  m_pFunction[ name ] = pFunc;
}

/**
 * Todo
 * @param void
 * @return void
 */
void* AmPlugin::Get(const wchar_t* name ) const
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
bool AmPlugin::Say(const wchar_t* msg, unsigned int nElapse, unsigned int nFadeOut)
{
  void* pFunc = Get( L"say" );
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
double AmPlugin::Version ()
{
  void* pFunc = Get( L"version" );
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
size_t AmPlugin::GetCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer )
{
  void* pFunc = Get( L"getCommand" );
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
size_t AmPlugin::GetFile(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const
{
  void* pFunc = Get( L"getFile" );
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
size_t AmPlugin::GetFolder(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const
{
  void* pFunc = Get( L"getFolder" );
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
size_t AmPlugin::GetURL(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const
{
  void* pFunc = Get( L"getURL" );
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
int AmPlugin::GetString(unsigned int nBufferLength, wchar_t* lpBuffer)
{
  void* pFunc = Get( L"getString" );
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
size_t AmPlugin::GetAction(unsigned int nBufferLength, wchar_t* lpBuffer)
{
  void* pFunc = Get( L"getAction" );
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
size_t AmPlugin::GetCommandCount() const
{
  void* pFunc = Get( L"getCommandCount" );
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
bool AmPlugin::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged)
{
  void* pFunc = Get( L"execute" );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCEXECUTE)(pFunc))( module, cmdLine, isPrivileged );
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
bool AmPlugin::AddAction(const wchar_t* szText, const wchar_t* szPath )
{
  void* pFunc = Get( L"addAction" );
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
bool AmPlugin::RemoveAction(const wchar_t* szText, const wchar_t* szPath )
{
  void* pFunc = Get( L"removeAction" );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FNCOMMAND)(pFunc))( szText, szPath );
}

/**
 * Add a set of command to the list of commands.
 * Note that we do hardly any checks to see of the command already exists
 * @param unsigned int the index of the action we are looking for.
 * @param LPCTSTR the name of the command we want to find
 * @param unsigned int the max size of the buffer.
 * @param wchar_t* if the action exists, return the path for it.
 * @return bool if the action exits or not.
 */
bool AmPlugin::FindAction(unsigned int idx, const wchar_t* szText, unsigned int nBufferLength, wchar_t* lpBuffer)
{
  void* pFunc = Get( L"findAction" );
  if( !pFunc )
  {
    return false;
  }
  return ((PLUGIN_FINDCOMMAND)(pFunc))( idx, szText, nBufferLength, lpBuffer );
}

/**
* Get the last foreground window.
* @return void* the last foregorund window.
*/
void * AmPlugin::GetForegroundWindow() const
{
  void* pFunc = Get(L"getForegroundWindow");
  if (!pFunc)
  {
    return NULL;
  }
  return ((PLUGIN_FOREGROUNDWINDOW)(pFunc))();
}