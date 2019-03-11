#include "stdafx.h"
#include "ActiveByeAction.h"
#include "ActionMonitor.h"

/**
 * \copydoc
 */
ActiveByeAction::ActiveByeAction(IApplication& application, const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd ) :
  ActiveAction( src, virtualMachines, hTopHWnd, L"", false  ),
  _application( application )
{
}

ActiveByeAction::~ActiveByeAction()
{
}

bool ActiveByeAction::OnInitialize()
{
  // do nothing
  return true;
}

bool ActiveByeAction::OnDeInitialize()
{
  // do nothing
  return true;
}

/**
 * Execute the action in thread.
 * Call to close this app.
 */
void ActiveByeAction::OnExecuteInThread()
{
  _application.Close();
}
