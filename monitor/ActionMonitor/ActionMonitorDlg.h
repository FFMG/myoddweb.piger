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
#include "../common/trayDialog.h" //  system tray icon item
#include "IApplication.h"
#include "IDisplay.h"

class ActionMonitorDlg final : public CTrayDialog, FadeWnd, public IDisplay
{
// Construction
public:
  explicit ActionMonitorDlg(
    IApplication& application,
    CWnd* pParent);
  virtual ~ActionMonitorDlg();

  void Show(const std::wstring& sCommand) override;
  void Hide() override;
  void Active() override;
  void Inactive() override;

	enum { IDD = IDD_ACTIONMONITOR_DIALOG };

	// ClassWizard generated virtual function overrides
protected:
	void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support

	HICON m_hIcon;

  //  the command window functions
  RECT m_rWindow;                         //  the current position of the window

  void ShowWindow(const std::wstring& sCommand, BYTE bTrans );         //  show the window, (0 == hide)
  void InitWindow( );                     //  set up the window for the first time

  bool DisplayCommand( const std::wstring& sCommand, HDC hdc);
  void DisplayTime( HDC hdc, RECT &rParent );
  bool ResizeCommandWindow( const RECT &newSize );
  
  CFont *_fontTime;    //  the time font

  HGDIOBJ SelTimeFont( HDC hdc );

protected:
  /**
   * \brief the current command
   */
  std::wstring _sCommand;

  /**
   * \brief the applications controller.
   */
  IApplication& _application;

  //  ---------------------------------------------------------------------------------
  //  thwe max width/hewight
  POINT m_ptMaxValues;
  void CalcMaxes();

  afx_msg LRESULT OnMessagePumpReady(WPARAM wParam, LPARAM lParam);
  afx_msg void OnWindowPosChanging  (WINDOWPOS FAR* lpwndpos);
  afx_msg void OnPaint();
  afx_msg void OnTrayExit();
  afx_msg void OnTrayVersion();
  afx_msg void OnTrayReload();
  virtual BOOL OnInitDialog();
  afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
  std::mutex _mutex;
};
