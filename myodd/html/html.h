#pragma once
/* DrawHTML2()
* Drop-in replacement for DrawText() supporting a tiny subset of HTML.
*/

namespace myodd{ namespace html{
  SIZE html(HDC     hdc,        // handle of device context
            const wchar_t* lpString,   // address of string to draw
            int     nCount,     // string length, in characters
            LPRECT  lpRect,     // address of structure with formatting dimensions
            UINT    uFormat     // text-drawing flags
            );
  } //  html
} //  myodd
