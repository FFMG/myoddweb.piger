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
#include "stdafx.h"
#include "ActionsCore.h"
#include "ActionMonitorDlg.h"
#include "MessagesHandler.h"

#define RECT_MIN_H 70

/**
 * \brief the constructor
 * \see CTrayDialog::CTrayDialog
 * \param application the main application
 * \param actions the list of possible actions.
 * \param pParent the parent owner of this window
 */
ActionMonitorDlg::ActionMonitorDlg(
  IApplication& application,
  CWnd* pParent)
  : 
  CTrayDialog(ActionMonitorDlg::IDD, pParent),
  m_rWindow(),
  _fontTime(nullptr), 
  _application( application ),
  m_ptMaxValues()
{
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/**
 * \brief destructor
 */
ActionMonitorDlg::~ActionMonitorDlg()
{
  if( _fontTime )
  {
    _fontTime->DeleteObject();
    delete _fontTime;
  }
}

void ActionMonitorDlg::Show(const std::wstring& sCommand )
{
  ShowWindow(sCommand, ::myodd::config::Get(L"commands\\transparency", 127));
}

void ActionMonitorDlg::Hide()
{
  ShowWindow(L"", 0);
}

void ActionMonitorDlg::Active()
{
  TraySetIcon(IDR_MAINFRAME_ACTIVE);
  TrayUpdate();
}

void ActionMonitorDlg::Inactive()
{
  TraySetIcon(IDR_MAINFRAME);
  TrayUpdate();
}

/**
 * todo
 * @see CTrayDialog::DoDataExchange
 * @param void
 * @return void
 */
void ActionMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ActionMonitorDlg, CTrayDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_REGISTERED_MESSAGE(UWM_MESSAGE_PUMP_READY, OnMessagePumpReady )
  ON_WM_WINDOWPOSCHANGING()
  ON_COMMAND(ID_TRAY_EXIT, OnTrayExit)
	ON_COMMAND(ID_TRAY_RELOAD, OnTrayReload)
  ON_COMMAND(ID_TRAY_VERSION, OnTrayVersion)
  ON_WM_DESTROY()
END_MESSAGE_MAP()

/**
 * todo
 * @see CTrayDialog::OnWindowPosChanging
 * @param void
 * @return void
 */
void ActionMonitorDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
  if( 0 == IsVisible() )
  {
    lpwndpos->flags &= ~SWP_SHOWWINDOW;
  }
  
  CTrayDialog::OnWindowPosChanging(lpwndpos);
}

/**
 * todo
 * @see CTrayDialog::OnInitDialog
 * @param void
 * @return void
 */
BOOL ActionMonitorDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();
  ModifyStyleEx(WS_EX_APPWINDOW,0); //  no taskbar!

  // Set the icon for this dialog.  The framework does this automatically
	//  when the applications main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	TraySetIcon(IDR_MAINFRAME);

  myodd::files::Version _ver;
#ifdef _DEBUG
  auto strSay = myodd::strings::Format(_T("Action Monitor [Debug]: %d.%d.%d.%d"),
#else
  auto strSay = myodd::strings::Format(_T("Action Monitor : %d.%d.%d.%d"),
#endif
  _ver.GetFileVersionMajor(),
  _ver.GetFileVersionMinor(),
  _ver.GetFileVersionMaintenance(),
  _ver.GetFileVersionBuild());

	TraySetToolTip( strSay.c_str() );
  TraySetMenu(IDR_TRAY );
  TrayShow();

  //  set up up the command window for the first time, (hidden)
  InitWindow();

  // set the fade window
  SetFadeParent( m_hWnd );

  // tell everybody that the message pump is ready
  // only once the message pump is ready can we actually send messages
  // this is important to things like IPC Windows.
  ::PostMessage(m_hWnd, UWM_MESSAGE_PUMP_READY, 0, 0);
  
  return TRUE;
}

/**
 * todo
 * @see CTrayDialog::CTrayDialog
 * @param void
 * @return void
 */
