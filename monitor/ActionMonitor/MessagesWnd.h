#pragma once
#include "ActionsCore.h"
#include "IMessages.h"

class MessagesWnd final
{
public:
  MessagesWnd();
  virtual ~MessagesWnd();

  bool Close();
  bool Create();
  bool Show(IMessages& parent, const std::wstring& wsText, int nElapse, int nFadeOut);

protected:
  const std::wstring _szClassName;
  WNDCLASSEX _wc;
  HWND _hwnd;
  MSG _Msg;

  class Msg
  {
  public:
    Msg(IMessages& parent, const std::wstring& wsText, int elapse, int fadeOut) :
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
    IMessages& Parent() const { return _parent; }

  protected:
    const std::wstring _text;
    int _elapse;
    int _fadeOut;
    IMessages& _parent;
  };

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
