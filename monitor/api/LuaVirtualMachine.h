#pragma once
#include "IVirtualMachine.h"

#ifdef ACTIONMONITOR_API_LUA

#include <mutex>

// support for LUA
#include "luaapi.h"

class LuaVirtualMachine final : public IVirtualMachine
{
public:
  explicit LuaVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler );
  ~LuaVirtualMachine();
  
  static bool IsExt(const MYODD_STRING& file );

  int Execute( const ActiveAction& action, const std::wstring& pluginFile) override;
  void Destroy() override{};
  bool Initialize() override { return true; };

protected:
  lua_State* CreateState(LuaApi* api );
  void Dispose(lua_State* lua);
  void Dispose();

protected:
  std::mutex _mutex;
  typedef std::map<lua_State*, LuaApi*> state_api;
  state_api _lua_Api;

  static LuaApi& GetApi(lua_State* lua);

public:
  static int Version(lua_State *lua);
  static int Say(lua_State *lua);
  static int GetAction(lua_State *lua);
  static int GetCommand(lua_State *lua);
  static int GetCommandCount(lua_State *lua);
  static int Execute(lua_State *lua);
  static int Getstring(lua_State *lua);
  static int GetVersion(lua_State *lua);
  static int Getfile(lua_State *lua);
  static int Getfolder(lua_State *lua);
  static int Geturl(lua_State *lua);
  static int AddAction(lua_State *lua);
  static int RemoveAction(lua_State *lua);
  static int FindAction(lua_State *lua);
  static int Log(lua_State *lua);
};

#endif /*ACTIONMONITOR_API_LUA*/