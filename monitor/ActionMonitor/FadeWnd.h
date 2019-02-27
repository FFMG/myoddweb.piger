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

  void SetTransparency( unsigned char bTrans ) const;

  CFont *fontDisplay; //  the display font

  //
  void Stop() {
    m_stop = true;
  }
  bool m_stop;
  bool Stopped() const {
    return m_stop;
  }
};
