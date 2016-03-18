#include "stdafx.h"

//
// this is the path to amplugin.h and all the plugin/libs
// by default all the libs, (debug/x64/x86) are copied to the /Plugins/ folder.
// 
// If you want to copy them somewhere else
// 1) Make sure that the inlcude path below is valid
// 2) Make sure that the library path is valid
//     Project > Properties > Linker > "Aditional Library properties"
//    The path that was used is relative to the "Piger" project.
#include "../../amplugin.h"

PLUGIN_API
AM_RESPONSE am_Msg(AM_MSG msg, AM_UINT wParam, AM_INT lParam)
{
  switch (msg)
  {
  case AM_MSG_INIT:
    {
      //
      // the plugin manager.
      amplugin* p = (amplugin*)(lParam);

      //
      // get the full path of this plugin
      // call 'getCommand( 0, ... ) and it will return this file.
      WCHAR szPath[MAX_PATH];
      size_t l = p->GetCommand(0, MAX_PATH, szPath);

      //
      // This is the name of the action e will call
      // the second param is who to call when this happens
      // in this case, we want Piger to call us back here.
      // but we could launch something else.
      p->AddAction(L"OnTop", szPath);
    }
    break;

  case AM_MSG_DEINIT:
    break;

  case AM_MSG_MAIN:
    {
      //
      // Our action as called!
      amplugin* p = (amplugin*)(lParam);

      // try and say something
      p->Say(L"Hello from Sample plugin!", 100, 100);
    }
    break;

  case AM_MSG_NAME:
    //  get the name of this action
    // WPARAM is the size of the buffer.
    // LPARAM is the buffer, (wchar_t*)
    return AM_RESP_NOT_SUPPORTED;
    break;

  case AM_MSG_PATH_CMD:     //  this is the root path of the commands, %appdata%\myoddweb\ActionMonitor\RootCommands
  case AM_MSG_PATH_IN:      //  this is the path of actions called on Piger start up, %appdata%\myoddweb\ActionMonitor\RootCommands\__in
  case AM_MSG_PATH_OUT:     //  this is the path of actions called on Piger closing down, %appdata%\myoddweb\ActionMonitor\RootCommands\__out
  case AM_MSG_PATH_TMP:     //  this is the path ignored by piger, %appdata%\myoddweb\ActionMonitor\RootCommands\__tmp
  case AM_MSG_PATH_PLUGIN:  //  the path where the plugins are located, (also where _we_ are located).
    // Paths - lParam is a pointer to const wchar_t*
    // const wchar_t* path = (const wchar_t*)lParam
    return AM_RESP_NOT_SUPPORTED;
    break;

  default:
    break;
  }

  // not handled here.
  return AM_RESP_NOT_SUPPORTED;
}