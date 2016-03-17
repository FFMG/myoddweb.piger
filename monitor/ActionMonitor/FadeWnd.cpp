#include "stdafx.h"
#include "FadeWnd.h"

/**
 * Todo
 * @param void
 * @return void
 */
FadeWnd::FadeWnd(void) :
  m_byteVisible( 0 ),
  m_hFade( NULL ),
  fontDisplay( NULL ),
  m_stop( false )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
FadeWnd::~FadeWnd(void)
{
  if( fontDisplay )
  {
    fontDisplay->DeleteObject();
    delete fontDisplay;
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
void FadeWnd::SetFadeParent( HWND hFade )
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
 * Todo
 * @param void
 * @return void
 */
void FadeWnd::Transparent( BYTE bTrans )
{
  //  get the function pointer for SetLayeredWindowAttributes 
  //  in User32.dll
  if( g_pSetLayeredWindowAttributes == NULL  )
  {
    HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
    g_pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32,"SetLayeredWindowAttributes");
  }
  
  //  if this was not created properly then we need to do it again
  if( NULL == g_pSetLayeredWindowAttributes )
  {
    return;
  }
  ::SetWindowLong(m_hFade, GWL_EXSTYLE, GetWindowLong(m_hFade, GWL_EXSTYLE) | WS_EX_LAYERED);
  //  range of opacity between 0 and 255
  g_pSetLayeredWindowAttributes(m_hFade, RGB(255,255,255),   (BYTE)bTrans, LWA_COLORKEY | LWA_ALPHA);
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
 * Todo
 * @param void
 * @return void
 */
void FadeWnd::MessagePump(  HWND hWnd )
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