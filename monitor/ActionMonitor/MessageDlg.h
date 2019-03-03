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
  explicit MessageDlg();   // standard constructor
	virtual ~MessageDlg();

// Dialog Data
	enum { IDD = IDD_ACTIONMONITOR_DIALOG };

public:
  void Create( const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage);

protected:
  virtual HGDIOBJ SelDisplayFont( HDC hdc, UINT fontSize = 70 );

  myodd::threads::Workers _worker;
  static void Fade(MessageDlg* owner );
  
protected:
  long _totalMilisecondsToShowMessage;           //  how fast the text will fade out
  std::wstring _mStdMessage;                    //  the message
  long _elapseMiliSecondsBeforeFadeOut;    //  how long before we fade out.

  void DoFade() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();

protected:
  void InitWindowPos();

  void CloseFromThread();

  std::function<void(CWnd*)> _onComplete;

  void FadeCloseWindow();

  unsigned char GetStartTransparency() const;

  void ShowMessageWithNoFadding( long milliseconds ) const;

public:
  BOOL OnInitDialog() override;

  void Show( std::function<void(CWnd*)> onComplete );
  
  static void Close(MessageDlg* owner);

  bool IsRunning() const;
};
