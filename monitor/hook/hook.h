#ifndef HOOK_HEADER
#define HOOK_HEADER

#include <tchar.h>

#pragma once

#ifdef HOOK_CPP
  #define HOOKLIB __declspec(dllexport)
#else
  #define HOOKLIB __declspec(dllimport)
#endif

HOOKLIB BOOL hook_set( HWND, WPARAM );
HOOKLIB BOOL hook_clear( HWND );

//  return the previous state of the keyboard
// this tells the system to reject every single key strokes.
HOOKLIB BOOL hook_RejectKeyboad( BOOL );

static LPCTSTR UWM_KEYBOARD_MSG_CHAR    = _T("UWM_KEYBOARD_MSG_CHAR");
static LPCTSTR UWM_KEYBOARD_MSG_UP      = _T("UWM_KEYBOARD_MSG_UP");
static LPCTSTR UWM_KEYBOARD_MSG_DOWN    = _T("UWM_KEYBOARD_MSG_DOWN");
static LPCTSTR UWM_KEYBOARD_MSG_RELOAD  = _T("UWM_KEYBOARD_MSG_RELOAD");
static LPCTSTR UWM_KEYBOARD_MSG_VERSION = _T("UWM_KEYBOARD_MSG_VERSION");

#endif  //  HOOK_HEADER