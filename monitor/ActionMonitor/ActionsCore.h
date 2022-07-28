#pragma once

//////////////////////////////////////////////////////////////////////

#ifndef __ActionsCore_h__
#define __ActionsCore_h__

#pragma once

//  some common core keyboard functions
static unsigned int UWM_KEYBOARD_CHAR = RegisterWindowMessage(UWM_KEYBOARD_MSG_CHAR);
static unsigned int UWM_KEYBOARD_UP = RegisterWindowMessage(UWM_KEYBOARD_MSG_UP);
static unsigned int UWM_KEYBOARD_DOWN = RegisterWindowMessage(UWM_KEYBOARD_MSG_DOWN);

// IPC messages display.
static unsigned int UWM_DISPLAYMESSAGE = RegisterWindowMessage( L"UWM_MSG_DISPLAYMESSAGE" );
static unsigned int UWM_WAITFORALLMESSAGES = RegisterWindowMessage(L"UWM_WAITFORALLMESSAGES");

//  some core functions
static const wchar_t* ACTION_CORE_BYE = L"this.bye";       //  close the app
static const wchar_t* ACTION_CORE_LOAD = L"this.reload";    //  reload the list of items
static const wchar_t* ACTION_CORE_VERSION = L"this.version";   //  the version number
static const wchar_t* ACTION_CORE_LOG = L"this.log";   //  open the log file

#endif // __ActionsCore_h__