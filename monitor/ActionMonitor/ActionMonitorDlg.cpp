// ActionMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ActionsCore.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"

#define RECT_MIN_H 70

// we are not running
static bool g_isRunning = false;

/**
 * Check if the dialog is running or not.
 * Static function.
 * @param void
 * @return bool true|false if the dialog is running or not, (destroyed).
 */
bool CActionMonitorDlg::IsRunning()
{
  return g_isRunning;
}

/**
 * todo
 * @see CTrayDialog::CTrayDialog
 * @param void
 * @return void
 */
CActionMonitorDlg::CActionMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CTrayDialog(CActionMonitorDlg::IDD, pParent), 
  m_keyState (ACTION_NONE),
  fontTime(nullptr),
  _IpcListener(nullptr)
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  // save the main thread id.
  _main_threadId = std::this_thread::get_id();
}

/**
 * todo
 * @param void
 * @return void
 */
CActionMonitorDlg::~CActionMonitorDlg()
{
  //  clear the ipc listener
  if (_IpcListener != nullptr)
  {
    delete _IpcListener;
    _IpcListener = nullptr;
  }

  if( fontTime )
  {
    fontTime->DeleteObject();
    delete fontTime;
  }
}

/**
 * todo
 * @see CTrayDialog::DoDataExchange
 * @param void
 * @return void
 */
void CActionMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CActionMonitorDlg, CTrayDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_REGISTERED_MESSAGE(UWM_KEYBOARD_CHAR   , OnHookKeyChar)
  ON_REGISTERED_MESSAGE(UWM_KEYBOARD_DOWN   , OnHookKeyDown)
  ON_REGISTERED_MESSAGE(UWM_KEYBOARD_UP     , OnHookKeyUp)
  ON_REGISTERED_MESSAGE(UWM_KEYBOARD_RELOAD , OnReload)
  ON_REGISTERED_MESSAGE(UWM_KEYBOARD_VERSION, OnVersion)
  ON_REGISTERED_MESSAGE(UWM_DISPLAYMESSAGE  , OnDisplayMessage )
  ON_WM_WINDOWPOSCHANGING()
  ON_COMMAND(ID_TRAY_EXIT, OnTrayExit)
	ON_COMMAND(ID_TRAY_RELOAD, OnTrayReload)
  ON_COMMAND(ID_TRAY_VERSION, OnTrayVersion)
  ON_WM_CLOSE()
  ON_WM_DESTROY()
END_MESSAGE_MAP()

/**
 * todo
 * @see CTrayDialog::OnWindowPosChanging
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
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
BOOL CActionMonitorDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();
  ModifyStyleEx(WS_EX_APPWINDOW,0); //  no taskbar!

  //  we are up and running!
  g_isRunning = true;

  // Set the icon for this dialog.  The framework does this automatically
	//  when the applications main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	TraySetIcon(IDR_MAINFRAME);

  myodd::files::Version _ver;
#ifdef _DEBUG
  MYODD_STRING strSay = myodd::strings::Format(_T("Action Monitor [Debug]: %d.%d.%d.%d"),
#else
  MYODD_STRING strSay = myodd::strings::Format(_T("Action Monitor : %d.%d.%d.%d"),
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

  // initialise the listener.
  InitializeListener();

  //  do all the actions that are labeled as 'start'
  App().DoStartActionsList();

  //  setup the hooks and the key
  InitHook();
  
  return TRUE;
}

void CActionMonitorDlg::InitializeListener()
{
  if (nullptr == _IpcListener)
  {
    // lock us in
    myodd::threads::Lock guard(_mutex);

    // if this is done already, we don't want to do it again.
    if (nullptr == _IpcListener)
    {
      // create the listenner and pass oursleves as the window.
      // so some messages can be routed back to us.
      auto hWnd = GetSafeHwnd();
      _IpcListener = new myodd::os::IpcListener(CONF_MUTEXT, hWnd);
    }
  }
}


/**
 * todo
 * @param void
 * @return void
 */
