// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionLoad.h"
#include "ActionsCore.h"
#include "ActiveUserMessageAction.h"

/**
 * todo
 * @param void
 * @return void
 */
ActionLoad::ActionLoad( ) : Action( ACTION_CORE_LOAD, L"")
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActionLoad::~ActionLoad()
{
}

/**
 * Run the command, we take into account the current selection and command parameters given.
 * @param const MYODD_STRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* ActionLoad::CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  get the last forground window handle
  auto hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : NULL;
  return new ActiveUserMessageAction(*this, hTopHWnd, UWM_KEYBOARD_RELOAD);
}