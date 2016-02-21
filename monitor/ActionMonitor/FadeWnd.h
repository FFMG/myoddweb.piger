#pragma once

//  flicker free dc.
#include "MemDC.h"

#define DT_DEFAULT ( DT_NOCLIP | DT_LEFT )

class FadeWnd 
{
public:
  FadeWnd( );
  ~FadeWnd(void);

  void SetFadeParent( HWND hFade );
  HWND GetFadeParent( ) const{ return m_hFade; }

  virtual HGDIOBJ SelDisplayFont( HDC hdc, UINT fontSize = 70 );
protected:
  HWND m_hFade;

protected:
  BYTE m_byteVisible;
  inline BYTE IsVisible()const{return m_byteVisible;};
  inline void Visible( BYTE b ){m_byteVisible = b;};

  void Transparent( BYTE bTrans );

  CFont *fontDisplay; //  the display font

  //
  void MessagePump( HWND hWnd );
};
