#pragma once

// add the LUA libs
#ifdef ACTIONMONITOR_API_LUA

#include "helperapi.h"

// this is the version number for that particular API
// 0.1 was the old API, not idea what version of Python it was using.
// 2.0 uses version 5.3.
static const double ACTIONMONITOR_API_LUA_VERSION = 2.0;

// support for LUA
extern "C" 
{
  #include "lua/src/lua.h"
  #include "lua/src/lualib.h"
  #include "lua/src/lauxlib.h"
}

# ifdef _WIN64
#  pragma comment(lib, "lua64.lib" )
# else
#  pragma comment(lib, "lua.lib" )
# endif

class luaapi : public helperapi
{
public:
  luaapi(ActiveAction* action);
  virtual ~luaapi();

public:

  // static functions
  int version (lua_State *lua);
  int say (lua_State *lua);
  int getAction (lua_State *lua);
  int getCommand (lua_State *lua);
  int getCommandCount (lua_State *lua);
  int execute (lua_State *lua);
  int getstring( lua_State *lua );
  int getVersion( lua_State *lua );
  int getfile( lua_State *lua );
  int getfolder( lua_State *lua );
  int geturl( lua_State *lua );
  int addAction( lua_State *lua );
  int removeAction( lua_State *lua );
  int findAction( lua_State *lua );
};

#endif /* ACTIONMONITOR_API_LUA */