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

#define DT_DEFAULT ( DT_NOCLIP | DT_LEFT )

class FadeWnd 
{
public:
  FadeWnd( );
  virtual ~FadeWnd(void);

  void SetFadeParent( HWND hFade );
  HWND GetFadeParent( ) const{ return _hFadeWindowHandle; }

protected:
  /**
   * \brief the window that is being faded.
   */
  HWND _hFadeWindowHandle;

  HGDIOBJ SelDisplayFont(HDC hdc, UINT fontSize);

protected:
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
