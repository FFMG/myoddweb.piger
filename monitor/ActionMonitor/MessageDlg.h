#pragma once
#include "FadeWnd.h"

// MessageDlg dialog

class MessageDlg : public CDialog, FadeWnd
{
	DECLARE_DYNAMIC(MessageDlg)

public:
	MessageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MessageDlg();

// Dialog Data
	enum { IDD = IDD_ACTIONMONITOR_DIALOG };

public:
  void Create( LPCTSTR pText, UINT nElapse, UINT nFadeOut);

protected:
  virtual HGDIOBJ SelDisplayFont( HDC hdc, UINT fontSize = 70 );

protected:
  UINT m_nFadeOut;          //  how fast the text will fade out
  STD_TSTRING m_stdMessage; //  the message
  UINT m_nElapse;           //  how long before we fade out.

protected:
  UINT_PTR m_timerId;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnPaint();

protected:
  void InitWindowPos();
public:
  virtual BOOL OnInitDialog();

  void FadeShowWindow();
  void FadeKillWindow();
};
