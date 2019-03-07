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
#include "MessageDlg.h"
#include "MemDC.h"

#define MAX_TRANSPARENCY (unsigned char)255
#define DEFAULT_TRANSPARENCY (unsigned char)127

/**
 * \brief the constructor
 * \see CDialog::CDialog
 */
MessageDlg::MessageDlg()
  : CommonWnd( L"MessageDlg" ),
    FadeWnd(), 
    _totalMilisecondsToShowMessage(0),
    _elapseMiliSecondsBeforeFadeOut(0)
{
  _completeRect = { 0,0,0,0 };
}

MessageDlg::~MessageDlg()
{
  Stop();
  CommonWnd::Close();
}

/**
 * \brief called when the window is being created, we can then set our own values.
 * \see CDialog::Create
 * \param sText the text we want to display
 * \param elapseMiliSecondsBeforeFadeOut how long we want to display the message for before we fade out.
 * \param totalMilisecondsToShowMessage how long we want the message to be displayed before fading out.
 */
void MessageDlg::Create(const std::wstring& sText, const long elapseMiliSecondsBeforeFadeOut, const long totalMilisecondsToShowMessage)
{
  //  save the value
  _totalMilisecondsToShowMessage = totalMilisecondsToShowMessage;
  _mStdMessage = sText;
  _elapseMiliSecondsBeforeFadeOut = elapseMiliSecondsBeforeFadeOut;

  __super::Create();
}

/**
 * \brief Initialise the fading window.
 */
void MessageDlg::InitWindowPos()
{
  //  get the size of the windows
  const auto hdc = ::GetDC( GetSafeHwnd() );

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont( hdc, 35 );

  RECT r = {0,0,0,0};
  myodd::html::html(hdc, _mStdMessage.c_str() , _mStdMessage.length(), &r, DT_DEFAULT | DT_CALCRECT );

  //  clean up old fonts
  if ( pOldFont != nullptr )
  {
    SelectObject( hdc, pOldFont );
  }

  ::ReleaseDC( nullptr, hdc );

  const auto width   = GetSystemMetrics( SM_CXFULLSCREEN ) / 2;
  const auto height  = GetSystemMetrics( SM_CYFULLSCREEN ) / 2;

  int widthM   = abs( r.right - r.left);
  int heightM  = abs( r.bottom - r.top);

  //  pad a little
  widthM  += 2 * static_cast<int>(::myodd::config::Get(L"commands\\pad.x", 2));
  heightM += 2 * static_cast<int>(::myodd::config::Get(L"commands\\pad.y", 2));
  
  //  move the window to the top left hand corner 
  SetWindowPos(GetSafeHwnd(),
                HWND_TOPMOST,
                (width - widthM/2), 
                (height - heightM/2), 
                (widthM), 
                (heightM), 
                SWP_HIDEWINDOW );

  // save the full rectangle
  _completeRect = { 0,0, widthM, heightM };
}

/**
 * \brief Check if anything is running
 * \return if we are still busy
 */
bool MessageDlg::IsRunning() const
{
  return _worker.Running();
}

/**
 * \brief the timer to fade the window from time to time.
 * \param owner the timer event number
 */
void MessageDlg::Fade(MessageDlg* owner )
{
  // make it thread safe
  try
  {
    owner->DoFade();
  }
  catch ( ... )
  {
    // ignore the errors.
    // something broke ... somewhere.
  }

  // close it
  owner->CloseFromThread();
}

/**
 * \brief do the actual fading.
 */
void MessageDlg::DoFade() const
{
  // if we do not want to show the message, we might as well hide it right away.
  if ((_totalMilisecondsToShowMessage+_elapseMiliSecondsBeforeFadeOut) <= 0)
  {
    return;
  }

  // where we want the transparancy to start from, (we will end at zero)
  const auto startTransparency = GetStartTransparency();

  // if we want to show the message for a total of _totalMilisecondsToShowMessage
  // but we want to wait for _elapseMiliSecondsBeforeFadeOut before we go
  // how many milliseconds do we have before we need to fade out.
  const auto timeLeftForFadeOutMilliseconds = _totalMilisecondsToShowMessage - _elapseMiliSecondsBeforeFadeOut;

  // and given that we have 'startTransparency' steps to fade out in
  // the number of milliseconds between each steps is ...
  // the sleep time _could_ be zero
  const auto waitTimeBetweenFadeStepsInMilliseconds = static_cast<long>(static_cast<double>(timeLeftForFadeOutMilliseconds) / static_cast<double>(startTransparency));

  // show the message without any fadding.
  for (auto timeToSleptInMilliseconds = 0;
    timeToSleptInMilliseconds < _elapseMiliSecondsBeforeFadeOut;
    timeToSleptInMilliseconds += (waitTimeBetweenFadeStepsInMilliseconds <= 0 ? 1 : waitTimeBetweenFadeStepsInMilliseconds)
    )
  {
    // if we have been asked to stop ... then stop right away.
    if (Stopped())
    {
      return;
    }

    // then show the window a little.
    ShowMessageWithNoFadding(waitTimeBetweenFadeStepsInMilliseconds);
  }
  
  // we can now start fading down toward 'hidden' so the total time
  // should be near our target time of '_totalMilisecondsToShowMessage'
  for (auto currentTransparancy = startTransparency; 
       currentTransparancy > 0; 
       --currentTransparancy)
  {
    // if we have been asked to stop ... then stop right away.
    if (Stopped())
    {
      return;
    }

    // set the transparancy
    SetTransparency(currentTransparancy);

    // show the message a little bit more.
    ShowMessageWithNoFadding(waitTimeBetweenFadeStepsInMilliseconds);
  }
}

