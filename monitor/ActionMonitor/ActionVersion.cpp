// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionVersion.h"
#include "ActionsCore.h"

/**
 * todo
 * @param void
 * @return void
 */
ActionVersion::ActionVersion( ) : Action( ACTION_CORE_VERSION, NULL )
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
 * @param const STD_TSTRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* ActionVersion::CreateActiveAction(const STD_TSTRING& szCommandLine, bool isPrivileged) const
{
  //  display the version.
  PostMessage(AfxGetMainWnd()->GetSafeHwnd(), UWM_KEYBOARD_VERSION, 0, 0);
  return NULL;
}