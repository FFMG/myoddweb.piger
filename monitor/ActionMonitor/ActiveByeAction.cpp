#include "stdafx.h"
#include "ActiveByeAction.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"

/**
 * Constructor
 * @param const Action& src the parent action
 * @param HWND hTopHWnd the top window at the time of the call.
 */
ActiveByeAction::ActiveByeAction(const Action& src, HWND hTopHWnd ) :
  ActiveAction( src, hTopHWnd, L"", false  )
{
  Initialize();
}

ActiveByeAction::~ActiveByeAction()
{
}

/**
 * Execute the action in thread.
 * Call to close this app.
 */
void ActiveByeAction::ExecuteInThread()
{
  // close all the active windows
  // we no longer need them.
  CActionMonitorDlg* pThis = (CActionMonitorDlg*)App().GetMainWnd();
  pThis->KillAllActiveWindows();

  // let the end actions proceed.
  // we are creating a thread within a thread.
  App().DoEndActionsList();

  //  close us
  CWnd* pWnd = App().GetMainWnd();
  if (pWnd)
  {
    pWnd->PostMessage(WM_CLOSE, 0, 0);
  }
}
