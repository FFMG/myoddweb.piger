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
#include "ActiveActions.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// ActionMonitorDlg dialog
#define ACTION_NONE           0x000
#define ACTION_MAINKEY_DOWN   0x001
#define ACTION_SHIFT_DOWN     0x002
#define ACTION_LSHIFT_DOWN    0x004 /* if not set then RSHIFT is down */

#define SPECIAL_KEY VK_CAPITAL

#include "../common/trayDialog.h" //  system tray icon item
#include <os/ipclistener.h>
#include "IMessages.h"

class ActionMonitorDlg final : public CTrayDialog, FadeWnd, ActiveActions
{
// Construction
public:
  explicit ActionMonitorDlg( IMessages& messages, CWnd* pParent = nullptr);	// standard constructor
  virtual ~ActionMonitorDlg();

	enum { IDD = IDD_ACTIONMONITOR_DIALOG };

	// ClassWizard generated virtual function overrides
	protected:
	void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

protected:
  //  the command window functions
  RECT m_rWindow;                         //  the current position of the window
  void ShowWindow( BYTE bTrans );         //  show the window, (0 == hide)
  void InitWindow( );                     //  set up the window for the first time

protected:
  //  set up the hook
  void InitHook();

protected:
  //  check if the param is our special key
  static bool IsSpecialKey( WPARAM wparam );
  BOOL IsSpecialKeyDown( )const;

protected:
  //  the state of our special key
  DWORD m_keyState;
protected:

  static void MessagePump(HWND hWnd);

public:

  // kill all the active windows.
  void KillAllActiveWindows();

  // before we close the dlg we must wait for our last messages
  void WaitForActiveWindows();

  //  add a message handler
  void AddMessageHandler(myodd::os::IpcMessageHandler& handler);

  // remove a message handler.
  void RemoveMessageHandler(myodd::os::IpcMessageHandler& handler);

protected:
  /**
   * \brief the messages handler
   */
  IMessages& _messages;

  bool DisplayCommand( HDC hdc = NULL );
  void DisplayTime( HDC hdc, RECT &rParent );
  bool ResizeCommandWindow( const RECT &newSize );
  
  CFont *_fontTime;    //  the time font

  HGDIOBJ SelTimeFont( HDC hdc );

protected:
  //  ---------------------------------------------------------------------------------
  //  thwe max width/hewight
  POINT m_ptMaxValues;
  void CalcMaxes();

  afx_msg LRESULT OnHookKeyChar     (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHookKeyDown     (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHookKeyUp       (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReload          (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnVersion         (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMessagePumpReady(WPARAM wParam, LPARAM lParam);
  afx_msg void OnWindowPosChanging  (WINDOWPOS FAR* lpwndpos);

  // Generated message map functions
	//{{AFX_MSG(ActionMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTrayExit();
  afx_msg void OnTrayVersion();
	afx_msg void OnTrayReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//  virtual void PostNcDestroy();
public:
  afx_msg void OnClose();

  static bool IsRunning();

  bool DisplayMessage(const std::wstring& wsText, const int nElapse, const int nFadeOut);
public:
  afx_msg void OnDestroy();

protected:
  std::mutex _mutex;

  void InitializeListener();
  myodd::os::IpcListener* _IpcListener;
};
