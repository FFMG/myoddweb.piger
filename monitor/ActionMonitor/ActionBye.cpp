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
 * @param const STD_TSTRING& szCommandLine the command line argument.
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return BOOL true.
 */
ActiveAction* ActionBye::CreateActiveAction(const STD_TSTRING& szCommandLine, bool isPrivileged) const
{
  return new ActiveByeAction( *this );
}