void ActionMonitorDlg::ShowWindow(const std::wstring& sCommand, const BYTE bTrans )
{
  // save the command we want to show.
  _sCommand = sCommand;

  if( IsVisible() != bTrans )
  {
    Visible(bTrans);
    SetTransparency( bTrans );
    if( bTrans == 0 )
    {
      //  we need to recalculate the size of the command window
      //  normally when we hide the window it is because we are no longer
      //  typing anything and the text has been removed
      //
      //  so by calling a Display(...) we are forcing a command window resize.
      //  this is handy so that the next time we press the key we wont see a 'flicker'
      //  of the previous command window.
      DisplayCommand( sCommand, nullptr ); //  return value is ignored.
    }
    ::ShowWindow( m_hWnd, bTrans>0?SW_SHOW:SW_HIDE );
  }

  ::RedrawWindow(m_hWnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

/**
 * Calc the max width/height of the command window given the position the user 
 * has chosen to use.
 * The width is a percentage of the screen.
 *
 * @param none
 * @return none
 */
void ActionMonitorDlg::CalcMaxes( )
{
  const auto fX = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
  const auto fY = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

  //  calc the percentages.
  m_ptMaxValues.x = static_cast<int>(fX * static_cast<int>(::myodd::config::Get(L"commands\\max.x", 99)) / 100.f);
  m_ptMaxValues.y = static_cast<int>(fY * static_cast<int>(::myodd::config::Get(L"commands\\max.y", 99)) / 100.f);
}

/**
 * Create the main window the window is hidden by default.
 *
 * @param none
 * @return none
 */
void ActionMonitorDlg::InitWindow( )
{
  CalcMaxes();

  m_rWindow.bottom  = RECT_MIN_H;
  m_rWindow.left    = ::myodd::config::Get( L"commands\\left", 5);
  m_rWindow.top     = ::myodd::config::Get( L"commands\\top", 5);
  m_rWindow.right   = m_ptMaxValues.x;

  //  move the window to the top left hand corner 
  SetWindowPos( &wndTopMost, 
                m_rWindow.left, 
                m_rWindow.top, 
                (m_rWindow.right-m_rWindow.left),
                (m_rWindow.bottom-m_rWindow.top),
                SWP_HIDEWINDOW );
}

/**
 * If you add a minimize button to your dialog, you will need the code below
 * to draw the icon.  For MFC applications using the document/view model,
 * this is automatically done for you by the framework.
 *
 * @see CTrayDialog::OnPaint
 * @param none
 * @return none
 */
void ActionMonitorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		const auto cxIcon = GetSystemMetrics(SM_CXICON);
    const auto cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
    const auto x = (rect.Width() - cxIcon + 1) / 2;
    const auto y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
    return;
	}
  if( IsVisible() == 0 )
  {
    //  hide the window.
    ::ShowWindow( m_hWnd, SW_HIDE );
  }
  else
  {
    auto myDC = new ActionMonitorMemDC( CDC::FromHandle(dc.GetSafeHdc()) );
    auto hdc = myDC->GetSafeHdc();

    // display command will return true if the rectangle size was updated.
    // if the rectangle, (the display window), changes size then we need to 
    //  create a new ActionMonitorMemDC as it only knows of the old size and will not update
    // the new area.
    //
    // so by deleting the old memdc we tell the system to reject it and to recalculate
    // the area from the device context
    while( DisplayCommand(_sCommand, hdc ) )
    {
      //  force the output to the screen now
      delete myDC;
      myDC = new ActionMonitorMemDC( CDC::FromHandle(dc.GetSafeHdc()) );
      hdc  = myDC->GetSafeHdc();
    }
    delete myDC;
  }
  CTrayDialog::OnPaint();
}

/**
 * The system calls this to obtain the cursor to display while the user drags
 * the minimized window.
 *
 * @see CTrayDialog::OnQueryDragIcon
 * @param void
 * @return void
 */
