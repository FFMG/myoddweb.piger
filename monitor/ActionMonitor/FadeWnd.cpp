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
  _hFadeWindowHandle( nullptr ),
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
  _hFadeWindowHandle = hFade;
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
static lpfn g_pSetLayeredWindowAttributes = nullptr;

/**
 * \brief set the current transparency of this window.
 * \param bTrans the transparency amount.
 */
void FadeWnd::SetTransparency(const unsigned char bTrans) const
{
  //  get the function pointer for SetLayeredWindowAttributes 
  //  in User32.dll
  if( g_pSetLayeredWindowAttributes == nullptr  )
  {
    const auto hUser32 = GetModuleHandle(_T("USER32.DLL"));
    g_pSetLayeredWindowAttributes = reinterpret_cast<lpfn>(GetProcAddress(hUser32, "SetLayeredWindowAttributes"));
  }
  
  //  if this was not created properly then we need to do it again
  if( nullptr == g_pSetLayeredWindowAttributes )
  {
    return;
  }
  ::SetWindowLong(GetFadeParent(), GWL_EXSTYLE, GetWindowLong(GetFadeParent(), GWL_EXSTYLE) | WS_EX_LAYERED);
  
  //  range of opacity between 0 and 255
  g_pSetLayeredWindowAttributes(GetFadeParent(), RGB(255,255,255), static_cast<BYTE>(bTrans), LWA_COLORKEY | LWA_ALPHA);
}

/**
 * \brief set the font we will be using to display the message
 * \param hdc the device context handle
 * \param fontSize the size of the font.
 * \return the created font.
 */
HGDIOBJ FadeWnd::SelDisplayFont( const HDC hdc, const UINT fontSize  )
{
  if( nullptr == fontDisplay )
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
		  return nullptr;
	  }
  }
  
  return ::SelectObject(hdc, *fontDisplay );
}
