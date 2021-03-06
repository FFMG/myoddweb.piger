#pragma once

//////////////////////////////////////////////////////////////////////

#ifndef __ActionsCore_h__
#define __ActionsCore_h__

#pragma once

//  some common core keyboard functions
static UINT UWM_KEYBOARD_CHAR = RegisterWindowMessage(UWM_KEYBOARD_MSG_CHAR);
static UINT UWM_KEYBOARD_UP = RegisterWindowMessage(UWM_KEYBOARD_MSG_UP);
static UINT UWM_KEYBOARD_DOWN = RegisterWindowMessage(UWM_KEYBOARD_MSG_DOWN);

// IPC messages display.
static UINT UWM_DISPLAYMESSAGE = RegisterWindowMessage( L"UWM_MSG_DISPLAYMESSAGE" );
static UINT UWM_WAITFORALLMESSAGES = RegisterWindowMessage(L"UWM_WAITFORALLMESSAGES");

//  some core functions
static LPCTSTR ACTION_CORE_BYE = _T("this.bye");       //  close the app
static LPCTSTR ACTION_CORE_LOAD = _T("this.reload");    //  reload the list of items
static LPCTSTR ACTION_CORE_VERSION = _T("this.version");   //  the version number

#endif // __ActionsCore_h__