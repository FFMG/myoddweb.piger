#include "stdafx.h"
#include "MessagesHandlerWnd.h"
#include "ActionsCore.h"

MessagesHandlerWnd::MessagesHandlerWnd() :
  CommonWnd(L"ActionMonitorMessagesWindowClass")
{
}

MessagesHandlerWnd::~MessagesHandlerWnd()
{
}

LRESULT MessagesHandlerWnd::OnMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  if(UWM_WAITFORALLMESSAGES == msg )
  {
    const auto messageHander = reinterpret_cast<IMessagesHandler*>(lParam);
    messageHander->WaitForAllToComplete();
    return 1L;
  }
  if (UWM_DISPLAYMESSAGE == msg)
  {
    const auto messageHanderData = reinterpret_cast<Msg*>(lParam);
    const auto result = messageHanderData->Parent().Show(messageHanderData->Text(), messageHanderData->Elapse(), messageHanderData->FadeOut());
    delete messageHanderData;
    
    return result ? 1L : 0L;
  }
  return CommonWnd::OnMessage( msg, wParam, lParam);
}

bool MessagesHandlerWnd::WaitForAllToComplete(IMessagesHandler& parent) const
{
  assert(::IsWindow(_hwnd));
  return ERROR_SUCCESS == SendMessage(_hwnd, UWM_WAITFORALLMESSAGES, 0, reinterpret_cast<LPARAM>( &parent));
}


bool MessagesHandlerWnd::Show(IMessagesHandler& parent, const std::wstring& sText, const long elapseMiliSecondsBeforeFadeOut, const long totalMilisecondsToShowMessage) const
{
  assert(::IsWindow(_hwnd));
  return ERROR_SUCCESS == SendMessage(_hwnd, UWM_DISPLAYMESSAGE, 0, reinterpret_cast<LPARAM>(new Msg(parent, sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage)));
}

