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
#include "FadeWnd.h"

/**
 * \brief the constructor
 */
FadeWnd::FadeWnd() :
  m_byteVisible( 0 ),
  m_hFade( nullptr ),
  fontDisplay( nullptr ),
  m_stop( false )
{
}

/**
 * \brief the destructor
 */
FadeWnd::~FadeWnd()
{
  if( fontDisplay )
  {
    fontDisplay->DeleteObject();
    delete fontDisplay;
  }
}

/**
 * \brief set the handle of this fading window.
 * \param hFade the handle that is fading. 
 */
void FadeWnd::SetFadeParent( const HWND hFade )
{
  m_hFade = hFade;
}

#define WS_EX_LAYERED 0x00080000 
#ifndef LWA_COLORKEY
  #define LWA_COLORKEY 0x00000001 // Use color as the transparency color.
#endif

#ifndef LWA_COLORKEY
  #define LWA_ALPHA    0x00000002 // Use bAlpha to determine the opacity of the layer
#endif

/**
 * Some code to set the background transparent
 * It ranges from 0 -> 255 
 *
 * @version 0.1
 * @return none
 */
typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
static lpfn g_pSetLayeredWindowAttributes = NULL;

/**
 * \brief set the current transparency of this window.
 * \param bTrans the transparency amount.
 */
void FadeWnd::SetTransparency(unsigned char bTrans)
{
  //  get the function pointer for SetLayeredWindowAttributes 
  //  in User32.dll
  if( g_pSetLayeredWindowAttributes == NULL  )
  {
    const auto hUser32 = GetModuleHandle(_T("USER32.DLL"));
    g_pSetLayeredWindowAttributes = reinterpret_cast<lpfn>(GetProcAddress(hUser32, "SetLayeredWindowAttributes"));
  }
  
  //  if this was not created properly then we need to do it again
  if( nullptr == g_pSetLayeredWindowAttributes )
  {
    return;
  }
  ::SetWindowLong(m_hFade, GWL_EXSTYLE, GetWindowLong(m_hFade, GWL_EXSTYLE) | WS_EX_LAYERED);
  //  range of opacity between 0 and 255
  g_pSetLayeredWindowAttributes(m_hFade, RGB(255,255,255), (BYTE)bTrans, LWA_COLORKEY | LWA_ALPHA);
}


/**
 * Create and select the font that we want to use
 * Remember that the calling function must re-select the object returned.
 *
 * @param HANDLE the device context we are using to load the font.
 * @return HANDLE of the font currently been used, it is good practice to restore it once done.
 */
HGDIOBJ FadeWnd::SelDisplayFont( HDC hdc, UINT fontSize /*= 70*/  )
{
  if( NULL == fontDisplay )
  {
	  LOGFONT logFont;
	  memset(&logFont, 0, sizeof(LOGFONT));
	  logFont.lfHeight	=	fontSize;
	  logFont.lfWeight	=	FW_NORMAL;
	  logFont.lfItalic	=	FALSE;
	  lstrcpy(logFont.lfFaceName, _T("Veranda") ); 

    fontDisplay = new CFont();
	  if (!fontDisplay->CreateFontIndirect(&logFont))
    {
		  return NULL;
	  }
  }

  HGDIOBJ pOldFont = ::SelectObject(hdc, *fontDisplay );
  return pOldFont;
}

/**
 * \brief the fading window message pump
 * \param hWnd the window busy fading
 */
void FadeWnd::MessagePump( const HWND hWnd )
{
  //  lock up to make sure we only do one at a time
  MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{         
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    if ( nullptr != hWnd && 0 == ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
    {
      break;
    }
	}
}
