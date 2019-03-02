// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionBye.h"
#include "ActionsCore.h"
#include "ActiveByeAction.h"

/**
 * Contructor
 * \param application the application to close the app
 */
ActionBye::ActionBye(IApplication& application ) : 
  Action( ACTION_CORE_BYE, L"" ),
  _application(application)
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
 * \copydoc
 */
ActiveAction* ActionBye::CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  get the last forground window handle
  const auto hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
  return new ActiveByeAction( _application, *this, virtualMachines, hTopHWnd );
}