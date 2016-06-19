// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionBye.h"
#include "ActionsCore.h"
#include "ActiveByeAction.h"

/**
 * todo
 * @param void
 * @return void
 */
ActionBye::ActionBye( ) : Action( ACTION_CORE_BYE, NULL )
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActionBye::~ActionBye()
{
}

/**
 * Run the command, we take into account the current selection and command parameters given.
 * @param HWND hTopWindow the last foreground window
 * @return BOOL true.
 */
ActiveAction* ActionBye::CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  get the last forground window handle
  HWND hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : NULL;
  return new ActiveByeAction( *this, hTopHWnd );
}