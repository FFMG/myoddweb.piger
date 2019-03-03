#pragma once
#include "IMessagesHandler.h"
#include "CommonWnd.h"

class MessagesHandlerWnd final : public CommonWnd
{
public:
  MessagesHandlerWnd();
  ~MessagesHandlerWnd();

  bool Show(IMessagesHandler& parent, const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const;
  bool WaitForAllToComplete(IMessagesHandler& parent) const;

protected:
  class Msg final
  {
  public:
    Msg(IMessagesHandler& parent, const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) :
      _parent(parent),
      _text(sText),
      _elapse(elapseMiliSecondsBeforeFadeOut),
      _fadeOut(totalMilisecondsToShowMessage)
    {
    }

    ~Msg() = default;
    const wchar_t* Text() const { return _text.c_str(); }
    int Elapse() const { return _elapse; }
    int FadeOut() const { return _fadeOut; }
    IMessagesHandler& Parent() const { return _parent; }

  protected:
    const std::wstring _text;
    int _elapse;
    int _fadeOut;
    IMessagesHandler& _parent;
  };

  LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
};
