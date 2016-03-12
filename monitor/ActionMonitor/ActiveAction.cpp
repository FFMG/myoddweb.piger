// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActiveAction.h"
#include "ActionsCore.h"
#include "ActionMonitor.h"

/**
 * todo
 * @param void
 * @return void
 */
ActiveAction::ActiveAction() : Action(), _clipboard(NULL)
{
  CreateClipboard();
}

/**
* todo
* @param void
* @return void
*/
ActiveAction::ActiveAction(const Action& src ) : Action( src ), _clipboard(NULL)
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActiveAction::~ActiveAction()
{
  delete _clipboard;
}

void ActiveAction::ClearClipboard()
{
  delete _clipboard;
  _clipboard = NULL;
}

void ActiveAction::CreateClipboard()
{
  //  clear the old one.
  ClearClipboard();

  // we must always get the clipboard because we never know for certain if the APIs might call
  // some functions. This will also reset the values if need be.
  //
  // so copy the text that the user could have currently selected or copy the name of the file that is probably selected
  // tell the clipboard to copy the data of the last known foreground window.
  CWnd* cwnd = CActionMonitorApp::GetLastForegroundWindow();
  _clipboard = new Clipboard(cwnd );
}