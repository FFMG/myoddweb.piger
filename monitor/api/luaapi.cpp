#include "StdAfx.h"

#ifdef ACTIONMONITOR_API_LUA

// this is the version number for that particular API
// 0.1 was the old API, not idea what version of Python it was using.
// 2.0 uses version 5.3.
// 3.0 added Log( ... )
// 3.1 fixed some minor issues with bool > boolean
// 3.2 check all the params type/number
// 3.3 fixed lua execute with just one element.
// 3.4 some code cleanup
// 3.5 C++11 code cleanup
static const double ACTIONMONITOR_API_LUA_VERSION = 3.5;

#include "luaapi.h"
#include "../string/string.h"

/**
 * \brief Todo
 * \param action the action that we are calling
 */
LuaApi::LuaApi(const ActiveAction& action, IMessages& messages) : HelperApi( action, messages )
{
}

/**
 * \brief the destructor
 */
LuaApi::~LuaApi(void)
{
}

/**
 * \brief Get the Monitor version number
 * \param lua the current LUA state.
 * \return success or not.
 */
int LuaApi::Version (lua_State *lua) const
{
  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments > 0 )
  {
    const auto errorMsg = _T("<b>Error : </b> The 'Version function does not take any parametters.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say( errorMsg , 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  // short and sweet
  // all we need is the version number.
  lua_pushnumber(lua, ACTIONMONITOR_API_LUA_VERSION );

  // return the number of results
  return 1;
}

/**
 * \brief Execute a module.
 * \param lua the current LUA state.
 * \return success or not.
 */
int LuaApi::Execute (lua_State *lua) const
{
  static const auto ARGUMENT_MODULE = 1;
  static const auto ARGUMENT_ARGS = 2;
  static const auto ARGUMENT_PRIVILEGED = 3;

  // get the number of arguments.
  // we can only have one or 2 arguments 
  // 
  // the first argument is the name of the module
  // and the second is the arguments.
  const auto numberOfArguments = lua_gettop( lua );
  if( numberOfArguments < 1 || numberOfArguments > 3 )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am_execute( module [, commandLine [, privileged])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg , 3000, 5 );
    lua_pushboolean ( lua, false );

    // return the number of results
    return 1;
  }

  // do we have a module?
  if (!lua_isstring(lua, ARGUMENT_MODULE) )
  {
    const auto errorMsg = _T("<b>Error : </b> The first parameters must be strings.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // do we have an argument?
  if ( numberOfArguments >= 2 && !lua_isstring(lua, ARGUMENT_ARGS))
  {
    const auto errorMsg = _T("<b>Error : </b> The second parameters must be a string.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  auto isPrivileged = false;
  if (numberOfArguments == 3)
  {
    if (!lua_isboolean(lua, ARGUMENT_PRIVILEGED))
    {
      auto errorMsg = _T("<b>Error : </b> The third argument, (privileged), can only be true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);
      return 1;
    }
    isPrivileged = (lua_toboolean(lua, ARGUMENT_PRIVILEGED) == 1);
  }

  //  get the module
  auto module = (numberOfArguments >= 1) ? myodd::strings::String2WString(lua_tostring(lua, ARGUMENT_MODULE)) : L"";

  // and the command line argument
  auto cmdLine = (numberOfArguments >= 2) ? myodd::strings::String2WString(lua_tostring(lua, ARGUMENT_ARGS)) : L"";

  // run the query
  const auto result = __super::Execute(module.c_str(), cmdLine.c_str(), isPrivileged, nullptr );

  // push the result.
  lua_pushboolean ( lua, result );

  // tell the user it did not work
  if (false == result)
  {
    const auto errorMsg = _T("<b>Error : </b> There was an error executing the request, please check the parameters.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
  }

  // return the number of results
  return 1;
}

/**
 * \brief Get the number of arguments passed.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetCommandCount(lua_State *lua) const
{
  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments > 0)
  {
    const auto errorMsg = _T("<b>Error : </b> The 'GetCommandCount' function does not take any parametters.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // get it
  const auto nSize = __super::GetCommandCount();
  
  // and push it to LUA.
  lua_pushinteger(lua, nSize );

  // we are returning one argument.
  return 1;
}

/**
 * \brief Get an arguments
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetCommand (lua_State *lua) const
{
  static const auto ARGUMENT_NUMBER = 1;
  const auto numberOfArguments = lua_gettop( lua );
  if( numberOfArguments != 1 || !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getCommand( <b>index</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  const auto idx = static_cast<unsigned int>(lua_tointeger (lua, ARGUMENT_NUMBER));
  MYODD_STRING sValue;
  if( !__super::GetCommand( idx, sValue ) )
  {
    const auto errorMsg = _T("Trying to get an argument past the number of arguments.");
    __super::Log(AM_LOG_WARNING, errorMsg);

    lua_pushboolean( lua, false );

    // return the number of results
    return 1;
  }

  // push the value
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str() );

  // return the number of results
  return 1;
}

/**
 * \brief Get an action
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetAction (lua_State *lua) const
{
  const auto numberOfArguments = lua_gettop( lua );
  if( numberOfArguments != 0 )
  {
    const auto errorMsg = _T("<b>Error : </b>.<br>Format is <i>am_getAction( )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean( lua, false );
    
    // return the number of results
    return 1;
  }

  MYODD_STRING sValue;
  if( !__super::GetAction( sValue ) )
  {
    const auto errorMsg = _T("Trying to get the action name/value.");
    __super::Log(AM_LOG_WARNING, errorMsg);

    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // push the value
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // return the number of results
  return 1;
}

/**
 * \brief Say a message on screen 
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::Say (lua_State *lua) const
{
  const auto ARGUMENT_TEXT = 1;
  const auto ARGUMENT_ELAPSE = 2;
  const auto ARGUMENT_FADEOUT = 3;

  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments < 2 || numberOfArguments > 3)
  {
    const auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am_say( msg, <b>elapse</b>[, fade=0])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  if (!lua_isstring( lua, ARGUMENT_TEXT))
  {
    auto errorMsg = _T("<b>Error : </b> The first argument must be a string.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  if (!lua_isinteger(lua, ARGUMENT_ELAPSE))
  {
    auto errorMsg = _T("<b>Error : </b> The elapse parameters must be an integer.");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // the first item is the message.
  const auto msg = myodd::strings::String2WString( lua_tostring (lua, ARGUMENT_TEXT) );
  const auto nElapse = (unsigned int)lua_tointeger (lua, ARGUMENT_ELAPSE);
  if(  nElapse == 0 )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am_say( msg, <b>elapse</b>[, fade=0])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say( errorMsg, 3000, 5 );
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // if it is 0 then we fade rapidely
  unsigned int nFadeOut = 0;
  if (numberOfArguments == 3)
  {
    if (!lua_isinteger(lua, ARGUMENT_FADEOUT))
    {
      const auto errorMsg = _T("<b>Error : </b> The fadout parameters must be an integer.");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);

      // return the number of results
      return 1;
    }
    nFadeOut = (unsigned int)lua_tointeger(lua, ARGUMENT_FADEOUT);
  }

  // and we can now display the message.
  const auto result = __super::Say( msg.c_str(), nElapse, nFadeOut );
  lua_pushboolean ( lua, result );

  // return the number of results
  return 1;
}

/**
 * \brief Get the currently selected string.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::Getstring( lua_State *lua ) const
{
  const auto ARGUMENT_QUOTE = 1;

  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments > 1 )
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getstring' does not take more than one parameter");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  // the quote
  auto bQuote = true;
  if (numberOfArguments == 1)
  {
    if (!lua_isboolean(lua, ARGUMENT_QUOTE) )
    {
      auto errorMsg = _T("The first parameter must be a boolean.");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);

      // return the number of results
      return 1;
    }
    bQuote = (1 == lua_toboolean(lua, ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if( !__super::GetString( sValue, bQuote ) )
  {
    __super::Log(AM_LOG_WARNING, _T("Could not get any selected string."));

    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // we have one item
  return 1;
}

/**
 * \brief Get the action monitor version string.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetVersion( lua_State *lua ) const
{
  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments > 0)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'GetVersion' does not take any parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  MYODD_STRING sValue = _T("");
  if( !__super::GetVersion( sValue ) )
  {
    __super::Log(AM_LOG_ERROR, _T("Unable to get the version number") );

    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // we have one item
  return 1;
}

/**
 * \brief Get a file at a given index.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetFile( lua_State *lua ) const
{
  //  the arguments.
  const auto ARGUMENT_NUMBER = 1;
  const auto ARGUMENT_QUOTE = 2;

  const auto numberOfArguments = lua_gettop( lua );
  if (numberOfArguments < 1 || numberOfArguments > 2 )
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getfile' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  if( !lua_isnumber(lua, ARGUMENT_NUMBER ) )
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfile( <b>index</b>[, quote])</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say( errorMsg, 3000, 5 );
    lua_pushboolean ( lua, false );

    // return the number of results
    return 1;
  }

  //  get the nunber
  const auto idx = (unsigned int)lua_tointeger (lua, ARGUMENT_NUMBER );

  // the quote
  auto bQuote = true;
  if (numberOfArguments == 2)
  {
    if (!lua_isboolean(lua, ARGUMENT_QUOTE))
    {
      const auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);

      // return the number of results
      return 1;
    }
    bQuote = (1 == lua_toboolean(lua, ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if( !__super::GetFile( idx, sValue, bQuote ) )
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested file index."));
    lua_pushboolean ( lua, false );

    // return the number of results
    return 1;
  }

  // otherwise push the string
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // we have one item
  return 1;
}

/**
 * \brief Get a folder at a given index.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetFolder( lua_State *lua ) const
{
  const auto ARGUMENT_NUMBER = 1;
  const auto ARGUMENT_QUOTE = 2;

  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments < 1 || numberOfArguments > 2)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Getfolder' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  if (!lua_isnumber(lua, ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_getfolder( <b>index</b>[, quote] )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  //  the number.
  const auto idx = static_cast<unsigned int>(lua_tointeger (lua, ARGUMENT_NUMBER));

  // the quote
  auto bQuote = true;
  if (numberOfArguments >= 2)
  {
    if (!lua_isboolean(lua, ARGUMENT_QUOTE))
    {
      auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);

      // return the number of results
      return 1;
    }
    bQuote = (1 == lua_toboolean(lua, ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if( !__super::GetFolder( idx, sValue, bQuote ) )
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested folder index."));

    //  just return false.
    lua_pushboolean ( lua, false );
    return 1;
  }

  // otherwise push the string
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // we have one item
  return 1;
}

/**
 * \brief Get a url at a given index.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::GetUrl( lua_State *lua ) const
{
  const auto ARGUMENT_NUMBER = 1;
  const auto ARGUMENT_QUOTE = 2;

  const auto numberOfArguments = lua_gettop(lua);
  if (numberOfArguments < 1 || numberOfArguments > 2)
  {
    const auto errorMsg = _T("<b>Error : </b> The function 'Geturl' takes 1 or 2 parameters");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  if (!lua_isnumber(lua, ARGUMENT_NUMBER))
  {
    auto errorMsg = _T("<b>Error : </b> Missing index number.<br>Format is <i>am_geturl( <b>index</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);

    // return the number of results
    return 1;
  }

  //  get the index.
  const auto idx = static_cast<unsigned int>(lua_tointeger (lua, ARGUMENT_NUMBER));

  // the quote
  auto bQuote = true;
  if (numberOfArguments >= 2)
  {
    if (!lua_isboolean(lua, ARGUMENT_QUOTE))
    {
      const auto errorMsg = _T("<b>Error : </b> The second parameter must be a boolean true|false");
      __super::Log(AM_LOG_ERROR, errorMsg);
      __super::Say(errorMsg, 3000, 5);
      lua_pushboolean(lua, false);

      // return the number of results
      return 1;
    }
    bQuote = (1 == lua_toboolean(lua, ARGUMENT_QUOTE));
  }

  MYODD_STRING sValue = _T("");
  if( !__super::GetUrl( idx, sValue, bQuote) )
  {
    __super::Log(AM_LOG_WARNING, _T("Unable to get the requested url index."));

    //  just return false.
    lua_pushboolean ( lua, false );
    return 1;
  }

  // otherwise push the string
  auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // we have one item
  return 1;
}

/**
 * \brief Add a given action
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::AddAction( lua_State *lua ) const
{
  const auto ARGUMENT_ACTION = 1;
  const auto ARGUMENT_PATH = 2;

  //  get the number of arguments
  const auto numberOfArguments = lua_gettop(lua);

  // we must have 2 items
  if(numberOfArguments != 2 )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_addAction( <b>action</b>, <b>path</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say( errorMsg, 3000, 5 );
    lua_pushboolean ( lua, false );
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_ACTION))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_PATH))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'path' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  // get the action and path
  const auto szAction = myodd::strings::String2WString( lua_tostring (lua, ARGUMENT_ACTION) );
  const auto szPath = myodd::strings::String2WString( lua_tostring (lua, ARGUMENT_PATH ) );

  // add it, (or try).
  const auto result = __super::AddAction(szAction.c_str(), szPath.c_str() );
  
  // return if it works.
  lua_pushboolean ( lua, result);
  return 1;
}

/**
 * \brief Remove a given action
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::RemoveAction( lua_State *lua ) const
{
  const auto ARGUMENT_ACTION = 1;
  const auto ARGUMENT_PATH = 2;

  //  get the number of arguments
  const auto numberOfArguments = lua_gettop(lua);

  // we must have 1 or 2
  if(numberOfArguments != 2 )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_removeAction( <b>action</b>, <b>path</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean ( lua, false );
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_ACTION))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_PATH))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'path' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  const auto szAction = myodd::strings::String2WString(lua_tostring(lua, ARGUMENT_ACTION));
  const auto szPath = myodd::strings::String2WString(lua_tostring(lua, ARGUMENT_PATH));
  const auto result = __super::RemoveAction(szAction.c_str(), szPath.c_str());
  
  // return if it works.
  lua_pushboolean ( lua, result);

  // one argument
  return 1;
}

/**
 * \brief Find an action at a given index.
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::FindAction( lua_State *lua ) const
{
  const auto ARGUMENT_INDEX = 1;
  const auto ARGUMENT_ACTION = 2;

  //  get the number of arguments
  const auto numberOfArguments = lua_gettop(lua);

  // we must have 2 arguments.
  if(numberOfArguments != 2 )
  {
    const auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_findAction( <b>index</b>, <b>action</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean ( lua, false );
    return 1;
  }

  if (!lua_isinteger(lua, ARGUMENT_INDEX))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'index' must be a number");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_ACTION))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'action' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  const auto idx = (unsigned int)lua_tointeger (lua, ARGUMENT_INDEX);
  const auto action = myodd::strings::String2WString(lua_tostring(lua, ARGUMENT_ACTION));

  MYODD_STRING sValue = _T("");
  if( !__super::FindAction( idx, action.c_str(), sValue ) )
  {
    __super::Log(AM_LOG_WARNING, _T("Could not find action at given index"));

    //  just return false.
    lua_pushboolean ( lua, false );

    // one return variable.
    return 1;
  }

  // otherwise push the string
  const auto asValue = myodd::strings::WString2String(sValue);
  lua_pushstring(lua, asValue.c_str());

  // one return variable.
  return 1;
}

/**
 * \brief Log a message 
 * \param lua the current LUA state.
 * \return the number of arguments being returned.
 */
int LuaApi::Log(lua_State *lua) const
{
  const auto ARGUMENT_LOGTYPE = 1;
  const auto ARGUMENT_MESSAGE = 2;

  //  get the number of arguments
  const auto numberOfArguments = lua_gettop(lua);

  // we must have 2 items
  if (numberOfArguments != 2)
  {
    const auto errorMsg = _T("<b>Error : </b> Missing values.<br>Format is <i>am_Log( <b>logType</b>, <b>string</b> )</i>");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  if (!lua_isinteger(lua, ARGUMENT_LOGTYPE))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'logtype' must be a number");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  if (!lua_isstring(lua, ARGUMENT_MESSAGE))
  {
    const auto errorMsg = _T("<b>Error : </b> Invalid argument type, the 'message' must be a string");
    __super::Log(AM_LOG_ERROR, errorMsg);
    __super::Say(errorMsg, 3000, 5);
    lua_pushboolean(lua, false);
    return 1;
  }

  const auto logType = (unsigned int)lua_tointeger(lua, ARGUMENT_LOGTYPE);
  const auto action = myodd::strings::String2WString( lua_tostring(lua, ARGUMENT_MESSAGE));

  __super::Log(logType, action.c_str() );

  // success.
  lua_pushboolean(lua, true );
  return 1;
}

#endif /* ACTIONMONITOR_API_LUA */