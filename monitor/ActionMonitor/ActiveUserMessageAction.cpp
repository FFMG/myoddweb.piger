#include "stdafx.h"
#include "ActiveUserMessageAction.h"
#include "ActionMonitor.h"

ActiveUserMessageAction::ActiveUserMessageAction(const Action& src, HWND hTopHWnd, UINT Msg)
  : ActiveAction( src, hTopHWnd, L"", false ),
  _Msg( Msg )
{
}


ActiveUserMessageAction::~ActiveUserMessageAction()
{
}

bool ActiveUserMessageAction::OnInitialize()
{
  // do nothing
  return true;
}

bool ActiveUserMessageAction::OnDeInitialize()
{
  // do nothing
  return true;
}

void ActiveUserMessageAction::OnExecuteInThread()
{
  //  ask to reload the actions.
  CWnd* pWnd = App().GetMainWnd();
  if (pWnd)
  {
    pWnd->PostMessage(_Msg, 0, 0);
  }
}