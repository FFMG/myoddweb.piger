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
#include "IDisplay.h"
#include "ITray.h"
#include "CommonWnd.h"

class ActionMonitorDlg final : public CommonWnd, public IDisplay, FadeWnd
{
// Construction
public:
  explicit ActionMonitorDlg( ITray& tray);
  virtual ~ActionMonitorDlg();

  void Show(const std::wstring& sCommand) override;
  void Hide() override;
  void Active() override;
  void Inactive() override;

	// ClassWizard generated virtual function overrides
protected:
  void ShowWindow(const std::wstring& sCommand, BYTE bTrans );         //  show the window, (0 == hide)

  CFont *_fontTime;    //  the time font

  HGDIOBJ SelTimeFont( HDC hdc );

  void RedrawTime(HDC hdc );
  void RedrawBackground(HDC hdc) const;
  void RedrawText(const std::wstring& sCommand, HDC hdc);
  void RepositionWindow() const;

  RECT _mainWindowPosition;
  RECT _commandRectangle;
  void RecalculateAllRectangles( const std::wstring& sCommand);
  RECT CalculateMinimumRectPosition(const std::wstring& sCommand, HDC hdc );
  RECT CalculateCommandRectangle(const std::wstring& sCommand, HDC hdc);

protected:
  /**
   * \brief the current command
   */
  std::wstring _sCommand;

  /**
   * \brief the system tray
   */
  ITray& _tray;

  void OnPaint() override;
  bool OnInitDialog() override;

protected:
  std::mutex _mutex;
};