BOOL CActionMonitorDlg::IsSpecialKeyDown( )const
{
  SHORT vk = GetKeyState(SPECIAL_KEY);
  return ((vk & 0xffff) == 1 );
}

/**
 * todo
 * @param void
 * @return void
 */
void CActionMonitorDlg::InitHook()
{
  //  in case we called setHook(...); and crashed we might not be able to
  //  restart the hook
  //  so clear the hook and then restart it...
  hook_clear( m_hWnd );

  //  if the key is down then we need to send a message to
  //  tell the system to do the work
  if( IsSpecialKeyDown() )
  {
    INPUT input[2] ;
    ZeroMemory( input, sizeof(input) ) ;
    input[0].type       = INPUT_KEYBOARD ;
    input[0].ki.wVk     = SPECIAL_KEY;

    input[0].ki.time    = input[1].ki.time    = GetTickCount() ;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP ;
    SendInput( 1, input, sizeof(INPUT) ) ; 

    //  surrender our thread time to give time for the toggle to work.
    Sleep(500);
  }

  //  start hook the DLL
  hook_set(m_hWnd, SPECIAL_KEY);
}

/**
 * todo
 * @see CTrayDialog::CTrayDialog
 * @param void
 * @return void
 */
void CActionMonitorDlg::ShowWindow( BYTE bTrans )
{
  if( IsVisible() != bTrans )
  {
    Visible(bTrans);
    Transparent( bTrans );
    if( bTrans == 0 )
    {
      // remove the current action
      // we are hidding the current action
      App().PossibleActions().CurrentActionReset();

      //  we need to recalculate the size of the command window
      //  normally when we hide the window it is because we are no longer
      //  typing anything and the text has been removed
      //
      //  so by calling a Display(...) we are forcing a command window resize.
      //  this is handy so that the next time we press the key we wont see a 'flicker'
      //  of the previous command window.
      DisplayCommand( ); //  return value is ignored.
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
void CActionMonitorDlg::CalcMaxes( )
{
	float fX = (float)GetSystemMetrics(SM_CXSCREEN);
	float fY = (float)GetSystemMetrics(SM_CYSCREEN);

  //  calc the percentages.
  m_ptMaxValues.x = (int)(fX * (int)myodd::config::get( _T("commands\\max.x"), 99)/100.f );
  m_ptMaxValues.y = (int)(fY * (int)myodd::config::get( _T("commands\\max.y"), 99)/100.f );
}

/**
 * Create the main window the window is hidden by default.
 *
 * @param none
 * @return none
 */
void CActionMonitorDlg::InitWindow( )
{
  CalcMaxes();

  m_rWindow.bottom  = RECT_MIN_H;
  m_rWindow.left    = myodd::config::get( _T("commands\\left"), 5);
  m_rWindow.top     = myodd::config::get( _T("commands\\top"), 5);
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
void CActionMonitorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
    if( IsVisible() == 0 )
    {
      //  hide the window.
      ::ShowWindow( m_hWnd, SW_HIDE );
    }
    else
    {
      ActionMonitorMemDC *myDC = new ActionMonitorMemDC( CDC::FromHandle(dc.GetSafeHdc()) );
      HDC hdc = myDC->GetSafeHdc();

      // display command will return true if the rectangle size was updated.
      // if the rectangle, (the display window), changes size then we need to 
      //  create a new ActionMonitorMemDC as it only knows of the old size and will not update
      // the new area.
      //
      // so by deleting the old memdc we tell the system to reject it and to recalculate
      // the area from the device context
      while( DisplayCommand( hdc ) )
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
}

/**
 * The system calls this to obtain the cursor to display while the user drags
 * the minimized window.
 *
 * @see CTrayDialog::OnQueryDragIcon
 * @param void
 * @return void
 */
HCURSOR CActionMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/**
 * todo
 * @see CTrayDialog::CTrayDialog
 * @param void
 * @param void
 * @return void
 */
LRESULT CActionMonitorDlg::OnHookKeyChar(WPARAM wParam, LPARAM lParam)
{ 
  /**
   * The WM_CHAR message is posted to the window with the keyboard focus when a WM_KEYDOWN message is translated by the TranslateMessage function. 
   * The WM_CHAR message contains the character code of the key that was pressed.
   */
  return 0L;
}

/**
 * Return true if a param is our special key
 *
 * @param void
 * @return void
 */
BOOL CActionMonitorDlg::IsSpecialKey( WPARAM wParam ) const
{
  return (wParam == SPECIAL_KEY);
}

/**
 * todo
 * @param void
 * @param void
 * @return void
 */
LRESULT CActionMonitorDlg::OnHookKeyDown(WPARAM wParam, LPARAM lParam)
{
  TRACE( "KeyDown 0x%x\n", wParam );

  /**
   * The WM_KEYDOWN message is posted to the window with the keyboard focus when a nonsystem key is pressed. 
   * A nonsystem key is a key that is pressed when the ALT key is not pressed. 
   */

  //  if it is the special key then tell the system that from now own
  //  we need to record keys and prevent any other key from passing accross.
  if( IsSpecialKey(wParam) )
  {
    //  only do it if the key was NOT down previously.
    //  otherwise we will forever be reseting
    if( (m_keyState & ACTION_MAINKEY_DOWN) != ACTION_MAINKEY_DOWN )
    {
      // we need to save the last forground window
      // because showing our dialog box will change things a bit.
      CActionMonitorApp::SetLastForegroundWindow( GetForegroundWindow() );
      
      //  tell the system to no longer accept any more key
      m_keyState |= ACTION_MAINKEY_DOWN;
      hook_RejectKeyboad( TRUE );
      TraySetIcon(IDR_MAINFRAME_ACTIVE);
      TrayUpdate();

      //  reset the last command
      App().PossibleActions().CurrentActionReset();
      ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );
    }
  }

  //  if the special key is not down then we don't need to go any further
  if( (m_keyState & ACTION_MAINKEY_DOWN) != ACTION_MAINKEY_DOWN )
  {
    return 0L;
  }

  switch (wParam) 
  { 
  case VK_BACK:     // backspace 
    App().PossibleActions().CurrentActionBack();
    ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );
    break;

  case 0x0A:        // linefeed 
  case VK_TAB:      // tab 
  case VK_RETURN:   // carriage return 
  case VK_CLEAR:
    break; 

  case VK_ESCAPE:
    App().PossibleActions().CurrentActionReset( );
    ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );

  case VK_DOWN:
    App().PossibleActions().down();
    ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );
    break;

  case VK_CAPITAL:
    break;

  case VK_UP:
    App().PossibleActions().up();
    ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );
    break;

  case VK_SHIFT:
  case VK_RSHIFT:
    m_keyState |= ACTION_SHIFT_DOWN;
    break;

  case VK_LSHIFT:
    m_keyState |= (ACTION_SHIFT_DOWN | ACTION_LSHIFT_DOWN);
    break;

  default:
    {
      BYTE ks[256];
      memset( ks, 0, sizeof(ks) );
      GetKeyboardState(ks);
      WORD w;
      UINT scan;
      scan=0;
      
      ks[VK_SHIFT] = 1;
      if( (m_keyState & ACTION_SHIFT_DOWN) == ACTION_SHIFT_DOWN )
      {
        //  force the shift key down.
        //  the low level keyboad does not alway pass the key(shift) before the letter key
        //  so we might not know that the shift key is pressed.
        //  so we set it here.
        ks[VK_SHIFT] = 129;
      }
      
      if( 0 != ToAscii(wParam,scan,ks,&w,0) )
      {
        TCHAR c = (TCHAR)( CHAR(w) );
        //  add the current character to the list of items
        App().PossibleActions().CurrentActionAdd( c );

        //  make sure that the window is visible
        //  This will also force a refresh of the window
        ShowWindow( myodd::config::get( _T("commands\\transparency"), 127) );
      }
    }
    break;
  }
  return 0L;
}

