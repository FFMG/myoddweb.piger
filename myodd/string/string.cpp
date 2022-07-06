#include "stdafx.h"
#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>

#include "string.h"
#include "assert.h"

namespace myodd{ namespace strings{
void Test()
#ifdef _DEBUG
{
  assert("a" == WString2String(_T("a")));
  assert("abcdef" == WString2String(_T("abcdef")));
  assert(_T("a") == String2WString( "a"));
  assert(_T("abcdef") == String2WString("abcdef"));

  std::wstring s1 = Format( _T("%10.2f"), 10.27 );
  assert( s1 == _T("     10.27"));

  assert( _tcsistr( _T("Hel"), _T("hello")) == NULL );

  assert( _tcsistr( _T("Hello"), _T("hello")) != NULL );
  assert( _tcsistr( _T("Hello"), _T("Hello")) != NULL );
  assert( _tcsistr( _T("abcdHello"), _T("hello")) != NULL );
  assert( _tcsistr( _T("abcdHello"), _T("Hello")) != NULL );
  assert( _tcsistr( _T("Helloabc"), _T("hello")) != NULL );
  assert( _tcsistr( _T("Helloabc"), _T("Hello")) != NULL );

  assert( _tcsistr( _T("Hello"), _T("no match")) == NULL );

  assert( ToString( (int)10, NULL ) == _T("10") );
  assert( ToString( (int)10 ) == _T("10") );
  assert( ToString( (double)10, NULL ) != _T("10") );
  assert( ToString( (float)10, NULL ) != _T("10") );
  assert( ToString( (float)10, NULL ) != _T("10") );
  assert( ToString( _T("A"), NULL ) == _T("A") );
}
#else
{
  __noop;
}
#endif

/**
 * \brief get a random string given a length and a source
 * \param source the list of characters we will be randomly choosing from
 * \param len the total length we want the string t
 * \return a random string made of 'source' characters.
 */
std::wstring RandomString(const std::wstring& source, const int len)
{
  // sanity checks.
  if( len == 0 )
  {
    return L"";
  }
  if( len<0 )
  {
    throw std::invalid_argument("The required string cannt be less than zero in lenght.");
  }
  const auto lengthOfSource = source.length();
  if( lengthOfSource ==  0 )
  {
    throw std::invalid_argument("The source must have at least one character!");
  }

  std::wstring result = L"";
  for (auto i = 0; i < len; ++i) 
  {
    result += source[rand() % (lengthOfSource - 1)];
  }
  return result;
}

/**
 * \brief generate a Version 4 uuid
 * \see https://en.wikipedia.org/wiki/Universally_unique_identifier#Format for more details
 * \return the version 4 uuid.
 */
std::wstring MakeUuid4()
{
  // a guid has hex numbers.
  const auto source = L"0123456789abcdef";
  std::wstring guid = L"";

  // format 8-4-4-4-12 (xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx)
  // https://en.wikipedia.org/wiki/Universally_unique_identifier#Format
  for( auto i = 0; i < 5; ++i )
  {
    switch( i )
    {
    case 0:
      guid += RandomString(source, 8);
      guid += L"-";
      break;

    case 2:
      // as we are version 4, we will add our version number.
      guid += L"4" + RandomString(source, 3);
      guid += L"-";
      break;

    case 4:
      guid += RandomString(source, 12);
      // this is the end
      break;

    default:
      guid += RandomString(source, 4);
      guid += L"-";
      break;
    }
  }
  return guid;
}

/**
 * Explode a string that is separated with '\0' chars.
 * This is used with items like GetPrivateProfileString( ... ) when we want to list all the keys.
 * @param std::vector<std::wstring>& the return container.
 * @param const wchar_t* the string with null chars '\0' finished by '\0\0'
 * @param size_t the length of the string. 
 * @return int the number of items returned.
 */
size_t explode_by_null_char
(
  std::vector<std::wstring>& ret,
  const wchar_t* s_keys,
  size_t nLen
)
{
  size_t retSize = 0;
  std::wstring stdToken;
  for(size_t nPos = 0; nPos < nLen; ++nPos )
  {
    if( s_keys[ nPos ] == _T('\0') )
    {
      ++retSize;
      ret.push_back( stdToken );
      stdToken.clear();
      if( nPos > 0 && s_keys[ nPos-1 ] == _T('\0'))
      {
        break;
      }
    }
    else
    {
      stdToken += s_keys[ nPos ];
    }
  }
  return retSize;
}

/**
 * Explode a given string given a delimiter string
 * @param std::vector<std::wstring>& the return container.
 * @param std::wstring the string we want to explode
 * @param const wchar_t* Set of delimiter characters.
 * @param int nCount the max number of items we want to return.
 *                   If the limit parameter is zero, then this is treated as 1.
 * @param bool bAddEmpty if we want to add empty params or not.
 * @return int the number of item that we found.
 */
size_t Explode
(
  std::vector<std::wstring>& ret,
  const std::wstring& s, 
  wchar_t strDelimit,
  int nCount /*=MYODD_MAX_INT32*/,
  bool bAddEmpty /*= true*/
)
{
  //  reset all
  ret.clear();

  auto l = s.length();

  // or return if we have no work to do.
  // If the limit parameter is zero, then this is treated as 1.
  if( 1 == nCount || 0 == nCount )
  {
    ret.push_back( s );
    return 1;
  }
  
  // reserve space.
  if( nCount > 1 && nCount != MYODD_MAX_INT32 )
  {
    ret.reserve( nCount );
  }

  size_t retSize = 0;
  size_t iLast = 0;

  for( size_t pos = 0; pos < l; ++pos)
  {
    if( s[pos] == strDelimit )
    {
      if( bAddEmpty || (pos-iLast) > 0 )
      {
        ++retSize;
        ret.push_back( s.substr( iLast, (pos-iLast) ));
      }
      iLast = (pos+1);
      if( nCount >-1 && (retSize+1) >= static_cast<size_t>(nCount) )
      {
        // the last item will be added bellow
        break;
      }
    }
  }

  if( bAddEmpty || (l-iLast) > 0 )
  {
    // add the remainder
    ret.push_back( s.substr( iLast ) );
    ++retSize;
  }

  // If count is negative, we only return retSize - count items.
  if( nCount < 0 )
  {
    if(static_cast<size_t>(nCount *-1) >= retSize )
    {
      ret.clear();
      retSize = 0;
    }
    else
    {
      retSize += nCount;  //  nCount is negative...
      ret.erase( ret.begin(), ret.begin() +retSize);
    }
  }
  
  // return the size.
  return retSize;
}

/**
 * Implode a string
 * 
 * @param const std::vector<std::wstring> & vector of strings.
 * @param std::wstring& the character that we will use to join the strings
 * @param int where we start joinning the items from, (default is 0).
 * @param int iToPiece = -1
 * @return std::wstring the replaced string
 */
std::wstring implode
(
  const std::vector<std::wstring> &s, 
  const std::wstring& glue,
  int iFromPiece,
  int iToPiece
)
{
  if( iToPiece != -1 && iFromPiece >= iToPiece )
    return _T("");

  if( iFromPiece >= (int)s.size() || iFromPiece < 0 )
    return _T("");

  std::vector<std::wstring>::const_iterator itEnd;
  if( iToPiece >= (int)s.size() || iToPiece < 0 )
    itEnd = s.end();
  else
    itEnd = s.begin() + iToPiece;

	return implode( glue, s.begin()+iFromPiece, itEnd);
}

/**
 * Implode/Join/Glue a string.
 * Using an array of strings, join them into a single string.
 * @see implode( ... )
 * @param const std::wstring& the string that will glue all the strings together.
 * @param std::vector<std::wstring>::const_iterator the iterator we want to start joining from.
 * @param std::vector<std::wstring>::const_iterator the iterator we want to stop joining to
 * @return std::wstring the recreated string
 */
std::wstring implode
(
  const std::wstring& glue,
  std::vector<std::wstring>::const_iterator itStart,
  std::vector<std::wstring>::const_iterator itEnd
)
{
	std::wstring r = _T("");
	for( std::vector<std::wstring>::const_iterator it = itStart; 
       it != itEnd; 
       ++it
      ) 
	{
		r = ((it != itStart)?	(r + glue) : (_T(""))) + (*it);
	}
  return (r);
}

/**
 * Compare 2 string
 * @param const std::wstring& lhs the lefthand string
 * @param const std::wstring& rhs the right hand string
 * @param bool caseSensitive if this is case sensitive or not.
 * @return int32_t 0 if they are the same or -1/1 depending on the strings length
 *         <0	the first character that does not match has a lower value in lhs than in rhs
 *         0	the contents of both strings are equal
 *         >0	the first character that does not match has a greater value in lhs than in rhs
 */
int32_t Compare( const std::wstring& lhs, const std::wstring& rhs, bool caseSensitive )
{
  if(caseSensitive)
  {
    // http ://www.cplusplus.com/reference/cwchar/wcscmp/
    return wcscmp(lhs.c_str(), rhs.c_str());
  }

  return _wcsicmp( lhs.c_str(), rhs.c_str() );
}

/**
 * Normal string search and replace,
 * This is 'inplace' meaning that the return value will be changed.
 *
 * @param const std::wstring& the original text we are searching in
 * @param const std::wstring& the string we are looking for
 * @param const std::wstring& the string we will be replacing it with.
 * @param bool caseSensitive if the replacement is case sensitive or not.
 * @return std::wstring the replaced string
 */
std::wstring Replace
(
  const std::wstring& haystack,
  const std::wstring& needle, 
  const std::wstring& replace,
  bool caseSensitive /*=caseSensitive*/
)
{
  const size_t needleLen = needle.length();
  if( 0 == needleLen )
  {
    //no needle.
    return haystack; //  what are we looking for?
  }

  const size_t replaceLen = replace.length();
  if (replaceLen == needleLen && caseSensitive && needle == replace )
  {
    // the needle and the replacement are the same.
    // nothing will change
    // if case insensitve, we must look for the meedle.
    return haystack;
  }
  // create the return value
  std::wstring result(haystack);

  // look for the first needle.
  auto loc = Find( result, needle, 0, caseSensitive);
  while (loc != -1)
  {
    // replace the string
    result.replace(loc, needleLen, replace);

    // look for the next item from the last known location
    // plus the len of the text we just replaced.
    loc = Find(result, needle, loc + replaceLen, caseSensitive);
  }

  // return what we found/did
  return result;
}

int32_t Find
(
  const std::wstring& haystack, 
  const std::wstring& needle, 
  const uint32_t from /*= 0*/, 
  bool caseSensitive /*=true*/)
{
  //  are we looking past the end?
  if (from >= haystack.length() )
  {
    return -1;
  }

  auto it = haystack.end();
  if (caseSensitive)
  {
    it = std::search(
      haystack.begin() + from, haystack.end(),
      needle.begin(), needle.end(),
      [](wchar_t ch1, wchar_t ch2) { return ch1 == ch2; }
    );
  }
  else
  {
    it = std::search(
      haystack.begin() + from, haystack.end(),
      needle.begin(), needle.end(),
      [](wchar_t ch1, wchar_t ch2) { return std::toupper(ch1, std::locale())
                                            ==
                                            std::toupper(ch2, std::locale()); }
    );
  }
  return (it == haystack.end() ? -1 : it - haystack.begin() );
}

/**
 * convert a string to the lower case equivalent.
 * @see boost/foreach.hpp
 *
 * @param std::wstring the character we want to convert.
 * @return std::wstring the lower string
 */
const std::wstring lower(const std::wstring& s)
{
  std::wstring ret = L"";
  BOOST_FOREACH(wchar_t tch, s )
  {
    ret += lower( tch );
  }
  return ret;
}

/**
 * convert single char to lower case.
 *
 * @param wchar_t the character we want to convert.
 * @return wchar_t the lower char character
 */
const wchar_t lower( const wchar_t c )
{
  if( c >= L'A' && c <= L'Z' )
  {
    return (L'a'+(c- L'A'));
  }
  return c;
}

/**
 * I can't shake the feeling that we might really need something a
 * little bit more powerful.
 * Or at least something that follows the regex standard(s).
 *
 * @param const wchar_t* the wild search, something like "^(5){1}"
 * @param const wchar_t* the string we will be comparing against.
 * @return int
 */
bool wildcmp(const std::wstring& wild, const std::wstring& string)
{
  return wildcmp(wild.c_str(), string.c_str());
}

/**
 * I can't shake the feeling that we might really need something a 
 * little bit more powerful.
 * Or at least something that follows the regex standard(s).
 *
 * @param const wchar_t* the wild search, something like "^(5){1}"
 * @param const wchar_t* the string we will be comparing against.
 * @return int 
 */
bool wildcmp(const wchar_t* wild, const wchar_t* string)
{
  try
  {
    boost::wsmatch matches;
    boost::wregex stringRegex;
    std::wstring stdString( string );
    stringRegex.assign( wild, boost::regex_constants::icase);
    if (boost::regex_match( stdString, matches, stringRegex))
    {
      return true;
    }
  }
  catch( const std::runtime_error & e )
  {
    UNUSED_ALWAYS( e );
    // std::runtime_error if the complexity of matching the expression 
    // against an N character string begins to exceed O(N2), 
    // or if the program runs out of stack space
  }
  catch( ... )
  {
    // boost couldn't work it out.
  }
  return false;
}

/**
 * Check if the given string is a number or not.
 * @param const std::wstring& the string we are checking
 * @param bool if we want to allow the number to be a decimal.
 * @return bool if the string is a number or not.
 */
bool IsNumeric( const std::wstring& s, bool allowDecimal /*= true*/ )
{
  auto t = s;
  Trim(t);
  if( t.length() == 0 )
  {
    //  empty is not a number
    return false;
  }

  auto it = t.begin();
  if( *it == _T('+') || *it == _T('-'))
  {
    t.erase(it);
    if (t.length() == 0)
    {
      //  just a + or a - is not a number.
      return false;
    }
  }

  // if we have '1.' we can remove the last '.'
  auto rit = t.rbegin();
  if (*rit == _T('.') )
  {
    t.erase(std::next(rit).base());
    if (t.length() == 0)
    {
      //  just a '.' is not a number.
      return false;
    }
  }

  // we have not yet found a decimal.
  auto decimalFound = false;

  // go around and look for non-numbers.
  for( auto string_iterator = t.begin(); string_iterator != t.end(); ++ string_iterator )
  {
    // is it decimal?
    if( *string_iterator == _T('.'))
    {
      if( decimalFound || !allowDecimal )
      {
        // we found one already
        return false;
      }

      // we cannot find it again.
      decimalFound = true;
      continue;
    }
    if(!iswdigit(*string_iterator))
    {
      return false;
    }
  }

  // if we are here, then it is...
  return true;
}

/**
 * Trim left && right character(s)
 * @param std::wstring& str the string we want to edit.
 * @param const wchar_t* the char(s) we want to trim off.
 * @return none
 */
void Trim( std::wstring& str, const wchar_t* chars /*= _T( " " )*/ )
{
  TrimLeft( str, chars );
  TrimRight( str, chars );
}

/**
 * Trim right character(s)
 * @param std::wstring& str the string we want to edit.
 * @param const wchar_t* the char(s) we want to trim off.
 * @return none
 */
void TrimRight( std::wstring& str, const wchar_t* chars )
{
  if (!str.empty())
  {
    const auto pos = str.find_first_not_of(chars);
    if (pos != std::string::npos)
    {
      str.erase(0,pos);
    }
    else
    {
      str.erase( str.begin() , str.end() ); // make empty
    }
  }
}

/**
 * Trim left character(s)
 * @param std::wstring& str the string we want to edit.
 * @param const wchar_t* the char(s) we want to trim off.
 * @return none
 */
void TrimLeft( std::wstring& str, const wchar_t* chars )
{
  if (!str.empty())
  {
    std::string::size_type pos = str.find_last_not_of(chars);
    if (pos != std::string::npos)
    {
      str.erase(pos+1);
    }
    else
    {
      str.erase( str.begin() , str.end() ); // make empty
    }
  }
}

/**
 * Find a substring within a string (case insensitive)
 * _tcsistr() will search by ignoring the case of those characters 
 * that fall in the ANSI range a-z and A-Z.
 * @see ::_tcsstr( ... )
 * @param const wchar_t* nul-terminated string to search
 * @param const wchar_t* nul-terminated string to search for
 * @return wchar_t* - if successful, returns a pointer to the first 
 *                  occurrence of strCharSet in string;  otherwise, 
 *                  returns NULL
 */ 
wchar_t* _tcsistr(const wchar_t* string, const wchar_t* strCharSet)
{
  size_t nLen = _tcslen(strCharSet);

	while (*string)
	{
		if (_tcsnicmp(string, strCharSet, nLen) == 0)
			break;
		string++;
	}

	if (*string == _T('\0'))
		string = NULL;

	return (wchar_t*)string;
}

/**
 * Find a substring within a string (case insensitive)
 * _tcsistr() will search by ignoring the case of those characters 
 * that fall in the ANSI range a-z and A-Z.
 * @see ::_tcsstr( ... )
 * @see _tcsistr( ... )
 * @param const std::wstring& string to search
 * @param const std::wstring& string to search for
 * @return wchar_t* - if successful, returns a pointer to the first 
 *                  occurrence of strCharSet in string;  otherwise, 
 *                  returns NULL
 */ 
wchar_t* _tcsistr(const std::wstring& string, const std::wstring& strCharSet)
{
  return _tcsistr( string.c_str(), strCharSet.c_str() );
}

/**
* Check if a given string is empty
* "     " even if the len of the string is not 0
* @param const wchar_t* src the string been checked.
* @return bool if the string is empty or not.
*/
bool IsEmptyString(const wchar_t* src )
{
	if (src == nullptr)
	{
		return true;
	}

  auto len = wcslen(src);
  for( ;len>0;)
  {
    if( src[--len] != L' ' )
    {
      return false;
    }
  }
  return true;
}

/**
* Check if a given string is empty
* "     " even if the len of the string is not 0
* @param const char* src the string been checked.
* @return bool if the string is empty or not.
*/
bool IsEmptyString(const char* src)
{
  if (src == nullptr)
  {
    return true;
  }

  auto len = strlen(src);
  for (; len>0;)
  {
    if (src[--len] != ' ')
    {
      return false;
    }
  }
  return true;
}

/**
 * Check if a given wide string is empty
 * "     " even if the len of the string is not 0
 * @param const std::wstring& src the string we are checking.
 * @return bool if the string is empty or not.
 */
bool IsEmptyString( const std::wstring& src )
{
  return IsEmptyString( src.c_str() );
}

/**
* Check if a given string is empty
* "     " even if the len of the string is not 0
* @param const std::string& src the string we are checking.
* @return bool if the string is empty or not.
*/
bool IsEmptyString(const std::string& src )
{
  return IsEmptyString(src.c_str());
}


/**
 * Convert a double value to an string given a format.
 * @param std::wstring& 
 * @param float the number we want to use.
 * @param const wchar_t*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool IntToString( std::wstring& value, int i, const wchar_t* pszFormat )
{
  try
  {
    value = (boost::wformat(pszFormat?pszFormat:_T("%d")) % i ).str();
  }
  catch( ... )
  {
    value = _T("");
    return false;
  }
  return true;
}

/**
 * Convert a double value to an string given a format.
 * @param std::wstring& 
 * @param float the number we want to use.
 * @param const wchar_t*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool DoubleToString( std::wstring& value, double d, const wchar_t* pszFormat )
{
  try
  {
    value = (boost::wformat(pszFormat?pszFormat:_T("%f")) % d ).str();
  }
  catch( ... )
  {
    value = _T("");
    return false;
  }
  return true;
}

/**
 * Convert a float value to an string given a format.
 * @param std::wstring& 
 * @param float the number we want to use.
 * @param const wchar_t*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool FloatToString( std::wstring& value, float f, const wchar_t* pszFormat )
{
  try
  {
    value = (boost::wformat(pszFormat?pszFormat:_T("%f")) % f ).str();
  }
  catch( ... )
  {
    value = _T("");
    return false;
  }
  return true;
}

/**
 * Convert a float value to an string given a format.
 * @param std::wstring& the return value.
 * @param const wchar_t* the string we would like to format.
 * @param const wchar_t*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool StringToString( std::wstring& value, const wchar_t* l, const wchar_t* pszFormat )
{
  try
  {
    value = (boost::wformat(pszFormat?pszFormat:_T("%s")) % l ).str();
  }
  catch( ... )
  {
    value = _T("");
    return false;
  }
  return true;
}

/**
 * Convert a AscII string to a wide char.
 * @param const char* the string we want to convert.
 * @param UINT the code page
 * @return wchar_t* the converted wide char.
 */
std::wstring String2WString(const std::string& s2Convert)
{
  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;
  return converterX.from_bytes(s2Convert);
}

/**
* Convert a AscII string to a wide char.
* @param const char* the string we want to convert.
* @param UINT the code page
* @return wchar_t* the converted wide char.
*/
std::string WString2String(const std::wstring& ws2Convert)
{
  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;
  return converterX.to_bytes(ws2Convert);
}

std::wstring Format(const wchar_t* pszFormat, ... )
{
  va_list argp;
  va_start(argp, pszFormat);

  // the required number of chars.
  int len = _vsctprintf( pszFormat, argp );
  if( 0 >= len )
  {
    // free the arguments.
    va_end(argp);

    // return nothing
    return std::wstring(_T(""));
  }

  // _vscprintf doesn't count + 1; terminating '\0'
  ++len;
  
  std::wstring result = _T("");

  // create the new buffer.
  wchar_t* buffer = new wchar_t[len];
  if( _vsntprintf_s(buffer, len, len, pszFormat, argp) > 0 )
  {
    // set the return the value
    result = std::wstring(buffer);
  }
  
  // free the arguments.
  va_end(argp);

  //
  delete [] buffer;

  // done.
  return result;
}

template<>
size_t Length<const wchar_t*>(const wchar_t* src)
{
  return src ? wcslen(src) : 0;
}

template<>
size_t Length<const std::wstring&>(const std::wstring& src)
{
  return src.length();
}

template<>
size_t Length<const char*>(const char* src)
{
  return src ? strlen(src) : 0;
}

template<>
size_t Length<const std::string&>(const std::string& src)
{
  return src.length();
}

} //  strings
} //  myodd