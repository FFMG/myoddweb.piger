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
#include "MemDC.h"
#include "ActionMonitorDlg.h"
#include "MessagesHandler.h"

/**
 * \brief the constructor
 * \see CDialog::CDialog
 * \param tray the system tray
 */
ActionMonitorDlg::ActionMonitorDlg( ITray& tray)
  : 
  CommonWnd( L"ActionMonitorDlg"),
  FadeWnd(),
  _fontTime(nullptr), 
  _tray( tray )

{
  _mainWindowPosition = { 0 };
  _commandRectangle = { 0 };
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
  // and now show the text
  ShowWindow(sCommand, ::myodd::config::Get(L"commands\\transparency", 127));
}

void ActionMonitorDlg::Hide()
{
  // just hide the window.
  ::ShowWindow(GetSafeHwnd(), SW_HIDE );
  ::UpdateWindow(GetSafeHwnd());
}

void ActionMonitorDlg::Active()
{
  _tray.SetActive();
}

void ActionMonitorDlg::Inactive()
{
  _tray.SetInactive();;
}

/**
 * \copydoc
 */
bool ActionMonitorDlg::OnInitDialog()
{
  // call the parent now
  if (!CommonWnd::OnInitDialog())
  {
    return false;
  }

  HideTaskBar();

  // tell the fade window who the parent window is.
  SetFadeParent(GetSafeHwnd());

  return true;
}

/**
 * \brief show the window with a given text.
 * \param sCommand the message we want to display in the window.
 * \param bTrans how transparent we want the window to be.
 */
void ActionMonitorDlg::ShowWindow(const std::wstring& sCommand, const BYTE bTrans )
{
  // recalculate everything
  RecalculateAllRectangles(sCommand);

  // make sure that the window has the correct position
  RepositionWindow();

  // show the window.
  if (!IsWindowVisible(GetSafeHwnd()))
  {
    ::ShowWindow(GetSafeHwnd(), SW_SHOW);
    ::UpdateWindow(GetSafeHwnd());
  }

  // set the transparency
  SetTransparency(bTrans );

  // save the command we want to show.
  _sCommand = sCommand;

  // and force a redraw
  ::PostMessage( GetSafeHwnd(), WM_PAINT, 0, 0 );
}

/**
 * \brief recalculate all the window sizes at once.
 */
void ActionMonitorDlg::RecalculateAllRectangles(const std::wstring& sCommand)
{
  // first we will need a device context.
  //  get the size of the windows
  const auto hdc = ::GetDC( GetSafeHwnd());
  try
  {
    // this is what we have to be inside of....
    _mainWindowPosition = CalculateMinimumRectPosition(sCommand, hdc);
    _commandRectangle = CalculateCommandRectangle(sCommand, hdc);
  }
  catch( ... )
  {
    // something broke here
    // but we still need to free the device context.
  }
  ::ReleaseDC(GetSafeHwnd(), hdc);
}

/**
 * \brief calculate the minimum window position, that is a window with no text in it.
 * \param sCommand the command we are waiting for.
 * \param hdc the handle to the device context we are using
 */
RECT ActionMonitorDlg::CalculateMinimumRectPosition(const std::wstring& sCommand, const HDC hdc )
{
  // const auto top = ::myodd::config::Get(L"commands\\top", 5);
  // const auto left = ::myodd::config::Get(L"commands\\left", 5);

  // get the width of the screen
  const auto screenWidth = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
  const auto screenHeight = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

  // now calculate a percent of the screen
  // and that will be our maximum
  const auto maxScreenWidth = static_cast<int>(screenWidth * static_cast<int>(::myodd::config::Get(L"commands\\max.x", 99)) / 100.f);
  const auto maxScreenHeight = static_cast<int>(screenHeight * static_cast<int>(::myodd::config::Get(L"commands\\max.y", 99)) / 100.f);

  // the padding on either side is half the difference of 100% of the screen.
  // and the calculated max
  const auto paddingScreenWidth = static_cast<int>((screenWidth - maxScreenWidth) / 2);
  const auto paddingScreenHeight = static_cast<int>((screenHeight - maxScreenHeight) / 2);

  // the padding of the text
  const auto paddingText = static_cast<int>(::myodd::config::Get(L"commands\\pad.y", 2));

  // calculate the text height so we can get the height.
  // the width does not matter as we will not be using it.
  // if the command is empty we will pass one character to make sure we have a proper minimum height.
  const auto nonEmptyCommand = (sCommand.length() == 0 ? L"IM" : sCommand);
  const auto rectText = CalculateCommandRectangle(nonEmptyCommand, hdc);
  const auto height = (2*paddingText+rectText.bottom) > maxScreenHeight ? maxScreenHeight : (2*paddingText+rectText.bottom);

  return {
    paddingScreenWidth,
    paddingScreenHeight,
    maxScreenWidth,
    height + paddingScreenHeight
  };
}

/**
 * \brief given the command window, recalculate the total size needed for the window to be displayed.
 *        it will use the minimum size to make sure that the text is not too small either
 * \param sCommand the command we are measuring
 * \param hdc the device context handle.
 */
