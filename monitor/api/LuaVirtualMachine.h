#pragma once

#ifdef ACTIONMONITOR_API_LUA

#include <mutex>

// support for LUA
#include "luaapi.h"

class LuaVirtualMachine
{
public:
  LuaVirtualMachine(void);
  virtual ~LuaVirtualMachine(void);
  
  int LoadFile( LPCTSTR luaFile, ActiveAction* action);

  static bool IsLuaExt( LPCTSTR ext );

protected:
  lua_State* Create(ActiveAction* action);
  void Dispose(lua_State* lua);
  void Dispose();

protected:
  std::mutex _mutex;
  typedef std::map<lua_State*, luaapi*> state_api;
  state_api _lua_Api;

  static luaapi& GetApi(lua_State* lua);

public:
  static int version(lua_State *lua);
  static int say(lua_State *lua);
  static int getAction(lua_State *lua);
  static int getCommand(lua_State *lua);
  static int getCommandCount(lua_State *lua);
  static int execute(lua_State *lua);
  static int getstring(lua_State *lua);
  static int getVersion(lua_State *lua);
  static int getfile(lua_State *lua);
  static int getfolder(lua_State *lua);
  static int geturl(lua_State *lua);
  static int addAction(lua_State *lua);
  static int removeAction(lua_State *lua);
  static int findAction(lua_State *lua);
};

#endif /*ACTIONMONITOR_API_LUA*/