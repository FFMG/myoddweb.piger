#pragma once
#include "ActionsCore.h"
#include "IMessagesHandler.h"

class MessagesHandlerWnd final
{
public:
  MessagesHandlerWnd();
  virtual ~MessagesHandlerWnd();

  bool Close();
  bool Create();
  bool Show(IMessagesHandler& parent, const std::wstring& wsText, int nElapse, int nFadeOut);

protected:
  const std::wstring _szClassName;
  WNDCLASSEX _wc;
  HWND _hwnd;
  MSG _Msg;

  class Msg
  {
  public:
    Msg(IMessagesHandler& parent, const std::wstring& wsText, int elapse, int fadeOut) :
      _parent(parent),
      _text(wsText),
      _elapse(elapse),
      _fadeOut(fadeOut)
    {
    }
    virtual ~Msg() = default;
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
