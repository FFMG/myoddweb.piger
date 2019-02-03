// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionVersion.h"
#include "ActionsCore.h"
#include "ActiveUserMessageAction.h"

/**
 * todo
 * @param void
 * @return void
 */
ActionVersion::ActionVersion( ) : Action( ACTION_CORE_VERSION, L"")
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActionVersion::~ActionVersion()
{
}

/**
 * Run the command, we take into account the current selection and command parameters given.
 * @param const MYODD_STRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* ActionVersion::CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  get the last forground window handle
  HWND hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : NULL;

  //  display the version.
  return new ActiveUserMessageAction(*this, hTopHWnd, UWM_KEYBOARD_VERSION);
}