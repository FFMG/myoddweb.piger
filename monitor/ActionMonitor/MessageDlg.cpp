#include "stdafx.h"

#include "ActionMonitor.h"
#include "MessageDlg.h"

IMPLEMENT_DYNAMIC(MessageDlg, CDialog)

/**
 * Todo
 * @see CDialog::CDialog
 * @param void
 * @return void
 */
MessageDlg::MessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MessageDlg::IDD, pParent),
  FadeWnd( ),
  m_timerId( 0 ),
  m_nElapse( 0 )
{

}

/**
 * Todo
 * @param void
 * @return void
 */
MessageDlg::~MessageDlg()
{
}

/**
 * Todo
 * @see CDialog::DoDataExchange
 * @param void
 * @return void
 */
void MessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MessageDlg, CDialog)
  ON_WM_TIMER()
  ON_WM_PAINT()
END_MESSAGE_MAP()

/**
 * Todo
 * @see CDialog::PostNcDestroy
 * @param void
 * @return void
 */
void MessageDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  delete this;
}

/**
 * Todo
 * @see CDialog::Create
 * @param void
 * @param void
 * @param void
 * @return void
 */
void MessageDlg::Create( LPCTSTR pText, UINT nElapse, UINT nFadeOut )
{
  //  save the value
  m_nFadeOut = nFadeOut;
  m_stdMessage = pText;
  m_nElapse = nElapse;

  __super::Create( MessageDlg::IDD );
}

/**
 * Todo
 * @param void
 * @return void
 */
void MessageDlg::InitWindowPos()
{
  //  get the size of the windows
  HDC hdc = ::GetDC( m_hWnd );

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  HGDIOBJ pOldFont = SelDisplayFont( hdc );

  RECT r = {0,0,0,0};
  myodd::html::html(hdc, m_stdMessage.c_str() , m_stdMessage.length(), &r, DT_DEFAULT | DT_CALCRECT );

  //  clean up old fonts
  if ( pOldFont != NULL )
  {
    SelectObject( hdc, pOldFont );
  }

  ::ReleaseDC( NULL, hdc );

  int width   = GetSystemMetrics( SM_CXFULLSCREEN ) / 2;
  int height  = GetSystemMetrics( SM_CYFULLSCREEN ) / 2;

  int widthM   = abs( r.right - r.left);
  int heightM  = abs( r.bottom - r.top);

  //  pad a little
  widthM  += 2 * (int)myodd::config::get( _T("commands\\pad.x"), 2);
  heightM += 2 * (int)myodd::config::get( _T("commands\\pad.y"), 2);


  //  move the window to the top left hand corner 
  SetWindowPos( &wndTopMost, 
                (width - widthM/2), 
                (height - heightM/2), 
                (widthM), 
                (heightM), 
                SWP_HIDEWINDOW );
}

/**
 * Todo
 * @param void
 * @return void
 */
HGDIOBJ MessageDlg::SelDisplayFont( HDC hdc, UINT fontSize )
{
  // TODO replace magic number.
  return __super::SelDisplayFont( hdc, 35 );
}

/**
 * Todo
 * @see CDialog::OnTimer
 * @param void
 * @return void
 */
void MessageDlg::OnTimer(UINT_PTR nIDEvent)
{
  CDialog::OnTimer(nIDEvent);
  if( nIDEvent == m_timerId )
  {
    // kill the timer
    KillTimer( nIDEvent );

    // make it thread safe
    Fade();

    //  if this is the timer event then we can close the window.
    FadeKillWindow();
  }
}

void MessageDlg::Fade()
{
  // 0 means that we move right away
  if (m_nFadeOut > 0)
  {
    BYTE bStart = myodd::config::get(_T("commands\\transparency"), 127);
    for (BYTE b = bStart; b > 0; --b)
    {
      Transparent(b);
      DWORD d = (GetTickCount() + m_nFadeOut);
      while (d > GetTickCount())
      {
        MessagePump(m_hWnd);
        Sleep(0);
        MessagePump(NULL);
      }
    }
  }
}

/**
 * Todo
 * @see CDialog::OnPaint
 * @param void
 * @return void
 */
void MessageDlg::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  HDC hdc = dc.GetSafeHdc();
  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  HGDIOBJ pOldFont = SelDisplayFont( hdc );

  RECT r = {0,0,0,0};
  myodd::html::html(hdc, m_stdMessage.c_str() , m_stdMessage.length(), &r, DT_DEFAULT | DT_CALCRECT );

  //  pad a little
  r.left += (int)myodd::config::get( _T("commands\\pad.x"), 2);
  r.top  += (int)myodd::config::get( _T("commands\\pad.y"), 2);

  SetBkMode( hdc, TRANSPARENT );
  myodd::html::html(hdc, m_stdMessage.c_str() , m_stdMessage.length(), &r, DT_DEFAULT );

  //  clean up old fonts
  if ( pOldFont != NULL )
  {
    SelectObject( hdc, pOldFont );
  }
}

/**
 * Todo
 * @see CDialog::OnInitDialog
 * @param void
 * @return void
 */
BOOL MessageDlg::OnInitDialog()
{
  __super::OnInitDialog();

  SetFadeParent( m_hWnd );

  ModifyStyleEx(WS_EX_APPWINDOW,0); //  no taskbar!

  // move the window at the right place/size
  InitWindowPos();

  // fade the window a little.
  ShowWindow( SW_HIDE );

  return TRUE;
}

/**
 * Todo
 * @param void
 * @return void
 */
void MessageDlg::FadeShowWindow()
{
  // fade the window a little.
  ShowWindow(SW_SHOW);
  Transparent( myodd::config::get( _T("commands\\transparency"), 127) );

  m_timerId = SetTimer( WM_USER+1, m_nElapse, NULL );
}

/**
 * Todo
 * @param void
 * @return void
 */
void MessageDlg::FadeKillWindow()
{
  //  if this is the timer event then we can close the window.
  DestroyWindow();
}