RECT ActionMonitorDlg::CalculateCommandRectangle(const std::wstring& sCommand, const HDC hdc)
{
  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont(hdc, 70);

  // get the text size
  RECT rTextRectangle = { 0,0,0,0 };
  myodd::html::html(hdc, sCommand.c_str(), static_cast<int>(sCommand.length()), &rTextRectangle, DT_DEFAULT | DT_CALCRECT);

  // replace the old font.
  // The resource for it is freed when FramWnd closes.
  if (pOldFont != nullptr)
  {
    SelectObject(hdc, pOldFont);
  }
  return rTextRectangle;
}

/**
 * \brief If you add a minimize button to your dialog, you will need the code below
 *        to draw the icon.  For MFC applications using the document/view model,
 *        this is automatically done for you by the framework.
 */
void ActionMonitorDlg::OnPaint() 
{
  const auto mainHdc = ::GetDC(GetSafeHwnd());
  const auto myDc = new ActionMonitorMemDC(CDC::FromHandle(mainHdc));
  const auto hdc = myDc->GetSafeHdc();

  try
  {
    // show the background
    RedrawBackground(hdc);

    // and the command itself 
    RedrawText( _sCommand, hdc);

    // then draw the clock
    RedrawTime(hdc);
  }
  catch (...)
  {
    // something broke
    // but we must still free thr resources.
  }

  delete myDc;
  ReleaseDC(GetSafeHwnd(), mainHdc);
}

void ActionMonitorDlg::RedrawText(const std::wstring& sCommand, const HDC hdc)
{
  // the padding of the text
  const auto paddingText = static_cast<int>(::myodd::config::Get(L"commands\\pad.y", 2));

  // calculate the text height so we can get the height.
  // the width does not matter as we will not be using it.
  auto rectText = _commandRectangle;
  rectText.top += _mainWindowPosition.top+paddingText;
  rectText.bottom += _mainWindowPosition.bottom + paddingText;
  rectText.left += _mainWindowPosition.left + paddingText;
  rectText.right += _mainWindowPosition.right + paddingText;

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont(hdc, 70);

  // get the text size
  myodd::html::html(hdc, sCommand.c_str(), static_cast<int>(sCommand.length()), &rectText, DT_DEFAULT );

  //  clean up old fonts
  if (pOldFont != nullptr)
  {
    SelectObject(hdc, pOldFont);
  }
}

/**
 * \brief resize and move the window if needed.
 */
void ActionMonitorDlg::RepositionWindow() const
{
  RECT rCurrent = { 0 };
  GetWindowRect(GetSafeHwnd(), &rCurrent);
  if( 0 == memcmp(&_mainWindowPosition, &rCurrent, sizeof(RECT) ))
  {
    // nothing has changed.
    return;
  }

  //  move the window to the top left hand corner 
  SetWindowPos(GetSafeHwnd(),
    HWND_TOPMOST,
    _mainWindowPosition.left,
    _mainWindowPosition.top,
    (_mainWindowPosition.right - _mainWindowPosition.left),
    (_mainWindowPosition.bottom - _mainWindowPosition.top),
    SWP_HIDEWINDOW );
}

/**
 * \brief draw the background in the device context.
 *        this is the un-faded background.
 *        the fade wnd will do the fading for us.
 * \param hdc the handle to the device context we are working in.
 */
void ActionMonitorDlg::RedrawBackground(const HDC hdc) const
{
  SetBkMode(hdc, TRANSPARENT);

  const auto colour = ::GetSysColor(COLOR_3DFACE);
  const auto hBrush = CreateSolidBrush(colour);

  const RECT zeroSizeWindow = { 0,0, (_mainWindowPosition.right - _mainWindowPosition.left), (_mainWindowPosition.bottom - _mainWindowPosition.top) };
  FillRect(hdc, &zeroSizeWindow, hBrush);

  DeleteObject(hBrush);
}

/**
 * \brief add the time at the bottom of the commands window.
 * \param hdc the handle device context
 */
void ActionMonitorDlg::RedrawTime( const HDC hdc )
{
  //  But only if we want to!
  if (1 != ::myodd::config::Get(L"commands\\show.time", 1))
  {
    return;
  }

  const auto pOldTime = SelTimeFont( hdc );
  if (nullptr == pOldTime)
  {
    return;
  }

  __time64_t longTime;
  _time64( &longTime ); 
  struct tm newtime;

  const auto err = _localtime64_s( &newtime, &longTime ); 
  if (!err)
  {
    TCHAR szBuffer[ 256 ];
    _tcsftime(szBuffer, _countof(szBuffer), _T("%A, %B %d, %Y, %H:%M"), &newtime);

    const auto bufferLen = _tcslen(szBuffer);
    if( bufferLen > 0 )
    {
      RECT rClock = { 0,0,0,0 };
      DrawText( hdc, szBuffer , static_cast<int>(bufferLen), &rClock, DT_DEFAULT | DT_CALCRECT);
      rClock.left   = (_mainWindowPosition.right- _mainWindowPosition.left) - (rClock.right) - 5;
      rClock.right  = (_mainWindowPosition.right- _mainWindowPosition.left) - 5;
      rClock.top    = (_mainWindowPosition.top);
      rClock.bottom = (rClock.top+ rClock.bottom);
      DrawText( hdc, szBuffer , static_cast<int>(bufferLen), &rClock, DT_DEFAULT );
    }
  }// if _tcsftime(...)
  
  SelectObject( hdc, pOldTime );
}// DisplayTime(...)

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