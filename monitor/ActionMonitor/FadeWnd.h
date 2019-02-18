#pragma once

//  flicker free dc.
#include "MemDC.h"
#include <mutex>

#define DT_DEFAULT ( DT_NOCLIP | DT_LEFT )

class FadeWnd 
{
public:
  FadeWnd( );
  virtual ~FadeWnd(void);

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
  static void MessagePump( HWND hWnd );
  void Stop() {
    m_stop = true;
  }
  bool m_stop;
  bool Stopped() const {
    return m_stop;
  }
};