/**
 * todo
 * @param void
 * @param void
 * @return void
 */
LRESULT CActionMonitorDlg::OnHookKeyUp(WPARAM wParam, LPARAM lParam)
{
  TRACE( "KeyUp 0x%x\n", wParam );

  //  check the key that the user had released
  //  if it the special key then tell the user that the key is no longer down
  if( IsSpecialKey(wParam) )
  {
    //  the key is no longer down
    //  but was it down at all to start with? - (sanity check)
    if( (m_keyState & ACTION_MAINKEY_DOWN) == ACTION_MAINKEY_DOWN )
    {
      //  remove the fact that the key was pressed.
      //  do that next time we come around here we don't do it again
      m_keyState &= ~ACTION_MAINKEY_DOWN;

      //  tell the system that we can now accept key press
      hook_RejectKeyboad( FALSE );
      TraySetIcon(IDR_MAINFRAME);
      TrayUpdate();

      try
      {
        //  We use the Actions to find the actual command
        //  because either it is an exact match, (and it doesn't matter)
        //  or it is not an exact match and this is what we meant to use
        // 
        //  so if the user enters 'goo french victories'
        //  we will use the first command 'google' with the arguments 'french victories'
        //
        //  we use getCommand in case the user has chosen number 1, 2 ... in the list of possible commands 
        MYODD_STRING szCommandLine = _T( "" );
        const Action* action = App().PossibleActions().GetCommand(&szCommandLine);
        if (nullptr != action)
        {
          //  do the action now
          //  we might not have any, but that's not for us to decides :).
          CWnd* pWnd = CActionMonitorApp::GetLastForegroundWindow();
          QueueAndExecute( action->CreateActiveAction( pWnd, szCommandLine, false) );
        }
      }
      catch( ... )
      {
        //  we don't have a valid command.
      }

      //  hide the window
      ShowWindow(0);
    }
  }

  switch (wParam) 
  { 
  case VK_SHIFT:
  case VK_RSHIFT:
  case VK_LSHIFT:
    m_keyState &= ~ACTION_SHIFT_DOWN;
    m_keyState &= ~ACTION_LSHIFT_DOWN;
    break;

  default:
    break;
  }

  /**
   * The WM_KEYUP message is posted to the window with the keyboard focus when a nonsystem key is released.
   * A nonsystem key is a key that is pressed when the ALT key is not pressed, 
   * or a keyboard key that is pressed when a window has the keyboard focus. 
   */
  return 0L;
}

