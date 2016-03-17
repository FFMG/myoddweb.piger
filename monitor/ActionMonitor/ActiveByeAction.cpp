#include "stdafx.h"
#include "ActiveByeAction.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"

ActiveByeAction::ActiveByeAction(const Action& src) :
  ActiveAction( src, L"", false  )
{
  Initialize();
}

ActiveByeAction::~ActiveByeAction()
{
}


void ActiveByeAction::ExecuteInThread()
{
  CActionMonitorDlg* pThis = (CActionMonitorDlg*)App().GetMainWnd();
  pThis->KillAllActiveWindows();

  // let the end actions proceed.
  // we are creating a thread within a thread.
  App().DoEndActionsList();

  //  close use
  CWnd* pWnd = App().GetMainWnd();
  if (pWnd)
  {
    pWnd->PostMessage(WM_CLOSE, 0, 0);
  }
}
