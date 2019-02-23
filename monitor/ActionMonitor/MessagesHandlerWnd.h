#pragma once
#include "IMessagesHandler.h"

class MessagesHandlerWnd final
{
public:
  MessagesHandlerWnd();
  ~MessagesHandlerWnd();

  bool Close();
  bool Create();
  bool Show(IMessagesHandler& parent, const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const;

protected:
  /**
   * \brief Create the class if it does not exist
   */
  bool CreateClass();

  const std::wstring _szClassName;
  WNDCLASSEX _wc;
  HWND _hwnd;

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

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
