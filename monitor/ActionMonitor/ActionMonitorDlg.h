// ActionMonitorDlg.h : header file
//
#pragma once

#include "FadeWnd.h"
#include "ActiveActions.h"
#include "MessageDlg.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CActionMonitorDlg dialog
#define ACTION_NONE           0x000
#define ACTION_MAINKEY_DOWN   0x001
#define ACTION_SHIFT_DOWN     0x002
#define ACTION_LSHIFT_DOWN    0x004 /* if not set then RSHIFT is down */


#define SPECIAL_KEY VK_CAPITAL

#include "../common/trayDialog.h" //  system tray icon item
#include <os/ipclistener.h>

class CActionMonitorDlg : public CTrayDialog, FadeWnd, ActiveActions
{
// Construction
public:
	CActionMonitorDlg(CWnd* pParent = NULL);	// standard constructor
  virtual ~CActionMonitorDlg();

// Dialog Data
	//{{AFX_DATA(CActionMonitorDlg)
	enum { IDD = IDD_ACTIONMONITOR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionMonitorDlg)
	protected:
	void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support
	//}}AFX_VIRTUAL

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
  BOOL IsSpecialKey( WPARAM wparam )const;
  BOOL IsSpecialKeyDown( )const;

protected:
  //  the state of our special key
  DWORD m_keyState;
  
protected:
  typedef std::vector<MessageDlg*> vMessages;
  vMessages _displayWindows;

  void MessagePump(HWND hWnd);

public:
  bool DisplayMessage( LPCTSTR pText, UINT nElapse, UINT nFadeOut );

  // kill all the active windows.
  void KillAllActiveWindows();

  // before we close the dlg we must wait for our last messages
  void WaitForActiveWindows();

protected:
  bool DisplayCommand( HDC hdc = NULL );
  void DisplayTime( HDC hdc, RECT &rParent );
  bool ResizeCommandWindow( const RECT &newSize );
  
  CFont *fontTime;    //  the time font

  HGDIOBJ SelTimeFont( HDC hdc );

  // remove old/unused messages clogging up the vector
  void ClearUnusedMessages();
protected:
  //  ---------------------------------------------------------------------------------
  //  thwe max width/hewight
  POINT m_ptMaxValues;
  void CalcMaxes();

  afx_msg LRESULT OnHookKeyChar   (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHookKeyDown   (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHookKeyUp     (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReload        (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnVersion       (WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnDisplayMessage(WPARAM wParam, LPARAM lParam);
  afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

  // Generated message map functions
	//{{AFX_MSG(CActionMonitorDlg)
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

//  virtual void PostNcDestroy();
public:
  afx_msg void OnDestroy();

protected:
  std::mutex _mutex;
  std::thread::id _main_threadId;
  bool IsMainThread() const;

  myodd::os::IpcListener* _IpcListener;
};
