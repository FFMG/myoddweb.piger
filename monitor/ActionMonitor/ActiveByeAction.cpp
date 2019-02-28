#include "stdafx.h"
#include "ActiveByeAction.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"

/**
 * Constructor
 * @param const Action& src the parent action
 * @param HWND hTopHWnd the top window at the time of the call.
 */
ActiveByeAction::ActiveByeAction(const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd ) :
  ActiveAction( src, virtualMachines, hTopHWnd, L"", false  )
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
  App().DoClose();
}
