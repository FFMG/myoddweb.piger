#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "luavirtualmachine.h"

/**
 * Todo
 * @param void
 */
static LuaVirtualMachine luaVirtualMachine;

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine& GetLuaVirtualMachine()
{
  luaVirtualMachine.Initialize();
  return (luaVirtualMachine);
}

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine::LuaVirtualMachine(void)
{
  m_lua_State = NULL;
}

/**
 * Todo
 * @param void
 * @return void
 */
LuaVirtualMachine::~LuaVirtualMachine(void)
{
  if( m_lua_State )
  {
    lua_close( m_lua_State );
  }
}

/**
 * Todo
 * @return void
 */
void LuaVirtualMachine::Initialize()
{
  //  only do it once
  if( NULL == m_lua_State )
  {
    m_lua_State = luaL_newstate();
    luaL_openlibs(m_lua_State); // provides io.*

    // register our LUA functions.
    luaapi::Initialize( );
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
int LuaVirtualMachine::LoadFile( LPCTSTR luaFile )
{
  if( NULL == m_lua_State )
  {
    return -1;
  }

  USES_CONVERSION;

  int s = luaL_loadfile( m_lua_State , T_T2A(luaFile) );
  if( s != 0 )
  {
    STD_TSTRING sErr = T_A2T( lua_tostring( m_lua_State, -1) );
    myodd::log::LogError( _T("LUA Err: %s"), sErr.c_str() );
    return s;
  }

  // execute Lua program
  return lua_pcall(m_lua_State, 0, LUA_MULTRET, 0);
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
int LuaVirtualMachine::Register( LPCTSTR lpName, const lua_CFunction& fn )
{
  if( NULL == m_lua_State )
  {
    return -1;
  }

  USES_CONVERSION;
  lua_register( m_lua_State, T_T2A(lpName), fn ); 
  return 0;
}

/**
 * Check if a given file extension is used by this API or not.
 * @param LPCTSTR the extensions.
 * @return bool true|false if the given extension is LUA or not.
 */
bool LuaVirtualMachine::IsLuaExt( LPCTSTR ext ) const
{
  return ( _tcsicmp( ext, _T("lua") ) == 0 );
}