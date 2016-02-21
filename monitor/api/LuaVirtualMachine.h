#pragma once

#include "luaapi.h"

class LuaVirtualMachine
{
public:
  LuaVirtualMachine(void);
  virtual ~LuaVirtualMachine(void);

  void Initialize(); 

  int LoadFile( LPCTSTR luaFile );
  int Register( LPCTSTR lpName, const lua_CFunction& fn );

  bool IsLuaExt( LPCTSTR ext ) const;
protected:
  lua_State* m_lua_State;
};

// the Lua virtual machine
LuaVirtualMachine& GetLuaVirtualMachine();