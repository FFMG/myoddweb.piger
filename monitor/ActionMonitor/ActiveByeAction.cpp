#include "stdafx.h"
#include "ActiveByeAction.h"

/**
 * \copydoc
 */
ActiveByeAction::ActiveByeAction(IApplication& application, const Action& src, HWND hTopHWnd ) :
  ActiveAction( src, hTopHWnd, L"", false  ),
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