void MessageDlg::ShowMessageWithNoFadding(const long milliseconds) const
{
  if (milliseconds <= 0)
  {
    return;
  }

  // sleep a little.
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

  // is the window still visible?
  // if not, then there is no point in going any further.
  if (0 == ::GetWindowLongPtr(GetSafeHwnd(), GWLP_HWNDPARENT))
  {
    return;
  }

  // pump the message
  myodd::wnd::MessagePump( GetSafeHwnd() );
}

/**
 * \brief we cannot close the window from inside a thread
 *        so we post a message here, so the owning thread can call destroy window.
 */
void MessageDlg::CloseFromThread()
{
  CommonWnd::Close();

  // close it
  //_worker.WaitForAllWorkers([&]()
  // {
  //   myodd::wnd::MessagePump(GetSafeHwnd());
  //   myodd::wnd::MessagePump(nullptr);
  //   return true;
  // });
}

/**
 * \brief function called every time we are repainting the window.
 * \see CDialog::OnPaint
 */
void MessageDlg::OnPaint()
{
  const auto mainHdc = GetDC(GetSafeHwnd());
  const auto myDC = new ActionMonitorMemDC(CDC::FromHandle(mainHdc ));
  const auto hdc = myDC->GetSafeHdc();

  try
  {
    // show the background
    RedrawBackground(hdc);

    // the text
    RedrawMessage(hdc);
  }
  catch(...)
  {
    // something broke
    // but we must still free thr resources.
  }
  
  delete myDC;
  ReleaseDC(GetSafeHwnd(), mainHdc );
}

/**
 * \brief draw the message in the device context.
 * \param hdc the handle to the device context we are working in.
 */
void MessageDlg::RedrawMessage(const HDC hdc)
{
  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont(hdc, 35 );

  RECT r = { 0,0,0,0 };
  myodd::html::html(hdc, _mStdMessage.c_str(), _mStdMessage.length(), &r, DT_DEFAULT | DT_CALCRECT);

  //  pad a little
  r.left += static_cast<int>(::myodd::config::Get(L"commands\\pad.x", 2));
  r.top += static_cast<int>(::myodd::config::Get(L"commands\\pad.y", 2));

  myodd::html::html(hdc, _mStdMessage.c_str(), _mStdMessage.length(), &r, DT_DEFAULT);

  //  clean up old fonts
  if (pOldFont != nullptr)
  {
    SelectObject(hdc, pOldFont);
  }
}

/**
 * \brief draw the background in the device context.
 *        this is the un-faded background.
 *        the fade wnd will do the fading for us.
 * \param hdc the handle to the device context we are working in.
 */
void MessageDlg::RedrawBackground( const HDC hdc) const
{
  SetBkMode(hdc, TRANSPARENT);

  const auto colour = ::GetSysColor(COLOR_3DFACE);
  const auto hBrush = CreateSolidBrush(colour);
  FillRect(hdc, &_completeRect, hBrush);

  DeleteObject(hBrush);
}


/**
 * \brief initialise the dialog.
 */
bool MessageDlg::OnInitDialog()
{
  // call the parent now
  if( !CommonWnd::OnInitDialog() )
  {
    return false;
  }

  // hide the window right way
  // fading will re-show the window.
  ShowWindow(GetSafeHwnd(), SW_HIDE);

  // set the window style
  SetWindowLong(GetSafeHwnd(), GWL_STYLE, DS_SETFONT | WS_POPUP );

  auto style = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED;
  style &= ~WS_EX_APPWINDOW;
  SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, style ); //  no taskbar!

  // show/hide for the style to take effect.
  ShowWindow(GetSafeHwnd(), SW_SHOW);
  ShowWindow(GetSafeHwnd(), SW_HIDE);

  // tell the fade window who the parent window is.
  SetFadeParent( GetSafeHwnd() );

  // move the window at the right place/size
  InitWindowPos();

  // all done.
  return true;
}

/**
 * \brief start the thread to fade the window.
 */
void MessageDlg::Show()
{
  // fade the window a little.
  ShowWindow(GetSafeHwnd(), SW_SHOW);
  SetTransparency( GetStartTransparency() );

  // start the worker.
  _worker.QueueWorker(&MessageDlg::Fade, this);
}

/**
 * \brief get the start transparency and make sure that it falls within range.
 * \return safe start transparency
 */
unsigned char MessageDlg::GetStartTransparency()
{
  const auto transparency = ::myodd::config::Get(L"commands\\transparency", DEFAULT_TRANSPARENCY);
  if( transparency < 0 )
  {
    return DEFAULT_TRANSPARENCY;
  }
  return transparency > MAX_TRANSPARENCY ? DEFAULT_TRANSPARENCY : transparency;
}

/**
 * \brief kill the window and wait for it to complete.
 *        this is a static function so we can call it with workers.
 * \param owner the owner of the window we are closing.
 */
void MessageDlg::Close(MessageDlg* owner)
{
  try
  {
    owner->FadeCloseWindow();
  }
  catch(...)
  {
    // it is posible that the deleted window
    // tried to close the message
  }
}

/**
 * \brief kill the window and wait for it to complete.
 */
void MessageDlg::FadeCloseWindow()
{
  // are we done already?
  if ( !IsRunning() )
  {
    // close the window.
    CloseFromThread();
    return;
  }

  // stop all
  Stop();

  // pump the remaining messages

  // wait for all the workers to finish.
  _worker.WaitForAllWorkers([&]()
  {
    myodd::wnd::MessagePump(GetSafeHwnd() );
    if (GetSafeHwnd() != nullptr)
    {
      myodd::wnd::MessagePump(nullptr);
    }
    return true;
  });
}