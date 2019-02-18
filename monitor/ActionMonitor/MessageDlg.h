//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once
#include "FadeWnd.h"
#include "resource.h"		// main symbols
#include "threads/workers.h"

// MessageDlg dialog

class MessageDlg final : public CDialog, FadeWnd
{
	DECLARE_DYNAMIC(MessageDlg)

public:
  explicit MessageDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MessageDlg() = default;

// Dialog Data
	enum { IDD = IDD_ACTIONMONITOR_DIALOG };

public:
  class Msg 
  {
  public:
    Msg(const std::wstring& wsText, int nElapse, int nFadeOut) :
      _pText( wsText),
      _nElapse( nElapse ),
      _nFadeOut(  nFadeOut )
    {
    }
    virtual ~Msg() = default;
    const wchar_t* Text() const { return _pText.c_str(); }
    int Elapse() const { return _nElapse; }
    int FadeOut() const { return _nFadeOut; }

  protected:
    const std::wstring _pText;
    int _nElapse;
    int _nFadeOut;
  };

  void Create( const std::wstring& sText, int nElapse, int nFadeOut);

protected:
  virtual HGDIOBJ SelDisplayFont( HDC hdc, UINT fontSize = 70 );

  myodd::threads::Workers _worker;
  static void Fade(MessageDlg* owner );

protected:
  int _mNFadeOut;          //  how fast the text will fade out
  std::wstring _mStdMessage; //  the message
  int _mNElapse;           //  how long before we fade out.

  void DoFade();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  afx_msg void OnPaint();
  afx_msg void OnClose();

protected:
  void InitWindowPos();

  void CloseFromThread();

  std::function<void(CWnd*)> _onComplete;

public:
  BOOL OnInitDialog() override;

  void FadeShowWindow( std::function<void(CWnd*)> onComplete );
  void FadeKillWindow();
  bool IsRunning() const;
};
