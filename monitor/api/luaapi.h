#pragma once

// this is the version number for that particular API
static const double ACTIONMONITOR_API_LUA_VERSION = 0.1f;

// support for LUA
extern "C" 
{
  #include "lua/src/lua.h"
  #include "lua/src/lualib.h"
  #include "lua/src/lauxlib.h"
}

// add the LUA libs
#ifdef _WIN64
# pragma comment(lib, "lua64.lib" )
#else
# pragma comment(lib, "lua.lib" )
#endif

class luaapi
{
protected: // all static
  luaapi(void);
  ~luaapi(void);

public:
  static void Initialize();

  static int version (lua_State *lua);
  static int say (lua_State *lua);
  static int getAction (lua_State *lua);
  static int getCommand (lua_State *lua);
  static int getCommandCount (lua_State *lua);
  static int execute (lua_State *lua);
  static int getstring( lua_State *lua );
  static int getVersion( lua_State *lua );
  static int getfile( lua_State *lua );
  static int getfolder( lua_State *lua );
  static int geturl( lua_State *lua );
  static int addAction( lua_State *lua );
  static int removeAction( lua_State *lua );
  static int findAction( lua_State *lua );
};