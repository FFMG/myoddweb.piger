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
  class Msg {
  public:
    Msg(LPCTSTR pText, UINT nElapse, UINT nFadeOut) {
      _pText = new STD_TSTRING(pText);
      _nElapse = nElapse;
      _nFadeOut = nFadeOut;
    }
    virtual ~Msg() {
      delete _pText;
    }
    LPCTSTR Text() const { return _pText->c_str(); }
    UINT Elapse() const { return _nElapse; }
    UINT FadeOut() const { return _nFadeOut; }

  protected:
    STD_TSTRING* _pText;
    UINT _nElapse;
    UINT _nFadeOut;
  };

  void Create( LPCTSTR pText, UINT nElapse, UINT nFadeOut);

protected:
  virtual HGDIOBJ SelDisplayFont( HDC hdc, UINT fontSize = 70 );

protected:
  UINT m_nFadeOut;          //  how fast the text will fade out
  STD_TSTRING m_stdMessage; //  the message
  UINT m_nElapse;           //  how long before we fade out.

  void Fade();

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