HCURSOR ActionMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
 * \brief Display the current command that will be executing.
 * \param sCommand the command we want to display
 * \param hdc handle to the device context, if null we will be using a temp one
 * \return success or not.
 */
bool ActionMonitorDlg::DisplayCommand(const std::wstring& sCommand, const HDC hdc )
{
  // should we even be here?
  if( 0 == IsVisible() )
  {
    return false;
  }
  if(nullptr == hdc )
  {
    const auto localHdc = ::GetDC(nullptr);
    if( nullptr == localHdc )
    {
      return false;
    }
    const auto result = DisplayCommand(sCommand, localHdc);
    ::ReleaseDC(nullptr, localHdc);
    return result;
  }

  //  get the current text as well as the possible commands.
  //  we pass what ever the user entered to whatever commands are saved.
  const auto len = sCommand.length();

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont( hdc );

  //  we will use 1 rectangle.
  RECT rDraw  = {0,0,0,0};
  
  // first we use DT_CALCRECT to get their size rather than drawing them on the screen.

  //calculate the width of the string using the classic way
  if( 0 == sCommand.length() )
  {
    SIZE sizeText;
    GetTextExtentPoint32(hdc, _T("I"), 1, &sizeText);
    rDraw.bottom = sizeText.cy;
  }
  else
  {
    myodd::html::html(hdc, sCommand.c_str() , len, &rDraw, DT_DEFAULT | DT_CALCRECT );
  }
 
  // add padding to the bottom pos of the window.
  rDraw.bottom        += m_rWindow.top + ( 2* (int)::myodd::config::Get( L"commands\\pad.y", 2));
  
  //  make sure that it is within limits
  rDraw.bottom        = rDraw.bottom>m_ptMaxValues.y?m_ptMaxValues.y:rDraw.bottom;
  
  rDraw.left = m_rWindow.left;
  rDraw.top = m_rWindow.top;
  rDraw.right = m_rWindow.right;

  //  m_rWindow.top     = //  unchanged - always to of screen
  //  m_rWindow.left    = //  unchanged - always left hand side
  //  m_rWindow.right   = //  unchanged, should be full width
  //  make sure that the right is also correct
  //  but that should not change.
  rDraw.right         = rDraw.right>m_ptMaxValues.x?m_ptMaxValues.x:rDraw.right;
  const auto bRedraw  = ResizeCommandWindow( rDraw );

  //  if we are going to redraw this
  //  don't bother outputting anything to the screen
  //  but we need to finish the work with this device context.
  //  because the new window size is not into effect just yet.
  if( false == bRedraw )
  {
    // if we have no text to display then we have no need to call the
    // DrawHTML(...) function again.
    if( len > 0 )
    {
      rDraw.left   = m_rWindow.left  + static_cast<int>(::myodd::config::Get(L"commands\\pad.x", 2));    //  use the full width, (left right)
      rDraw.right  = m_rWindow.right - static_cast<int>(::myodd::config::Get(L"commands\\pad.x", 2));    //  and make sure we tabulate the items, (probably RECT_MIN_TAB).
      //  and now we can show the text properly
      //
      // Set the background to transparent
      // This only relly applies to text, but that is what we are showing here.
      SetBkMode( hdc, TRANSPARENT );
      myodd::html::html(hdc, sCommand.c_str() , sCommand.length(), &rDraw, DT_DEFAULT );
    }

    //  
    // Show the time in the top left corner
    // Note that we only show the time if we actually draw the rectangle.
    DisplayTime( hdc, rDraw );
  }// if we are redrawing

  //  clean up old fonts
  if ( pOldFont != nullptr)
  {
    SelectObject( hdc, pOldFont );
  }
  return bRedraw;
}

/**
 * \brief add the time at the bottom of the commands window.
 * \param hdc the handle device context
 * \param rParent the parent rectangle.
 */
