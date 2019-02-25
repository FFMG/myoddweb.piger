#pragma once

#ifndef HOOK_HEADER
#define HOOK_HEADER

#ifdef HOOK_CPP
  #define HOOKLIB __declspec(dllexport)
#else
  #define HOOKLIB __declspec(dllimport)
#endif

HOOKLIB int hook_set( HWND, WPARAM );
HOOKLIB int hook_clear( HWND );

//  return the previous state of the keyboard
// this tells the system to reject every single key strokes.
HOOKLIB int hook_RejectKeyboad( int );

  #ifdef UNICODE
    static const wchar_t* UWM_KEYBOARD_MSG_CHAR = L"UWM_KEYBOARD_MSG_CHAR";
    static const wchar_t* UWM_KEYBOARD_MSG_UP = L"UWM_KEYBOARD_MSG_UP";
    static const wchar_t* UWM_KEYBOARD_MSG_DOWN = L"UWM_KEYBOARD_MSG_DOWN";
    static const wchar_t* UWM_KEYBOARD_MSG_RELOAD = L"UWM_KEYBOARD_MSG_RELOAD";
    static const wchar_t* UWM_KEYBOARD_MSG_EXIT = L"UWM_KEYBOARD_MSG_EXIT";
    static const wchar_t* UWM_KEYBOARD_MSG_VERSION = L"UWM_KEYBOARD_MSG_VERSION";
    static const wchar_t* UWM_MSG_MESSAGE_PUMP_READY = L"UWM_MSG_MESSAGE_PUMP_READY";
  #else
    static const char* UWM_KEYBOARD_MSG_CHAR = "UWM_KEYBOARD_MSG_CHAR";
    static const char* UWM_KEYBOARD_MSG_UP = "UWM_KEYBOARD_MSG_UP";
    static const char* UWM_KEYBOARD_MSG_DOWN = "UWM_KEYBOARD_MSG_DOWN";
    static const char* UWM_KEYBOARD_MSG_RELOAD = "UWM_KEYBOARD_MSG_RELOAD";
    static const char* UWM_KEYBOARD_MSG_EXIT = "UWM_KEYBOARD_MSG_EXIT";
    static const char* UWM_KEYBOARD_MSG_VERSION = "UWM_KEYBOARD_MSG_VERSION";
    static const char* UWM_MSG_MESSAGE_PUMP_READY = "UWM_MSG_MESSAGE_PUMP_READY";
  #endif // UNICODE
#endif  //  HOOK_HEADER