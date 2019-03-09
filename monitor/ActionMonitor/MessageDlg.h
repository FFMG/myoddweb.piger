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
#include "threads/workers.h"
#include "CommonWnd.h"

// MessageDlg dialog

class MessageDlg final : public CommonWnd, FadeWnd
{
public:
  explicit MessageDlg();   // standard constructor
	virtual ~MessageDlg();

  void Create( const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage);

protected:

  myodd::threads::Workers _worker;
  static void Fade(MessageDlg* owner );
  
  long _totalMilisecondsToShowMessage;      //  how fast the text will fade out
  std::wstring _mStdMessage;                //  the message
  long _elapseMiliSecondsBeforeFadeOut;     //  how long before we fade out.

  void DoFade() const;

public:
  void OnPaint() override;

protected:
  /**
   * \brief this is the full message window area
   */
  RECT _completeRect;

  void RedrawBackground(HDC hdc) const;
  void RedrawMessage(HDC hdc);

  void InitWindowPos();

  void CloseFromThread();

  void FadeCloseWindow();

  static unsigned char GetStartTransparency();

  void ShowMessageWithNoFadding( long milliseconds ) const;

public:
  bool OnInitDialog() override;

  void Show();
  
  static void Close(MessageDlg* owner);

  bool IsRunning() const;
};