/**
 * Check if the current thread is the main thread or not.
 * We only know we are the main thread because that number was set at construction.
 * @return bool if this thread is main or not.
 */
bool CActionMonitorDlg::IsMainThread() const
{
  return (std::this_thread::get_id() == _main_threadId);
}

/**
 * Display a message, this comes from another thread.
 * @param WPARAM unused/reserved
 * @param LPARAM unused/reserved
 * @return LRESULT unused/reserved
 */
LRESULT CActionMonitorDlg::OnDisplayMessage
(
  WPARAM,
  LPARAM l
  )
{
  MessageDlg::Msg* msg = (MessageDlg::Msg*)l;
  bool result = DisplayMessage(msg->Text(), msg->Elapse(), msg->FadeOut());
  delete msg;
  return result ? 1 : 0;
}

/**
 * todo
 * @param void
 * @param void
 * @param void
 * @return void
 */
bool CActionMonitorDlg::DisplayMessage
( 
 LPCTSTR pText, 
 UINT nElapse, 
 UINT nFadeOut 
)
{
  // Sanity check
  if(nullptr == pText )
  {

    return false;
  }

  if (!IsMainThread())
  {

    // if this is not the main thread
    // then we need to POST to oursleves and wait for the message to complete.
    PostMessage(UWM_DISPLAYMESSAGE, 0, (LPARAM)(new MessageDlg::Msg(pText, nElapse, nFadeOut)) );
    return true;
  }
  
  //  look for old messages to remove
  ClearUnusedMessages( );

  MessageDlg* messageDlg = new MessageDlg( this );
  messageDlg->Create( pText, nElapse, nFadeOut );

  // if this window is still running, (and valid)
  if( CActionMonitorDlg::IsRunning() )
  {
    messageDlg->FadeShowWindow();

    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    // add it to our list of messages.
    _displayWindows.push_back( messageDlg );
  }
  else
  {
    messageDlg->DestroyWindow();
  }

  return true;
}

