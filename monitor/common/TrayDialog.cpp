// TrayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TrayDialog.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

/**
 * Todo
 * @see CDialog::CDialog
 * @param void
 * @param void
 * @return void
 */
CTrayDialog::CTrayDialog(UINT uIDD,CWnd* pParent /*=NULL*/)
: CDialog(uIDD, pParent),
  m_bTrayIconVisible( false )
{
  m_nidIconData.cbSize			= sizeof(NOTIFYICONDATA);
  
  m_nidIconData.hWnd				= 0;
  m_nidIconData.uID				  = 1;
  
  m_nidIconData.uCallbackMessage	= WM_TRAY_ICON_NOTIFY_MESSAGE;
  
  m_nidIconData.hIcon				= 0;
  m_nidIconData.szTip[0]		= 0;	
  m_nidIconData.uFlags			= NIF_MESSAGE;
  
  m_nDefaultMenuItem				= 0;
}


BEGIN_MESSAGE_MAP(CTrayDialog, CDialog)
//{{AFX_MSG_MAP(CTrayDialog)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SYSCOMMAND()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE , OnTrayNotify )
END_MESSAGE_MAP()

/**
 * Todo
 * @see CDialog::OnCreate
 * @param void
 * @return void
 */
int CTrayDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  m_nidIconData.hWnd = this->m_hWnd;
  m_nidIconData.uID = 1;
  
  return 0;
}

/**
 * Todo
 * @see CDialog::OnDestroy
 * @return void
 */
void CTrayDialog::OnDestroy() 
{
  CDialog::OnDestroy();
  if(m_nidIconData.hWnd && m_nidIconData.uID>0 && TrayIsVisible())
  {
    Shell_NotifyIcon(NIM_DELETE,&m_nidIconData);
  }
}

/**
 * Todo
 * @see CDialog::CDialog
 * @return bool 
 */
bool CTrayDialog::TrayIsVisible() const
{
  return m_bTrayIconVisible;
}

/**
 * Todo
 * @param void
 */
void CTrayDialog::TraySetIcon(HICON hIcon)
{
  ASSERT(hIcon);
  
  m_nidIconData.hIcon = hIcon;
  m_nidIconData.uFlags |= NIF_ICON;
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::TraySetIcon(UINT nResourceID)
{
  ASSERT(nResourceID>0);
  HICON hIcon = 0;
  hIcon = AfxGetApp()->LoadIcon(nResourceID);
  if(hIcon)
  {
    m_nidIconData.hIcon = hIcon;
    m_nidIconData.uFlags |= NIF_ICON;
    
  }
  else
  {
    TRACE0("FAILED TO LOAD ICON\n");
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::TraySetIcon(LPCTSTR lpszResourceName)
{
  HICON hIcon = 0;
  hIcon = AfxGetApp()->LoadIcon(lpszResourceName);
  if(hIcon)
  {
    m_nidIconData.hIcon = hIcon;
    m_nidIconData.uFlags |= NIF_ICON;
  }
  else
  {
    TRACE0("FAILED TO LOAD ICON\n");
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::TraySetToolTip(LPCTSTR lpszToolTip)
{
  ASSERT(_tcslen(lpszToolTip) > 0 && _tcslen(lpszToolTip) < 64);
  
  _tcscpy_s(m_nidIconData.szTip,lpszToolTip);
  m_nidIconData.uFlags |= NIF_TIP;
}

/**
 * Todo
 * @return void
 */
BOOL CTrayDialog::TrayShow()
{
  BOOL bSuccess = FALSE;
  if(!m_bTrayIconVisible)
  {
    bSuccess = Shell_NotifyIcon(NIM_ADD,&m_nidIconData);
    if(bSuccess)
    {
      m_bTrayIconVisible= true;
    }
  }
  else
  {
    TRACE0("ICON ALREADY VISIBLE");
  }
  return bSuccess;
}

/**
 * Todo
 * @param void
 * @return void
 */
BOOL CTrayDialog::TrayHide()
{
  BOOL bSuccess = FALSE;
  if(m_bTrayIconVisible)
  {
    bSuccess = Shell_NotifyIcon(NIM_DELETE,&m_nidIconData);
    if(bSuccess)
    {
      m_bTrayIconVisible= false;
    }
  }
  else
  {
    TRACE0("ICON ALREADY HIDDEN");
  }
  return bSuccess;
}

/**
 * Todo
 * @param void
 * @return void
 */
BOOL CTrayDialog::TrayUpdate()
{
  BOOL bSuccess = FALSE;
  if(m_bTrayIconVisible)
  {
    bSuccess = Shell_NotifyIcon(NIM_MODIFY,&m_nidIconData);
  }
  else
  {
    TRACE0("ICON NOT VISIBLE");
  }
  return bSuccess;
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
BOOL CTrayDialog::TraySetMenu(UINT nResourceID,UINT nDefaultPos)
{
  BOOL bSuccess;
  bSuccess = m_mnuTrayMenu.LoadMenu(nResourceID);
  return bSuccess;
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
BOOL CTrayDialog::TraySetMenu(LPCTSTR lpszMenuName,UINT nDefaultPos)
{
  BOOL bSuccess;
  bSuccess = m_mnuTrayMenu.LoadMenu(lpszMenuName);
  return bSuccess;
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
BOOL CTrayDialog::TraySetMenu(HMENU hMenu,UINT nDefaultPos)
{
  m_mnuTrayMenu.Attach(hMenu);
  return TRUE;
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
LRESULT CTrayDialog::OnTrayNotify(WPARAM wParam, LPARAM lParam) 
{ 
  UINT uID; 
  UINT uMsg; 
  
  uID = (UINT) wParam; 
  uMsg = (UINT) lParam; 
  
  if (uID != 1)
    return 0L;
 
  CPoint pt;	
  
  switch (uMsg ) 
  { 
  case WM_MOUSEMOVE:
    GetCursorPos(&pt);
    ClientToScreen(&pt);
    OnTrayMouseMove(pt);
    break;

  case WM_LBUTTONDOWN:
    GetCursorPos(&pt);
    ClientToScreen(&pt);
    OnTrayLButtonDown(pt);
    break;

  case WM_LBUTTONDBLCLK:
    GetCursorPos(&pt);
    ClientToScreen(&pt);
    OnTrayLButtonDblClk(pt);
    break;
    
  case WM_RBUTTONDOWN:
  case WM_CONTEXTMENU:
    GetCursorPos(&pt);
    //ClientToScreen(&pt);
    OnTrayRButtonDown(pt);
    break;
    
  case WM_RBUTTONDBLCLK:
    GetCursorPos(&pt);
    ClientToScreen(&pt);
    OnTrayRButtonDblClk(pt);
    break;

  default:
    break;
  } 
  return 1; 
} 

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::OnTrayRButtonDown(CPoint pt)
{
  ShowMenu(pt);
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::OnTrayLButtonDown(CPoint pt)
{
  ShowMenu(pt);
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::OnTrayLButtonDblClk(CPoint pt)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::OnTrayRButtonDblClk(CPoint pt)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::OnTrayMouseMove(CPoint pt)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
void CTrayDialog::ShowMenu(CPoint pt)
{
  if( m_mnuTrayMenu.m_hMenu )
  {
    CMenu* menu = m_mnuTrayMenu.GetSubMenu(0);
    menu->SetDefaultItem(m_nDefaultMenuItem,TRUE);

    SetForegroundWindow();
    menu->TrackPopupMenu(TPM_RIGHTBUTTON,pt.x,pt.y,this);
    PostMessage( WM_NULL, 0, 0 );
  }
}