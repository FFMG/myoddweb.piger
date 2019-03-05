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

#define MAX_TRANSPARENCY (unsigned char)255
#define DEFAULT_TRANSPARENCY (unsigned char)127

IMPLEMENT_DYNAMIC(MessageDlg, CDialog)

/**
 * \brief the constructor
 * \see CDialog::CDialog
 */
MessageDlg::MessageDlg()
  : CDialog(MessageDlg::IDD, nullptr),
    FadeWnd(), 
    _totalMilisecondsToShowMessage(0),
    _elapseMiliSecondsBeforeFadeOut(0),
    _onComplete(nullptr)
{
}

MessageDlg::~MessageDlg()
{
}

/**
 * Todo
 * \see CDialog::DoDataExchange
 * \param pDX the data exchange being processed.
 * \return void
 */
void MessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MessageDlg, CDialog)
  ON_WM_PAINT()
END_MESSAGE_MAP()

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

  __super::Create( MessageDlg::IDD );
}

/**
 * \brief Initialise the fading window.
 */
void MessageDlg::InitWindowPos()
{
  //  get the size of the windows
  const auto hdc = ::GetDC( m_hWnd );

  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont( hdc );

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
  SetWindowPos( &wndTopMost, 
                (width - widthM/2), 
                (height - heightM/2), 
                (widthM), 
                (heightM), 
                SWP_HIDEWINDOW );
}

/**
 * \brief set the font we will be using to display the message
 * \param hdc the device context handle
 * \param fontSize the size of the font.
 * \return the created font.
 */
HGDIOBJ MessageDlg::SelDisplayFont(const HDC hdc, UINT fontSize )
{
  // TODO replace magic number.
  return __super::SelDisplayFont( hdc, 35 );
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
  if (0 == ::GetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT))
  {
    return;
  }

  // pump the message
  myodd::wnd::MessagePump(m_hWnd);
}

/**
 * \brief we cannot close the window from inside a thread
 *        so we post a message here, so the owning thread can call destroy window.
 */
void MessageDlg::CloseFromThread()
{
  // call the complete function.
  // so we can tell all that we are done here.
  if (_onComplete != nullptr)
  {
    _onComplete(this);
  }

  // close it
  if (m_hWnd != nullptr)
  {
    EndDialog(IDOK);
  }
}

/**
 * \brief function called every time we are repainting the window.
 * \see CDialog::OnPaint
 */
void MessageDlg::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  const auto hdc = dc.GetSafeHdc();
  //  get the font that we will be using for display
  //  because of the XML config this is only really used for size.
  const auto pOldFont = SelDisplayFont( hdc );

  RECT r = {0,0,0,0};
  myodd::html::html(hdc, _mStdMessage.c_str() , _mStdMessage.length(), &r, DT_DEFAULT | DT_CALCRECT );

  //  pad a little
  r.left += static_cast<int>(::myodd::config::Get(L"commands\\pad.x", 2));
  r.top  += static_cast<int>(::myodd::config::Get(L"commands\\pad.y", 2));

  SetBkMode( hdc, TRANSPARENT );
  myodd::html::html(hdc, _mStdMessage.c_str() , _mStdMessage.length(), &r, DT_DEFAULT );

  //  clean up old fonts
  if ( pOldFont != nullptr )
  {
    SelectObject( hdc, pOldFont );
  }
}

/**
 * \brief initialise the dialog.
 * \see CDialog::OnInitDialog
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
 * \brief start the thread to fade the window.
 * \param onComplete the callback function we want to call when the window is complete
 */
void MessageDlg::Show(std::function<void(CWnd*)> onComplete)
{
  // fade the window a little.
  ShowWindow(SW_SHOW);
  SetTransparency( GetStartTransparency() );

  // save the complete callback function
  _onComplete = onComplete;

  // start the worker.
  _worker.QueueWorker(&MessageDlg::Fade, this);
}

/**
 * \brief get the start transparency and make sure that it falls within range.
 * \return safe start transparency
 */
unsigned char MessageDlg::GetStartTransparency() const
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
    myodd::wnd::MessagePump(m_hWnd );
    if (m_hWnd != nullptr)
    {
      myodd::wnd::MessagePump(nullptr);
    }
    return true;
  });
}