/**
 * Display the current command that will be executing.
 *
 * @param void
 * @return void
 */
bool CActionMonitorDlg::DisplayCommand( HDC hdc /*= nullptr*/ )
{
  // should we even be here?
  if( 0 == IsVisible() )
  {
    return false;
  }
  //  get the current text as well as the possible commands.
  //  we pass what ever the user entered to whatever commands are saved.
  MYODD_STRING sCommand = App().PossibleActions().toChar( );
  size_t len = sCommand.length();

  HDC localHdc = nullptr;
  if(nullptr == hdc )
  {
    localHdc = ::GetDC(nullptr);
  }
  else
  {
    localHdc = hdc;
  }

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  HGDIOBJ pOldFont = SelDisplayFont( localHdc );

  //  we will use 1 rectangle.
  RECT rDraw  = {0,0,0,0};
  
  // first we use DT_CALCRECT to get their size rather than drawing them on the screen.

  //calculate the width of the string using the classic way
  if( 0 == sCommand.length() )
  {
    SIZE sizeText;
    GetTextExtentPoint32(localHdc, _T("I"), 1, &sizeText);
    rDraw.bottom = sizeText.cy;
  }
  else
  {
    myodd::html::html(localHdc, sCommand.c_str() , len, &rDraw, DT_DEFAULT | DT_CALCRECT );
  }
 
  // add padding to the bottom pos of the window.
  rDraw.bottom        += m_rWindow.top + ( 2* (int)myodd::config::get( _T("commands\\pad.y"), 2));
  
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
  bool bRedraw        = ResizeCommandWindow( rDraw );

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
      rDraw.left   = m_rWindow.left  + (int)myodd::config::get( _T("commands\\pad.x"), 2);    //  use the full width, (left right)
      rDraw.right  = m_rWindow.right - (int)myodd::config::get( _T("commands\\pad.x"), 2);    //  and make sure we tabulate the items, (probably RECT_MIN_TAB).
      //  and now we can show the text properly
      //
      // Set the background to transparent
      // This only relly applies to text, but that is what we are showing here.
      SetBkMode( localHdc, TRANSPARENT );
      myodd::html::html(localHdc, sCommand.c_str() , sCommand.length(), &rDraw, DT_DEFAULT );
    }

    //  
    // Show the time in the top left corner
    // Note that we only show the time if we actually draw the rectangle.
    DisplayTime( localHdc, rDraw );
  }// if we are redrawing

  //  clean up old fonts
  if ( pOldFont != nullptr)
  {
    SelectObject( localHdc, pOldFont );
  }

  if(nullptr == hdc )
  {
    ::ReleaseDC(nullptr, localHdc );
  }
  return bRedraw;
}

/**
 * todo
 * @param void
 * @param void
 * @return void
 */
