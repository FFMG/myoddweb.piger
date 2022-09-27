/* DrawHTML()
* Drop-in replacement for DrawText() supporting a tiny subset of HTML.
*/
#include "../common/includes.h"
#include "../string/string.h"
#include "../math/math.h"
#include <assert.h>
#include "html.h"
#include "parser.h"

namespace myodd{ namespace html{
  /**
   * Draw a single line of html.
   * @param HDC the device context handle.
   * @param Parser& the parser data.
   * @param Parser::HTML_CONTAINER::const_iterator the beginning of our parsed html container.
   * @param Parser::HTML_CONTAINER::const_iterator the end of our parser html container.
   * @param LPRECT the rectangle where we want to draw, or the size of the rectangle.
   * @param int if known, the maximum height /working area, (for bottom/top adjusting) -1 if unknown
   * @param UINT the format on how we would like to display the text.
   * @return SIZE the height and width the the output text.
   */
  SIZE _htmlSingleLine(HDC     hdc,        // handle of device context
                       Parser& parser,
                       Parser::HTML_CONTAINER::const_iterator begin,
                       Parser::HTML_CONTAINER::const_iterator end,
                       LPRECT  lpRect,     // address of structure with formatting dimensions
                       const int maxLineHeight,
                       const int paddingTop,
                       const int paddingBottom,
                       UINT    uFormat     // text-drawing flags
                      )
  {
    //
    // do special cases.
    // the html function draws top/left so we need to first do that
    // and get the size then realign accordingly,
    if( (uFormat & DT_RIGHT) != 0 || (uFormat & DT_CENTER) != 0)
    {
      UINT uFormatOrg = uFormat;
      RECT rTopLeft = {0};
      memcpy( &rTopLeft, lpRect, sizeof(RECT));

      // change the format to left, as this is where we will be drawing
      uFormat |= DT_LEFT;
      uFormat &= ~(DT_RIGHT|DT_CENTER);

      // get the size of this LEFT aligned rectangle.
      const auto sizeRight = _htmlSingleLine( hdc, parser, begin, end, &rTopLeft, maxLineHeight, paddingTop, paddingBottom, uFormat|DT_CALCRECT);

      if( (uFormatOrg & DT_RIGHT) != 0 )
      {
        // then, move as much right as possible so we can draw left of it.
        // that will give the impression of been right.
        lpRect->left = lpRect->right - sizeRight.cx;
      }
      else
      {
        // then center the line
        // we move to the center less half of the new width
        lpRect->left += ((lpRect->right - lpRect->left) / 2) - ((sizeRight.cx)/2);
      }
    }

    // this is the rect we will be drawing in.
    // anytime we draw any text we will resize this rectangle 
    // until it has been all used up.
    RECT drawRect = {0};
    memcpy( &drawRect, lpRect, sizeof(RECT) );

    // the return Height/Width of or text.
    SIZE size = {0};

    for( Parser::HTML_CONTAINER::const_iterator it = begin;
      it != end;
      ++it )
    {
      const auto thisSize = parser.Apply( hdc, *it, drawRect, *lpRect, maxLineHeight, paddingTop, paddingBottom, uFormat );
      size.cx += thisSize.cx;                             // all in one line so we are moving along
      size.cy = myodd::math::max2( size.cy, thisSize.cy); // all in one line so we are using the biggest height.
    }

    // do we need to set the size of the rectangle?
    // remember that this can never be right or center
    // so our rectangle is always top/left
    if( (uFormat & DT_CALCRECT) != 0 )
    {
      lpRect->left = lpRect->right - size.cx;
      lpRect->bottom = lpRect->top + size.cy;
    }

    // the size of what was drawn
    if( size.cx > 0 )
    {
      size.cx += 1;
    }
    return size;
  }