void ActionMonitorDlg::DisplayTime( const HDC hdc, RECT &rParent )
{
  //  But only if we want to!
  if( ::myodd::config::Get( L"commands\\show.time", 1) )
  {
    const auto pOldTime = SelTimeFont( hdc );
    if(nullptr != pOldTime )
    {
      __time64_t long_time;
      _time64( &long_time ); 
      struct tm newtime;

      const auto err = _localtime64_s( &newtime, &long_time ); 
      if (!err)
      {
        TCHAR szBuffer[ 256 ];
        _tcsftime(szBuffer, _countof(szBuffer), _T("%A, %B %d, %Y, %H:%M"), &newtime);

        RECT r      = {0,0,0,0};
        const auto bufferLen = _tcslen(szBuffer);
        if( bufferLen > 0 )
        {
          DrawText( hdc, szBuffer , bufferLen, &r, DT_DEFAULT | DT_CALCRECT);
          r.left   = (rParent.right-rParent.left) - (r.right) - 5;
          r.right  = (rParent.right-rParent.left) - 5;
          r.top    = (rParent.top);
          r.bottom = (r.top+r.bottom);
          DrawText( hdc, szBuffer , bufferLen, &r, DT_DEFAULT );
        }
      }// if _tcsftime(...)
      
      //  restore the font
      if( pOldTime != nullptr)
      {
        SelectObject( hdc, pOldTime );
      }
    }// SelTimeFont(...)
  }// ::myodd::config::Get( "commands\\show.time", 1)
}// DisplayTime(...)

/**
 * \brief Resize the command window if need be, (and return true)
 *        Otherwise return false if nothing was changed.
 * \param newSize the new size of the rectangle
 * \return if the window size was changed or not.
 */
bool ActionMonitorDlg::ResizeCommandWindow( const RECT &newSize )
{
  //  compare the structs
  if( memcmp( &newSize, &m_rWindow, sizeof(RECT)) != 0 )
  {
    //  copy the struct
    memcpy( &m_rWindow, &newSize, sizeof(RECT) );

    //  set the window pos and resize the window.
    SetWindowPos( &wndTopMost, 
                  m_rWindow.left, 
                  m_rWindow.top, 
                  (m_rWindow.right-m_rWindow.left),
                  (m_rWindow.bottom-m_rWindow.top),
                  SWP_SHOWWINDOW );

    //
    //  we are not telling the system to redraw the window
    //  this is not what this function is for!
    //  all we are doing is resizing it.
    //
    return true;
  }

  //  nothing was changed
  return false;
}

/**
 * \brief get the time font.
 * \param hdc the handle of the device context.
 * \return handle of the timer font.
 */
HGDIOBJ ActionMonitorDlg::SelTimeFont( const HDC hdc )
{
  if(nullptr == _fontTime )
  {
	  LOGFONT logFont;
	  memset(&logFont, 0, sizeof(LOGFONT));
	  logFont.lfHeight	=	16;
	  logFont.lfWeight	=	FW_NORMAL;
	  logFont.lfItalic	=	FALSE;
	  lstrcpy(logFont.lfFaceName, _T("Arial") ); 

    _fontTime = new CFont();
	  if (!_fontTime->CreateFontIndirect(&logFont))
    {
      return nullptr;
    }
  }

  return ::SelectObject(hdc, *_fontTime );
}

/**
 * \brief When we get this message it means that the message pump is now up and running.
 *        this is important for plugins that require the message pump
 *        Like IPC messages.
 * \return unused/reserved
 */
LRESULT ActionMonitorDlg::OnMessagePumpReady(WPARAM, LPARAM)
{
  //  do all the actions that are labeled as 'start'
  _application.ShowStart();

  return 0L;
}

/**
 * \brief Close this instance
 */
void ActionMonitorDlg::OnTrayExit() 
{
  _application.Close();
}

/**
 * Reload the commands, options
 *
 * @param void
 * @return void
 */
void ActionMonitorDlg::OnTrayReload() 
{
  _application.Restart();
}

/**
 * et the software version number.
 *
 * @param void
 * @return void
 */
void ActionMonitorDlg::OnTrayVersion()
{
  _application.ShowVersion();
}