void CActionMonitorDlg::DisplayTime
(
  HDC hdc, 
  RECT &rParent
)
{
  //  But only if we want to!
  if( myodd::config::get( _T("commands\\show.time"), 1) )
  {
    HGDIOBJ pOldTime = SelTimeFont( hdc );
    if(nullptr != pOldTime )
    {
      __time64_t long_time;
      _time64( &long_time ); 
      struct tm newtime;
      errno_t err;

      err = _localtime64_s( &newtime, &long_time ); 
      if (!err)
      {
        TCHAR szBuffer[ 256 ];
        _tcsftime(szBuffer, _countof(szBuffer), _T("%A, %B %d, %Y, %H:%M"), &newtime);

        RECT r      = {0,0,0,0};
        size_t bufferLen = _tcslen(szBuffer);
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
  }// myodd::config::get( "commands\\show.time", 1)
}// DisplayTime(...)

/**
 * Resize the command window if need be, (and return true)
 * Otherwise return false if nothing was changed.
 *
 * @param RECT the new size of the rectangle
 * @return BOOL if the window size was changed or not.
 */
bool CActionMonitorDlg::ResizeCommandWindow( const RECT &newSize )
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
 * todo
 * @param void
 * @return void
 */
HGDIOBJ CActionMonitorDlg::SelTimeFont( HDC hdc )
{
  if(nullptr == fontTime )
  {
	  LOGFONT logFont;
	  memset(&logFont, 0, sizeof(LOGFONT));
	  logFont.lfHeight	=	16;
	  logFont.lfWeight	=	FW_NORMAL;
	  logFont.lfItalic	=	FALSE;
	  lstrcpy(logFont.lfFaceName, _T("Arial") ); 

    fontTime = new CFont();
	  if (!fontTime->CreateFontIndirect(&logFont))
    {
      return nullptr;
    }
  }

  HGDIOBJ pOldTime = ::SelectObject(hdc, *fontTime );
  return pOldTime;
}

/**
 * Message to tell the system to give us the version number.
 *
 * @param WPARAM unused/reserved
 * @param LPARAM unused/reserved
 * @return LRESULT unused/reserved
 */
LRESULT CActionMonitorDlg::OnVersion
(
  WPARAM,
  LPARAM
)
{
  myodd::files::Version _ver;
  MYODD_STRING strSay = myodd::strings::Format( _T("<b>Version : </b>%d.%d.%d.%d"),
    _ver.GetFileVersionMajor(),
    _ver.GetFileVersionMinor(),
    _ver.GetFileVersionMaintenance(),
    _ver.GetFileVersionBuild() );
  
  DisplayMessage( strSay.c_str(), 3000, 5 );

  return 0L;
}

/**
 * Message to tell the system to reload everything
 * This is called when there has been a change to the options.xml
 *
 * @param WPARAM unused/reserved
 * @param LPARAM unused/reserved
 * @return LRESULT unused/reserved
 */
LRESULT CActionMonitorDlg::OnReload
(
  WPARAM,
  LPARAM
)
{
  // stop everything and wait for them.
  App().DoEndActionsList( );

  // wait all the active windows.
  // those are the ones created by the end Action list.
  WaitForActiveWindows();

#ifdef ACTIONMONITOR_API_PLUGIN
  // We have to kill all the API plugins.
  // they should be all done and completed.
  PluginVirtualMachine* pg = App().GetPluginVirtualMachine( );
  pg->DestroyPlugins();
#endif // ACTIONMONITOR_API_PLUGIN

  //
  //  Restart everything.
  //

  //  set up up the command window for the first time, (hidden)
  InitWindow();

  //  setup the hooks and the key
  InitHook();

  // (re)build the action list
  App().BuildActionsList();

  //  and restart everything
  App().DoStartActionsList( );

  // wait all the active windows.
  // those we just re-started.
  WaitForActiveWindows();

  //  we are now done.
  return 0L;
}

/**
 * Close this instance
 *
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnTrayExit() 
{
  ::PostMessage( m_hWnd, WM_CLOSE, 0, 0 );	
}

/**
 * Reload the commands, options
 *
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnTrayReload() 
{
  ::PostMessage( m_hWnd, UWM_KEYBOARD_RELOAD, 0, 0 );
}

/**
 * et the software version number.
 *
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnTrayVersion()
{
  ::PostMessage( m_hWnd, UWM_KEYBOARD_VERSION, 0, 0 );
}

/**
 * todo
 * @see CTrayDialog::OnClose
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnClose()
{
  // log that we are closing down
  myodd::log::LogMessage(_T("Piger is shutting down."));

  //  hide the main window
  ShowWindow(0);

  //  remove the hooks
  hook_clear( m_hWnd );

  WaitForActiveWindows( );

  __super::OnClose();
}

/**
 * todo
 * @param void
 * @return void
 */
void CActionMonitorDlg::ClearUnusedMessages()
{
  while( _displayWindows.size() > 0 )
  {
    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    vMessages::iterator it = _displayWindows.begin();
    if( it == _displayWindows.end() )
    {
      break;
    }

    MessageDlg* dlg = (MessageDlg*)(*it);
    if (dlg != nullptr)
    {
      HWND hwnd = dlg->GetSafeHwnd();
      if (0 != ::GetWindowLongPtr(hwnd, GWLP_HWNDPARENT))
      {
        // no need to go past this one
        break;
      }
    }
    //  otherwise we can remove it from the list
    _displayWindows.erase( it );
  }
}

/**
 * Kill all the currently active message windows.
 */
void CActionMonitorDlg::KillAllActiveWindows()
{
  //  remove what is complete.
  ClearUnusedMessages();

  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // kill the other messages;
  for (vMessages::iterator it = _displayWindows.begin();
       it != _displayWindows.end();
       ++it)
  {
    //  clear what is still good.
    MessageDlg* dlg = (MessageDlg*)(*it);
    if (dlg != nullptr)
    {
      dlg->FadeKillWindow();
    }
  }

  // now that we asked for windows to be closed.
  // we can wait for them to close.
  WaitForActiveWindows();
}

/**
 * todo
 * @param void
 * @return void
 */
void CActionMonitorDlg::WaitForActiveWindows()
{
  // first give our window a chance to process UWM_DISPLAYMESSAGE.
  // but we don't want to do other messages only the pending display messages.
  MSG msg;
  while (PeekMessage(&msg, m_hWnd, UWM_DISPLAYMESSAGE, UWM_DISPLAYMESSAGE, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Wait for pending messages
  // we try and get the parent window
  // and if we cannot locate it, then it must be because the window no longer exists
  // and as such we must wait for it.
  while( true )
  {
    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    vMessages::iterator it = _displayWindows.begin();
    if( it == _displayWindows.end() )
      break;

    MessageDlg* dlg = (MessageDlg*)(*it);
    if (dlg != nullptr)
    {
      // then try and process the remaining messages
      // if the window has not been killed properly.
      HWND hWnd = dlg->GetSafeHwnd();
      if (0 != ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
      {
        //  give the other apps/classes one last chance to do some work.
        MessagePump(nullptr);

        // let go of the thread.
        std::this_thread::yield();

        // just do one message at a time
        // so we don't block others.
        // the peek message should allow all message to be handled
        MessagePump(hWnd);
        
        // go around one last time
        // to give everyone a chance to close.
        continue;
      }
    }

    //  otherwise we can remove it
    _displayWindows.erase( it );
  }
}

/**
* Todo
* @param void
* @return void
*/
void CActionMonitorDlg::MessagePump(HWND hWnd)
{
  //  lock up to make sure we only do one at a time
  MSG msg;
  while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (0 == ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
    {
      break;
    }
  }
}

/**
 * todo
 * @see CTrayDialog::OnDestroy
 * @param void
 * @return void
 */
void CActionMonitorDlg::OnDestroy()
{
  // the window is about to be destroyed, 
  // we are no longer up and running!
  g_isRunning = false;

  // destroy the window.
  __super::OnDestroy(); 
}

/**
 * Add a message handler to our list of handlers.
 * Each will be called when a new message arrives.
 * @param IpcMessageHandler& handler the message handler we are adding.
 */
void CActionMonitorDlg::AddMessageHandler(myodd::os::IpcMessageHandler& handler)
{
  myodd::threads::Lock guard(_mutex);
  if(_IpcListener == nullptr )
  {
    return;
  }
  _IpcListener->AddMessageHandler(handler);
}

/**
 * Remove a message handler from our list.
 * @param IpcMessageHandler& handler the message handler we want to remove.
 * @return none
 */
void CActionMonitorDlg::RemoveMessageHandler(myodd::os::IpcMessageHandler& handler)
{
  myodd::threads::Lock guard(_mutex);
  if (_IpcListener == nullptr)
  {
    return;
  }
  _IpcListener->RemoveMessageHandler(handler);
}
