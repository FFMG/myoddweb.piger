// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionLoad.h"
#include "ActionsCore.h"

/**
 * todo
 * @param void
 * @return void
 */
ActionLoad::ActionLoad( ) : Action( ACTION_CORE_LOAD, NULL )
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
 * @param const STD_TSTRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* ActionLoad::CreateActiveAction(const STD_TSTRING& szCommandLine, bool isPrivileged) const
{
  //  ask to reload the actions.
  PostMessage(AfxGetMainWnd()->GetSafeHwnd(), UWM_KEYBOARD_RELOAD, 0, 0);
  return NULL;
}