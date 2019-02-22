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

IMPLEMENT_DYNAMIC(MessageDlg, CDialog)

/**
 * \brief the constructor
 * \see CDialog::CDialog
 */
MessageDlg::MessageDlg()
  : CDialog(MessageDlg::IDD, nullptr),
    FadeWnd(), 
    _mNFadeOut(0),
    _mNElapse(0)
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
  ON_WM_CLOSE()
END_MESSAGE_MAP()

/**
 * \brief the dialog is about to destroy itself.
 * \see CDialog::PostNcDestroy
 */
void MessageDlg::PostNcDestroy()
{
  delete this;
}

/**
 * \brief called when the window is being created, we can then set our own values.
 * \see CDialog::Create
 * \param sText the text we want to display
 * \param nElapse how long we want to display the message for
 * \param nFadeOut where the fade start from.
 */
void MessageDlg::Create(const std::wstring& sText, const int nElapse, const int nFadeOut)
{
  //  save the value
  _mNFadeOut = nFadeOut;
  _mStdMessage = sText;
  _mNElapse = nElapse;

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
void MessageDlg::DoFade()
{
  // 0 means that we move right away
  if (_mNFadeOut <= 0)
  {
    return;
  }

  const auto bStart = ::myodd::config::Get(L"commands\\transparency", 127);
  for (auto b = bStart; b > 0; --b)
  {
    Transparent(b);
    const auto d = (GetTickCount() + _mNFadeOut);
    while (d > GetTickCount())
    {
      if (0 == ::GetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT))
      {
        break;
      }

      //  are we stopping everything?
      // then bail out now!
      if (Stopped())
      {
        return;
      }
      MessagePump(m_hWnd);

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      MessagePump(nullptr);
    }
  }
}

/**
 * \brief we cannot close the window from inside a thread
 *        so we post a message here, so the owning thread can call destroy window.
 */
void MessageDlg::CloseFromThread()
{
  // call the complete function.
  // so we can tell all that we are done here.
  _onComplete(this);

  // close it
  PostMessage(WM_CLOSE);

  // complete whatever needs to be done
  MessagePump(m_hWnd);

  // wait a little.
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // and let the other threads process their messages.
  MessagePump(nullptr);
}

/**
 * \brief called when the dialog is closing, (this is the main app).
 * \see CTrayDialog::OnClose
 */
void MessageDlg::OnClose()
{
  DestroyWindow();
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
  Transparent( ::myodd::config::Get( L"commands\\transparency", 127) );

  // save the complete callback function
  _onComplete = onComplete;

  // start the worker.
  _worker.QueueWorker(&MessageDlg::Fade, this);
}

/**
 * \brief kill the window and wait for it to complete.
 *        this is a static function so we can call it with workers.
 * \param owner the owner of the window we are closing.
 */
void MessageDlg::Close(MessageDlg* owner)
{
  owner->FadeCloseWindow();
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
  MessagePump( m_hWnd );
  MessagePump( nullptr );

  // then wait for everything to finish.
  _worker.WaitForAllWorkers();
}