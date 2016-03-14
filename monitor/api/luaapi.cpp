#include "StdAfx.h"

#ifdef ACTIONMONITOR_API_LUA
#include "luaapi.h"
#include "luaVirtualMachine.h"

/**
 * Todo
 * @param lua_State
 * @return void
 */
luaapi::luaapi(const ActiveAction& action) : helperapi( action )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
luaapi::~luaapi(void)
{
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::version (lua_State *lua)
{
  // short and sweet
  // all we need is the version number.
  lua_pushnumber(lua, ACTIONMONITOR_API_LUA_VERSION );

  // we passed one items
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::execute (lua_State *lua)
{
  // get the number of arguments.
  // we can only have one or 2 arguments 
  // 
  // the first argument is the name of the module
  // and the second is the arguments.
  int n = lua_gettop( lua );
  if( n < 1 || n > 3 )
  {
    __super::say( _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am_execute( module [, commandLine [, privileged])</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  USES_CONVERSION;

  LPCTSTR module  = T_A2T( lua_tostring (lua, 1) );
  LPCTSTR cmdLine = T_A2T( lua_tostring (lua, 2) );

  bool isPrivileged = false;
  if (n == 3)
  {
    if (!lua_isboolean(lua, 3))
    {
      __super::say(_T("<b>Error : </b> The third argument, (privileged), can only be true|false"), 3000, 5);
      lua_pushboolean(lua, false);
      return 1;
    }
    isPrivileged = (lua_toboolean(lua, 3 ) == 1);
  }

  // run the query
  bool result = __super::execute(module, cmdLine, isPrivileged );

  // push the result.
  lua_pushboolean ( lua, result );

  // return the number of results
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getCommandCount(lua_State *lua)
{
  // get it
  size_t nSize = __super::getCommandCount();
  
  // and push it to LUA.
  lua_pushinteger(lua, nSize );

  // we are returning one argument.
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getCommand (lua_State *lua)
{
  static const int ARGUMENT_NUMBER = 1;
  int n = lua_gettop( lua );
  if( n != 1 || !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getCommand( <b>index</b> )</i>"), 3000, 5 );
    lua_pushboolean( lua, false );
    
    // return the number of results
    return 1;
  }

  size_t idx = (size_t)lua_tointeger (lua, ARGUMENT_NUMBER);
  STD_TSTRING sValue;
  if( !__super::getCommand( idx, sValue ) )
  {
    lua_pushboolean( lua, false );

    // return the number of results
    return 1;
  }

  USES_CONVERSION;

  // push the value
  lua_pushstring(lua, T_T2A( sValue.c_str() ) );

  // return the number of results
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getAction (lua_State *lua)
{
  static const int ARGUMENT_NUMBER = 1;
  int n = lua_gettop( lua );
  if( n != 0 )
  {
    __super::say( _T("<b>Error : </b>.<br>Format is <i>am_getAction( )</i>"), 3000, 5 );
    lua_pushboolean( lua, false );
    
    // return the number of results
    return 1;
  }

  STD_TSTRING sValue;
  if( !__super::getAction( sValue ) )
  {
    lua_pushboolean( lua, false );

    // return the number of results
    return 1;
  }

  USES_CONVERSION;

  // push the value
  lua_pushstring(lua, T_T2A( sValue.c_str() ) );

  // return the number of results
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::say (lua_State *lua)
{
  USES_CONVERSION;

  // the first item is the message.
  LPCTSTR msg = T_A2T( lua_tostring (lua, 1) );
  UINT nElapse = (UINT)lua_tointeger (lua, 2);
  if(  nElapse == 0 )                                                               
  {
    __super::say( _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am_say( msg, <b>elapse</b>, fade=0)</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  // if it is 0 then we fade rapidely
  UINT nFadeOut = (UINT)lua_tointeger (lua, 3);

  // and we can now display the message.
  bool result = __super::say( msg, nElapse, nFadeOut );
  lua_pushboolean ( lua, result );

  // return the number of results
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getstring( lua_State *lua )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getString( sValue ) )
  {
    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  USES_CONVERSION;
  lua_pushstring(lua, T_T2A(sValue.c_str()) );

  // we have one item
  return 1;
}

/**
 * Get the action monitor version string.
 * @param lua_State *
 * @return int
 */
int luaapi::getVersion( lua_State *lua )
{
  STD_TSTRING sValue = _T("");
  if( !__super::getVersion( sValue ) )
  {
    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  USES_CONVERSION;
  lua_pushstring(lua, T_T2A(sValue.c_str()) );

  // we have one item
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getfile( lua_State *lua )
{
  static const int ARGUMENT_NUMBER = 1;
  int n = lua_gettop( lua );
  if( n != 1 || !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfile( <b>index</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  UINT idx = (UINT)lua_tointeger (lua, 1);
  STD_TSTRING sValue = _T("");
  if( !__super::getFile( idx, sValue ) )
  {
    lua_pushboolean ( lua, false );
    return 1;
  }

  // otherwise push the string
  USES_CONVERSION;
  LPCTSTR c = sValue.c_str();
  lua_pushstring(lua, T_T2A( c ) );

  // we have one item
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::getfolder( lua_State *lua )
{
  static const int ARGUMENT_NUMBER = 1;
  int n = lua_gettop( lua );
  if( n != 1 || !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfolder( <b>index</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  UINT idx = (UINT)lua_tointeger (lua, 1);
  STD_TSTRING sValue = _T("");
  if( !__super::getFolder( idx, sValue) )
  {
    //  just return false.
    lua_pushboolean ( lua, false );
    return 1;
  }

  USES_CONVERSION;

  // otherwise push the string
  LPCTSTR c = sValue.c_str();
  lua_pushstring(lua, T_T2A(c) );

  // we have one item
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::geturl( lua_State *lua )
{
  static const int ARGUMENT_NUMBER = 1;
  int n = lua_gettop( lua );
  if( n != 1 || !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am_geturl( <b>index</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  UINT idx = (UINT)lua_tointeger (lua, 1);
  STD_TSTRING sValue = _T("");
  if( !__super::getURL( idx, sValue) )
  {
    //  just return false.
    lua_pushboolean ( lua, false );
    return 1;
  }

  USES_CONVERSION;

  // otherwise push the string
  LPCTSTR c = sValue.c_str();
  lua_pushstring(lua, T_T2A(c) );

  // we have one item
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::addAction( lua_State *lua )
{
  //  get the number of arguments
  int n = lua_gettop( lua );

  // we must have 2 items
  if( n != 2 )
  {
    __super::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am_addAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  USES_CONVERSION;

  LPCTSTR szText = T_A2T( lua_tostring (lua, 1) );
  LPCTSTR szPath = T_A2T( lua_tostring (lua, 2) );
  bool r = __super::addAction( szText, szPath );
  
  lua_pushboolean ( lua, r );
  return 1;
}

/**
 * Todo
 * @param lua_State *
 * @return int
 */
int luaapi::removeAction( lua_State *lua )
{
  //  get the number of arguments
  int n = lua_gettop( lua );

  // we must have 1 or 2
  if( n != 2 )
  {
    __super::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am_removeAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  LPCSTR szText = lua_tostring (lua, 1);
  LPCSTR szPath = lua_tostring (lua, 2);
  bool r = __super::removeAction( helperapi::widen(szText).c_str(), helperapi::widen(szPath).c_str() );
  
  lua_pushboolean ( lua, r );
  return 1;
}

/**
 * Get the action monitor version string.
 * @param lua_State *
 * @return int
 */
int luaapi::findAction( lua_State *lua )
{
  //  get the number of arguments
  int n = lua_gettop( lua );

  // we must have 2 arguments.
  if( n != 2 )
  {
    __super::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am_findAction( <b>index</b>, <b>action</b> )</i>"), 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }


  UINT idx = (UINT)lua_tointeger (lua, 1);
  LPCSTR action = lua_tostring (lua, 2);

  STD_TSTRING sValue = _T("");
  if( !__super::findAction( idx, helperapi::widen( action ).c_str(), sValue ) )
  {
    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  USES_CONVERSION;
  lua_pushstring(lua, T_T2A(sValue.c_str()) );

  // one return variable.
  return 1;
}

#endif /* ACTIONMONITOR_API_LUA */