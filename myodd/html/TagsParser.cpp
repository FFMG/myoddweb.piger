#include "TagsParser.h"
#include "AttributesParser.h"
#include <algorithm>
#include "SimpleHtmlData.h"

namespace myodd { namespace html {
TagsParser::TagsParser( ) :
  _saveDC( -1 ),
  _font( nullptr )
{
}

TagsParser::~TagsParser()
{
  assert( _font == nullptr ); //  forgot to call deinit?
  Clear();
}

/**
 * \brief Delete all the tags/text that we have/
 */
void TagsParser::Clear()
{
  for (auto it = m_data.begin(); it != m_data.end(); ++it)
  {
    delete *it;
  }
  m_data.clear();
}

/**
 * \brief Parse a string and build an array of HTML tags/text and so on.
 * \param const std::wstring the string we are parsing.
 * \return const TagsParser::HTML_CONTAINER& a container with all html the tags and text.
 */
const TagsParser::HtmlDataContainer& TagsParser::Parse(const std::wstring& text)
{
  return Parse(text.c_str());
}

/**
 * \brief Parse a string and build an array of HTML tags/text and so on.
 * \param lpString the string we are parsing.
 * \return const TagsParser::HTML_CONTAINER& a container with all html the tags and text.
 */
const TagsParser::HtmlDataContainer& TagsParser::Parse(const wchar_t* lpString )
{
  Clear();
  if( nullptr == lpString )
  {
    return Tree();
  }

  // use the len of the string
  auto limit = lpString+ _tcslen(lpString); // the full string
  auto ptr = lpString; // the start
  auto begin  = ptr;
  auto end    = ptr;
  while (*ptr && end != limit ) 
  {
    // look for the next opening tag '<'
    end = FindTag(begin, '<');
    if (end != nullptr)
    {
      // assign the value from wherever we come from to here.
      // if the tag is the very first one, then there is no need to assign that value.
      end = end > limit ? limit : end;
      AddNonHtmlTag( begin, end );
      begin = end;

      // look for the closing tag.
      // if we have reached our limit no need to look further.
      if ( begin != limit )
      {
        end = FindTagExcluding(begin, '>', '<');
        if (end != nullptr)
        {
          if (begin[end - begin] == '>')
          {
            ++end;  //  include the closing tag as we know we will be using it.
            end = end > limit ? limit : end;

            // that tag was what we were looking for
            // so we can indeed include it as a valid html tag.
            AddHtmlTag(begin, end);
          }
          else
          {
            // we do not include the 'invalid' tag here.
            end = end > limit ? limit : end;

            // there was an invalid tag before the closing tag
            // so we cannot use it, just add it as a non tag
            AddNonHtmlTag(begin, end);
          }
          ptr = begin = end;
        }
        else
        {
          // we just don't have an end tag
          // this is a bit malformated but we can include it.
          AddHtmlTag( begin, limit );
          break;
        }
      }
    }
    // no more attribute, just go to the end,
    else
    {
      AddNonHtmlTag( begin, limit );
      break;
    }
  }
  return Tree();
}

/**
 * \brief look for a tag insde the given wchar
 * \param const wchar_t* the body we will be looking for the tag in
 * \param const wchar_t the tag we are looking for
 */
const wchar_t* TagsParser::FindTag(const wchar_t* body, const wchar_t tag) const
{
  return _tcschr(body, tag);
}

const wchar_t* TagsParser::FindTagExcluding(const wchar_t* body, const wchar_t tag, const wchar_t exclude) const
{
  // find the start tag
  // if it does not exist then the exclude does not really matter.
  const auto findTag = FindTag(body, tag);
  if (nullptr == findTag)
  {
    return nullptr;
  }

  // now look for the 'exclude' tag
  // if it does not exist then there is nothing to worry about.
  // we can use the tag we found earlier
  const auto excludeTag = FindTag(body+1, exclude);
  if (nullptr == excludeTag)
  {
    return findTag;
  }

  // we have to be carefull here, we have both our tag
  // if the excluded tag is after the tag, then we can return the tag.
  // if the excluded tag is first, then we have to return null.
  if (excludeTag < findTag)
  {
    auto lastKnown = excludeTag;
    for (;;)
    {
      const auto excludeTagAgain = FindTag(lastKnown +1, exclude);
      if (excludeTagAgain != nullptr && excludeTagAgain < findTag)
      {
        lastKnown = excludeTagAgain;
        continue;
      }
      return lastKnown;
    }
  }
  return findTag;
}

/**
 * \brief Add a string to the array of HTMLDATA 
 * \param begin the start of the string
 * \param end the end of the string 
 */
void TagsParser::AddHtmlTag(const wchar_t* begin, const wchar_t* end)
{
  // an empty string, no need to do it.
  if (begin == end)
    return;
    
  auto originalString = std::wstring( begin, end );

  Attributes attributes;

  assert(*begin == '<');
  begin++;    //  '<'

  if (*(end - 1) != '>')
  {
    AddNonHtmlTag(originalString);
    return;
  }

  int tagType = { Tag::None };
  end--;      //  '>'
  if (*begin == L'/' ? true : false)
  {
    tagType |= Tag::Closing;
    begin++;  //  '/'
  }
  else
  {
    tagType |= Tag::Opening;
  }

  // check if we are a star/end kind of tag
  // for example <br/> or <br />
  if (*(end - 1) == L'/' ? true : false)
  {
    tagType |= Tag::Closing;
    end--;  //  '/'
  }

  // just get the name skip the attributes.
  const wchar_t* space = _tcschr(begin, ' ');
  if (space && space < end)
  {
    AttributesParser attributesParser;
    attributes = attributesParser.Parse(std::wstring(space + 1, end));
    end = space;
  }

  assert(end - begin > 0);  // how can the tag be empty??
  auto text = std::wstring(end - begin, '\0');
  std::transform(begin, end, text.begin(), ::tolower);

  // look for the matching Tag for that tag
  // the Tag is what does the basic string transformation.
  auto tagData = Tags::CreateFromString(text, attributes, tagType );

  // if we do not have a tag it means that we are not going to parse it properly.
  // the text in the tag will not be displayed.
  // did you mean to use &gt; (>) and &lt; (<)?
  if (nullptr == tagData)
  {
    AddNonHtmlTag(originalString);
    return;
  }

  // add this to the list as a text only item.
  m_data.push_back(new HtmlData(tagData));
  delete tagData;
}

/**
  * \brief Add a string to the array of HTMLDATA
  * \param begin the start of the string
  * \param end the end of the string
  */
void TagsParser::AddNonHtmlTag(const wchar_t* begin, const wchar_t* end)
{
  AddNonHtmlTag(std::wstring(begin, end));
}

void TagsParser::AddNonHtmlTag(const std::wstring& text)
{
  // an empty string, no need to do it.
  if (text.length() == 0 )
    return;

  // this is not a tag so we just assign the text.
  // we might want to do more formating here.
  //
  // special chars are done straight away

  // add this to the list as a text only item.
  m_data.push_back( new SimpleHtmlData(EscapeText(text)));
}

/**
 * \brief Escape a given text
 * \param std::wstring the text we want to excape
 * \return the escaped text
 */
std::wstring TagsParser::EscapeText(const std::wstring& src) const
{
  auto text = src;
  text = myodd::strings::Replace(text, L"&nbsp;", L" ", false);
  text = myodd::strings::Replace(text, L"&lt;", L"<", false);
  text = myodd::strings::Replace(text, L"&gt;", L">", false);
  text = myodd::strings::Replace(text, L"&amp;", L"&", false);
  text = myodd::strings::Replace(text, L"&deg;", std::wstring(1, wchar_t(176)), false);    //  degree
  text = myodd::strings::Replace(text, L"&plusmn;", std::wstring(1, wchar_t(177)), false); //  Plus/minus symbol

  return text;
}

void TagsParser::CalculateSmartDimensions( SIZE& size, HDC hDCScreen, const wchar_t* szText, int nLen )
{
  if( nLen == -1 )
  {
    nLen = szText ? _tcslen(szText) : 0;
  }
  auto& lHeight = size.cy;
  auto& lWidth = size.cx;

  const auto hFont = static_cast<HFONT>(::GetCurrentObject(hDCScreen, OBJ_FONT));
  const auto hDCMem=CreateCompatibleDC(hDCScreen);

  const auto hFontOld=static_cast<HFONT>(SelectObject(hDCMem, hFont));

  //calculate the width of the string using the classic way
  SIZE sizeText;
  GetTextExtentPoint32(hDCMem, szText, nLen, &sizeText);

  //Or calculate like this. But it's no better than using GetTextExtentPoint32
  //RECT rectResult={0,0,0,0};
  //DrawText(hDCMem,szText,-1,&rectResult,DT_CALCRECT|DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
  //sizeText.cx=rectResult.right-rectResult.left;


  lHeight=sizeText.cy;//lHeight==0 if the text is empty, so try GetTextMetrics below!
  lWidth=sizeText.cx;

  if(nullptr==szText || L'\0'==szText[0])
  {
    TEXTMETRIC tm;
    GetTextMetrics(hDCMem,&tm);//this helps if the text is empty!
    lHeight=tm.tmHeight;//no space between the lines, doesn't need +tm.tmExternalLeading;

    SelectObject(hDCMem,hFontOld);
    DeleteDC(hDCMem);
    return;
  }

  // if the last item is a space then we cannot calculate its width
  if( szText[-1+nLen] == L' ')
  {
    SelectObject(hDCMem,hFontOld);
    DeleteDC(hDCMem);
    return;
  }

  //RECT rect={0,0,lWidth+2*size.cx,lHeight};
  // August 17, 2006
  // Set the right-limit of the bounding rectangle to be scanned to just 
  // 'sizeText.cx+widthOfTheLastCharacter', as suggested by 'oupoi'. 

  SIZE sizeLastCharacter;
  GetTextExtentPoint32(hDCMem, &szText[-1+nLen], 1, &sizeLastCharacter);

  //enough to fit the text
  RECT rect={0,0,lWidth+sizeLastCharacter.cx,lHeight};

  const auto hBitmap= CreateCompatibleBitmap ( hDCMem,
    rect.right-rect.left,
    rect.bottom-rect.top);

  const auto hOldBitmap=static_cast<HBITMAP>(SelectObject(hDCMem, hBitmap));

  //fill with white
  FillRect(hDCMem,&rect,static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
  DrawText(hDCMem,szText,nLen,&rect,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);

  auto iXmax=0;
  auto bFound=false;
  for(int x=rect.right-1;x>=0 && !bFound ;x--)	
  {
    for (auto y=0;y<=rect.bottom-1 && !bFound;y++)	
    {

      const auto rgbColor=GetPixel(hDCMem,x,y);

      if(rgbColor!=RGB(255,255,255))
      {
        //found a non-white pixel, save the horizontal position
        //and exit the loop. Job finished.
        iXmax=x;
        bFound=true;
      }
    }//endfor
  }//endfor	

  //here we have the width of the painted text
  lWidth=iXmax+1;//+1 because we use 0-based indexes

  SelectObject(hDCMem,hFontOld);
  SelectObject(hDCMem,hOldBitmap);
  DeleteObject(hBitmap);
  DeleteDC(hDCMem);
}

void TagsParser::Init( const HDC hdc )
{
  // save the dc so we can restore it when we are done.
  _saveDC = SaveDC( hdc );

  // get the current font
  const auto hFont = static_cast<HFONT>(::GetCurrentObject(hdc, OBJ_FONT));
  
  // and the current log font.
  ::GetObject(hFont, sizeof(LOGFONT), &_logFont);
}

void TagsParser::DeInit( const HDC hdc )
{
  if( -1 != _saveDC )
  {
    RestoreDC( hdc, _saveDC );
    _saveDC = -1;
  }

  // delete the old font.
  if( _font != nullptr )
  {
    DeleteObject( _font );
    _font = nullptr;
  }
}

const LOGFONT& TagsParser::GetCurrentLogFont() const 
{
  return _logFont;
}

SIZE TagsParser::Apply( const HDC hdc,
                    HtmlData* hd, 
                    RECT& rect,
                    const RECT& givenRect,
                    const int maxLineHeight,
                    const int paddingTop,
                    const int paddingBottom,
                    const UINT uFormat 
                  )
{
  if (nullptr != hdc && _saveDC == -1)
  {
    Init(hdc);
  }
  SIZE size = {0};

  // are we a tag?
  if( hd->IsHtmlTag() )
  {
    // this function does single lines only
    // but when asked to parse from beginning to end we are 
    // given the tag that caused us to go to the next line
    auto lf = GetCurrentLogFont();
    if( hd->IsClosing() )
    {
      hd->Pop( hdc, lf );
      PopFont( hdc, lf );
    }
    else
    {
      hd->Push(hdc, lf);
      PushFont( hdc, lf );
    }
  }

  // we only output if we are not a string 
  if( hd->TextLength() > 0 && !hd->IsHtmlTag() )
  {
    // in case the user has ellipses we need to use the rectangle 
    // as it was given to us.
    // RECT rc = {0};
    // memcpy( &rc, &rect, sizeof(RECT) );
    // DrawText( hdc, lpString, lpStringLen, &rc, uFormat | DT_CALCRECT );

    if( (uFormat & DT_CALCRECT) == 0 )
    {
      RECT bottomAlignedRect = rect;
      if (maxLineHeight != -1)
      {
        // get the hight of this text ...
        RECT thisRect = { 0 };
        DrawText(hdc, L"IM", 2, &thisRect, uFormat | DT_CALCRECT | DT_SINGLELINE);

        // then bottom align the text
        // we know that the *total* rectangle is equal to "rect"
        // we are drawing from the top/left corner
        // so to find the bottom of this current line we +maxHeight and then -thisRect.bottom
        // the maxHeight will take us to the bottom of the current line
        // and the -thisRect.bottom will bring us back to the "top/left" making us bottom aligned.
        bottomAlignedRect.top += (maxLineHeight - thisRect.bottom);
      }

      // we have now moved our rectangle right at the bottom of the rectange
      // so we need to re-add the padding.
      bottomAlignedRect.top += paddingTop;
      bottomAlignedRect.bottom += (paddingTop+paddingBottom);

      DrawText( hdc, hd->Text().c_str(), hd->TextLength(), &bottomAlignedRect, uFormat);
    }

    SIZE calcSize = {0};
    CalculateSmartDimensions( calcSize, hdc, hd->Text().c_str(), hd->TextLength());

    // given the size of the rectangle, update the various sizes.
    // they will be used later to make sure that we have the right sizes.
    int nWidth = calcSize.cx;
    if(  rect .left+nWidth > rect .right )
    {
      if( givenRect.right - givenRect.left > 0 )
      {
        nWidth =  rect .right - rect .left;
      }
    }
    rect .left += nWidth;
    rect .left = rect .left > rect .right ? rect .right : rect .left;
    size.cx+= nWidth;

    // update the return values.
    int nHeight = calcSize.cy;
    if(  rect .top+nHeight > rect .bottom )
    {
      if( givenRect.bottom - givenRect.top > 0 )
      {
        nHeight =  rect .bottom - rect .top;
      }
    }
    size.cy = size.cy< nHeight? nHeight : size.cy;          
  }

  // return the size of what we just did.
  return size;
}

void TagsParser::PushFont( HDC hdc, const LOGFONT& lf )
{
  ApplyFont( hdc, lf );
}

void TagsParser::PopFont( HDC hdc, const LOGFONT& lf )
{
  ApplyFont( hdc, lf );
}

void TagsParser::ApplyFont( HDC hdc, const LOGFONT& lf )
{
  // if the current style is not the same as the last one
  // then we don't need to actually change anything.
  if( 0 == memcmp( &lf, &GetCurrentLogFont(), sizeof(LOGFONT)))
  {
    return;
  }

  // do we already have a font?
  const auto hFont = ::CreateFontIndirect( &lf );

  // select that font
  SelectObject(hdc, hFont );

  if( _font != nullptr )
  {
    DeleteObject( _font );
  }

  // this is the new font.
  _font = hFont;

  // this is the new log font
  _logFont = lf;
}
}}