  /**
   * Output HTML formated text to the screen.
   * @param HDC the device context handle.
   * @param const wchar_t* the string we want to output.
   * @param int the number of characters we want to display.
   * @param LPRECT the rectangle where we want to draw, or the size of the rectangle.
   * @param int the maximum height per line at the 'maximum' font size.
   * @param UINT the format on how we would like to display the text.
   * @return SIZE the height and width the the output text.
   */
  SIZE html(HDC     hdc,        // handle of device context
            const wchar_t* lpString,   // address of string to draw
            int     nCount,     // string length, in characters
            LPRECT  lpRect,     // address of structure with formatting dimensions
            const int maxLineHeight,
            const int paddingTop,
            const int paddingBottom,
            UINT    uFormat     // text-drawing flags
          )
  {
    // do some basic validation in case the string is NULL
    if( !lpString )
    {
      SIZE s = {0};
      return s;
    }

    // where will we be drawing the text???
    assert( lpRect );

    // set the length of the text if the user did not pass it.
    nCount = nCount < 0 ? _tcslen( lpString ) : nCount;

    int vOffset = 0;
    // horizontal alignments are handled in the htmlSingle(...) function
    // but vertical alignments are handled here.
    if( (uFormat & DT_VCENTER) != 0 || (uFormat & DT_BOTTOM) != 0)
    {
      RECT rTopLeft = {0};
      memcpy( &rTopLeft, lpRect, sizeof(RECT));

      UINT uFormatOrg = uFormat;
      // change the format to left, as this is where we will be drawing
      // we also removed the right/center flags as our main concern is the height.
      uFormat &= ~(DT_VCENTER|DT_BOTTOM);

      // get the size of this LEFT aligned rectangle.
      // we pass -1 as a max height as we do not care about the height when calculating rectangle.
      const auto sizeRight = html( hdc, lpString, nCount, &rTopLeft, -1,-1, -1, (uFormat|DT_LEFT|DT_CALCRECT) & ~(DT_RIGHT|DT_CENTER));

      if( (uFormatOrg & DT_BOTTOM) != 0 )
      {
        // then, move as much right as possible so we can draw left of it.
        // that will give the impression of been right.
        vOffset = (lpRect->bottom - sizeRight.cy) - lpRect->top;
      }
      else
      {
        // then vcenter the rectangle.
        // we move to the center less half of the new width
        vOffset = ((lpRect->bottom - lpRect->top) / 2) - (sizeRight.cy/2);
      }
    }

    RECT returnRect = {0};
    memcpy( &returnRect, lpRect, sizeof(RECT));

    // parse all the html code. This will return a vector of all the text and tags in the right order.
    Parser parser( hdc );
    const Parser::HTML_CONTAINER& dom = parser.Parse( lpString, nCount );

    SIZE size = {0};
    auto& begin = dom.begin();
    const auto& last = (begin == dom.end()) ? dom.end() : (dom.end() -1);
    for( auto it = dom.begin();
         it != dom.end();
         ++it 
         )
    {
      const auto hd = (*it);
      if( !hd->HasTokenData() || !hd->TokenData().ToNextLine(hd->IsEnd()))
      {
        if(  it == last )
        {
          ++it;
        }
        else
        {
          continue;
        }
      }

      // make sure that the top of the rectangle is below the last line
      // and we also include the offset.
      auto thisRect = returnRect;
      thisRect.top += (size.cy + vOffset);

      // draw the item
      auto s = _htmlSingleLine( hdc, parser, begin, it, &thisRect, maxLineHeight, paddingTop, paddingBottom, uFormat);
      if( s.cy == 0 )
      {
        SIZE h = {0};
        GetTextExtentPoint32(hdc, _T(" "), 1, &h);
        s.cy = h.cy;
      }

      // the new width
      size.cx = s.cx > size.cx? s.cx : size.cx; //  new width
      size.cy+= s.cy;                           //  new height.

      //  make sure that the new height does not take us past our maximum height.
      if( (lpRect->bottom - lpRect->top) > 0 && size.cy > (lpRect->bottom - lpRect->top))
      {
        // no need to go further if we are already in the edge of the rectangle.
        size.cy = (lpRect->bottom - lpRect->top);
        break;
      }

      // this is the new beginning
      begin = it;

      if( it == dom.end() )
        break;
    }

    // clear all the fonts/colors
    parser.DeInit( hdc );

    // do we need to set the size of the rectangle?
    if( (uFormat & DT_CALCRECT) != 0 )
    {
      if( (uFormat & DT_RIGHT) != 0 )
      {
        lpRect->left = lpRect->right - size.cx;
      }
      else if( (uFormat & DT_CENTER) != 0 )
      {
        int w = (lpRect->right-lpRect->left)/2;
        lpRect->left  = lpRect->left + w - int(size.cx/2);
        lpRect->right = lpRect->left + size.cx + (size.cx % 2);
      }
      else
      {
        lpRect->right = lpRect->left + size.cx;
      }

      lpRect->top += vOffset;
      lpRect->bottom = lpRect->top + size.cy;
    }
    return size;
  }

} //  html
} //  myodd