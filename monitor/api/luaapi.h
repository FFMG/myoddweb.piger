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

class LuaApi : public HelperApi
{
public:
  LuaApi(const ActiveAction& action);
  virtual ~LuaApi();

public:

  // static functions
  int Version (lua_State *lua);
  int Say (lua_State *lua);
  int GetAction (lua_State *lua);
  int GetCommand (lua_State *lua);
  int GetCommandCount (lua_State *lua);
  int Execute (lua_State *lua);
  int Getstring( lua_State *lua );
  int GetVersion( lua_State *lua );
  int Getfile( lua_State *lua );
  int Getfolder( lua_State *lua );
  int Geturl( lua_State *lua );
  int AddAction( lua_State *lua );
  int RemoveAction( lua_State *lua );
  int FindAction( lua_State *lua );
};

#endif /* ACTIONMONITOR_API_LUA */