#include "stdafx.h"
#include <locale>
#include <codecvt>
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

  MYODD_STRING s1 = Format( _T("%10.2f"), 10.27 );
  assert( s1 == _T("     10.27"));
  assert( IsEmptyString( _T("    " )));
  assert( IsEmptyString( NULL ));
  assert( IsEmptyString( _T("" )));
  assert( !IsEmptyString( _T("    A" )));
  assert( !IsEmptyString( _T("A" )));

  assert( false == IsNumeric( _T("-1.2" ), false ));  // decimal not allowed.
  assert( true == IsNumeric( _T("-1" ), false ));  // decimal allowed.

  assert( true == IsNumeric( _T("-1" )));
  assert( true == IsNumeric( _T("1" )));
  assert( true == IsNumeric( _T("-" )));
  assert( true == IsNumeric( _T("+" )));
  assert( true == IsNumeric( _T("+.1" )));
  assert( true == IsNumeric( _T("1." )));
  assert( true == IsNumeric( _T("-1.1" )));
  assert( false == IsNumeric( _T("-1.1.1" )));
  assert( false == IsNumeric( _T("A1,1" )));
  assert( false == IsNumeric( _T("A11" )));
  assert( false == IsNumeric( _T("11A" )));
  assert( true == IsNumeric( _T("-999" )));
  assert( false == IsNumeric( _T("-999.a" )));
  assert( false == IsNumeric( _T("-999.1   2" )));

  assert( _tcsistr( _T("Hel"), _T("hello")) == NULL );

  assert( _tcsistr( _T("Hello"), _T("hello")) != NULL );
  assert( _tcsistr( _T("Hello"), _T("Hello")) != NULL );
  assert( _tcsistr( _T("abcdHello"), _T("hello")) != NULL );
  assert( _tcsistr( _T("abcdHello"), _T("Hello")) != NULL );
  assert( _tcsistr( _T("Helloabc"), _T("hello")) != NULL );
  assert( _tcsistr( _T("Helloabc"), _T("Hello")) != NULL );

  assert( _tcsistr( _T("Hello"), _T("no match")) == NULL );

  assert( icompare( _T("Hello"), _T("Hello") ) == 0 );
  assert( icompare( _T("Hello"), _T("hello") ) == 0 );
  assert( icompare( _T("Hello"), _T("helo") ) != 0 );   //  no match

  assert( ifind( _T("Hello"), _T("hello") ) == 0 );
  assert( ifind( _T("HelloHello"), _T("hello"), 5 ) == 5 );
  assert( ifind( _T("HelloHello"), _T("hello"), 2 ) == 5 );

  assert( ireplace(  _T("Hellohello"), _T("Hello"), _T("no" )) == _T("nono") );
  assert( ireplace(  _T("Hellohello"), _T("Hello"), _T("No" )) == _T("NoNo") );
  assert( ireplace(  _T("Hellohello"), _T("Hello"), _T("" )) == _T("") );
  assert( ireplace(  _T("ABC"), _T("A"), _T("A" )) == _T("ABC") );  //  prevent infinite loop
  assert( ireplace(  _T("ABC"), _T("A"), _T("a" )) == _T("ABC") );  //  prevent infinite loop
  
  assert( replace( _T("Hellohello"), _T("Hello"), _T("No" )) == _T("Nohello") );
  assert( replace( _T("Hellohello"), _T("Hello"), _T("" )) == _T("hello") );
  assert( replace(  _T("ABC"), _T("A"), _T("A" )) == _T("ABC") );   //  prevent infinite loop

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
 * Explode a string that is separated with '\0' chars.
 * This is used with items like GetPrivateProfileString( ... ) when we want to list all the keys.
 * @param std::vector<MYODD_STRING>& the return container.
 * @param const MYODD_CHAR* the string with null chars '\0' finished by '\0\0'
 * @param size_t the length of the string. 
 * @return int the number of items returned.
 */
size_t explode_by_null_char
(
  std::vector<MYODD_STRING>& ret,
  const MYODD_CHAR* s_keys,
  size_t nLen
)
{
  size_t retSize = 0;
  MYODD_STRING stdToken;
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
 * @param std::vector<MYODD_STRING>& the return container.
 * @param MYODD_STRING the string we want to explode
 * @param const MYODD_CHAR* Set of delimiter characters.
 * @param int nCount the max number of items we want to return.
 *                   If the limit parameter is zero, then this is treated as 1.
 * @param bool bAddEmpty if we want to add empty params or not.
 * @return int the number of item that we found.
 */
size_t explode
(
  std::vector<MYODD_STRING>& ret,
  const MYODD_STRING& s, 
  MYODD_CHAR strDelimit,
  int nCount /*=-1*/,
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
  if( nCount > 1 )
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
      if( nCount >-1 && (retSize+1) >= (size_t)nCount )
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

  // return the size.
  return retSize;
}

/**
 * Implode a string
 * 
 * @param const std::vector<MYODD_STRING> & vector of strings.
 * @param MYODD_STRING& the character that we will use to join the strings
 * @param int where we start joinning the items from, (default is 0).
 * @param int iToPiece = -1
 * @return MYODD_STRING the replaced string
 */
MYODD_STRING implode
(
  const std::vector<MYODD_STRING> &s, 
  const MYODD_STRING& glue,
  int iFromPiece,
  int iToPiece
)
{
  if( iToPiece != -1 && iFromPiece >= iToPiece )
    return _T("");

  if( iFromPiece >= (int)s.size() || iFromPiece < 0 )
    return _T("");

  std::vector<MYODD_STRING>::const_iterator itEnd;
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
 * @param const MYODD_STRING& the string that will glue all the strings together.
 * @param std::vector<MYODD_STRING>::const_iterator the iterator we want to start joining from.
 * @param std::vector<MYODD_STRING>::const_iterator the iterator we want to stop joining to
 * @return MYODD_STRING the recreated string
 */
MYODD_STRING implode
(
  const MYODD_STRING& glue,
  std::vector<MYODD_STRING>::const_iterator itStart,
  std::vector<MYODD_STRING>::const_iterator itEnd
)
{
	MYODD_STRING r = _T("");
	for( std::vector<MYODD_STRING>::const_iterator it = itStart; 
       it != itEnd; 
       ++it
      ) 
	{
		r = ((it != itStart)?	(r + glue) : (_T(""))) + (*it);
	}
  return (r);
}

/**
 * Compare 2 string but case insensitive.
 * @param const MYODD_STRING& the lefthand string
 * @param const MYODD_STRING& the right hand string
 * @return int 0 if they are the same or -1/1 depending on the strings length
 */
int icompare( const MYODD_STRING& s1, const MYODD_STRING& s2 )
{
  MYODD_STRING::const_iterator it1=s1.begin();
  MYODD_STRING::const_iterator it2=s2.begin();

  //stop when either string's end has been reached
  while ( (it1!=s1.end()) && (it2!=s2.end()) ) 
  { 
    if( std::toupper((unsigned char)*it1) != std::toupper((unsigned char)*it2)) //letters differ?
    {
      // return -1 to indicate smaller than, 1 otherwise
      return (std::toupper((unsigned char)*it1)  < std::toupper((unsigned char)*it2)) ? -1 : 1; 
    }
    //proceed to the next character in each string
    ++it1;
    ++it2;
  }

  size_t size1=s1.size(), 
         size2=s2.size();// cache lengths

  //return -1,0 or 1 according to strings' lengths
  if (size1==size2)
  {
    return 0;
  }
  return (size1<size2) ? -1 : 1;
}

/**
 * Case insensitivce search
 * @see MYODD_STRING::find( ...)
 * @param const MYODD_STRING& the string we will be searching in.
 * @param const MYODD_STRING& the string we are looking for, (int the original string)
 * @param unsigned int the position we want to start searching for. 
 * @return int the position of the string or MYODD_STRING::npos if the string could not be found.
 */
size_t ifind
( 
  const MYODD_STRING& origStr, 
  const MYODD_STRING& srchStr, 
  const size_t nFrom /*= 0*/ 
)
{
  // make sure we are not searching past the length of the string
  // otherwise we know it cannot exist.
  if( nFrom >= origStr.length() )
    return MYODD_STRING::npos;  // cannot find it.

  // look for that string
  MYODD_CHAR* lpFind = _tcsistr( origStr.c_str()+ nFrom, srchStr.c_str() );
  if( NULL == lpFind )
    return MYODD_STRING::npos;  //  not found

  return (size_t)(lpFind - origStr.c_str());
}

/**
 * Normal string search and replace, case insensitive,
 * Note that we don't change the original value
 *
 * @param const MYODD_STRING& the original text we are searching in
 * @param const MYODD_STRING& the string we are looking for, case insensitive
 * @param const MYODD_STRING& the string we will be replacing it with.
 * @return MYODD_STRING the replaced string
 */
MYODD_STRING ireplace
(
  const MYODD_STRING& origStr,
  const MYODD_STRING& srchStr, 
  const MYODD_STRING& replaceStr
)
{
  MYODD_STRING retVal(origStr);
  ireplace_inplace( retVal, srchStr, replaceStr );
  return retVal;
}

/**
 * Normal string search and replace, case insensitive,
 * This is 'inplace' meaning that the return value will be changed.
 *
 * @param const MYODD_STRING& the original text we are searching in
 * @param const MYODD_STRING& the string we are looking for, case insensitive
 * @param const MYODD_STRING& the string we will be replacing it with.
 * @return none
 */
void ireplace_inplace
(
  MYODD_STRING& origStr,
  const MYODD_STRING& srchStr, 
  const MYODD_STRING& replaceStr
)
{
  const size_t len = replaceStr.length();
  const size_t lens = srchStr.length();
  if( 0 == lens || (len == lens && 0 == ifind(replaceStr, srchStr)) )
  {
    return; //  what are we looking for?
  }

  MYODD_STRING::size_type loc = ifind(origStr, srchStr);
  while (loc != MYODD_STRING::npos)
  {
    origStr.replace(loc, lens, replaceStr);
    loc = ifind(origStr, srchStr, loc + len );
  }
}

/**
 * Normal string search and replace,
 * This is 'inplace' meaning that the return value will be changed.
 *
 * @param const MYODD_STRING& the original text we are searching in
 * @param const MYODD_STRING& the string we are looking for
 * @param const MYODD_STRING& the string we will be replacing it with.
 * @return MYODD_STRING the replaced string
 */
MYODD_STRING replace
(
  const MYODD_STRING& origStr,
  const MYODD_STRING& srchStr, 
  const MYODD_STRING& replaceStr
)
{
  MYODD_STRING retVal(origStr);
  replace_inplace( retVal, srchStr, replaceStr );
  return retVal;
}

/**
 * Normal string search and replace,
 * Note that we don't change the original value
 *
 * @param const MYODD_STRING& the original text we are searching in
 * @param const MYODD_STRING& the string we are looking for
 * @param const MYODD_STRING& the string we will be replacing it with.
 * @return MYODD_STRING the replaced string
 */
void replace_inplace
(
  MYODD_STRING& origStr,
  const MYODD_STRING& srchStr, 
  const MYODD_STRING& replaceStr
)
{
  const size_t len = replaceStr.length();
  const size_t lens = srchStr.length();
  if( 0 == lens || (len == lens && replaceStr == srchStr) )
  {
    return; //  what are we looking for?
  }

  MYODD_STRING::size_type loc = origStr.find(srchStr);
  while (loc != MYODD_STRING::npos)
  {
    origStr.replace(loc, lens, replaceStr);
    loc = origStr.find(srchStr, loc + len );
  }
}

/**
 * convert a string to the lower case equivalent.
 * @see boost/foreach.hpp
 *
 * @param MYODD_STRING the character we want to convert.
 * @return MYODD_STRING the lower string
 */
MYODD_STRING lower(const MYODD_STRING& s)
{
  MYODD_STRING ret = _T("");
  BOOST_FOREACH(MYODD_CHAR tch, s )
  {
    ret += lower( tch );
  }
  return ret;
}

/**
 * convert single char to lower case.
 *
 * @param MYODD_CHAR the character we want to convert.
 * @return MYODD_CHAR the lower char character
 */
MYODD_CHAR lower(MYODD_CHAR c )
{
  if( c >= _T('A') && c <= _T('Z') )
  {
    return (_T('a')+(c-_T('A')));
  }
  return c;
}

/**
 * I can't shake the feeling that we might really need something a
 * little bit more powerful.
 * Or at least something that follows the regex standard(s).
 *
 * @param const MYODD_CHAR* the wild search, something like "^(5){1}"
 * @param const MYODD_CHAR* the string we will be comparing against.
 * @return int
 */
bool wildcmp(const MYODD_STRING& wild, const MYODD_STRING& string)
{
  return wildcmp(wild.c_str(), string.c_str());
}

/**
 * I can't shake the feeling that we might really need something a 
 * little bit more powerful.
 * Or at least something that follows the regex standard(s).
 *
 * @param const MYODD_CHAR* the wild search, something like "^(5){1}"
 * @param const MYODD_CHAR* the string we will be comparing against.
 * @return int 
 */
bool wildcmp(const MYODD_CHAR* wild, const MYODD_CHAR* string)
{
  try
  {
#ifdef _UNICODE
    boost::wsmatch matches;
    boost::wregex stringRegex;
    std::wstring stdString( string );
#else
    boost::smatch matches;
    boost::regex stringRegex;
    std::string stdString( string );
#endif
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
 * @param const MYODD_STRING& the string we are checking
 * @param bool if we want to allow the number to be a decimal.
 * @return bool if the string is a number or not.
 */
bool IsNumeric( const MYODD_STRING& s, bool allowDecimal /*= true*/ )
{
#ifdef _UNICODE
  typedef boost::wcmatch boost_match;
  typedef boost::wregex  boost_regex;
#else
  typedef boost::cmatch boost_match;
  typedef boost::regex  boost_regex;
#endif

  if( allowDecimal )
  {
    static const MYODD_CHAR* lpMatches = _T("^\\s*(([-+]?\\d{0,})(\\.?\\d{0,}))\\s*$");
    static boost_match matches;
    static boost_regex aStringregex( lpMatches );
    if (boost::regex_match( s.c_str(), matches, aStringregex))
    {
      return true;
    }
  }
  else
  {
    static const MYODD_CHAR* lpMatches = _T("^\\s*(([-+]?\\d{0,}))\\s*$");
    static boost_match matches;
    static boost_regex aStringregex( lpMatches );
    if (boost::regex_match( s.c_str(), matches, aStringregex))
    {
      return true;
    }
  }
  return false;
}

/**
 * Trim left && right character(s)
 * @param MYODD_STRING& str the string we want to edit.
 * @param const MYODD_CHAR* the char(s) we want to trim off.
 * @return none
 */
void Trim( MYODD_STRING& str, const MYODD_CHAR* chars /*= _T( " " )*/ )
{
  TrimLeft( str, chars );
  TrimRight( str, chars );
}

/**
 * Trim right character(s)
 * @param MYODD_STRING& str the string we want to edit.
 * @param const MYODD_CHAR* the char(s) we want to trim off.
 * @return none
 */
void TrimRight( MYODD_STRING& str, const MYODD_CHAR* chars )
{
  if (!str.empty())
  {
    std::string::size_type pos = str.find_first_not_of(chars);
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
 * @param MYODD_STRING& str the string we want to edit.
 * @param const MYODD_CHAR* the char(s) we want to trim off.
 * @return none
 */
void TrimLeft( MYODD_STRING& str, const MYODD_CHAR* chars )
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
 * @param const MYODD_CHAR* nul-terminated string to search
 * @param const MYODD_CHAR* nul-terminated string to search for
 * @return MYODD_CHAR* - if successful, returns a pointer to the first 
 *                  occurrence of strCharSet in string;  otherwise, 
 *                  returns NULL
 */ 
MYODD_CHAR* _tcsistr(const MYODD_CHAR* string, const MYODD_CHAR* strCharSet)
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

	return (MYODD_CHAR*)string;
}

/**
 * Find a substring within a string (case insensitive)
 * _tcsistr() will search by ignoring the case of those characters 
 * that fall in the ANSI range a-z and A-Z.
 * @see ::_tcsstr( ... )
 * @see _tcsistr( ... )
 * @param const MYODD_STRING& string to search
 * @param const MYODD_STRING& string to search for
 * @return MYODD_CHAR* - if successful, returns a pointer to the first 
 *                  occurrence of strCharSet in string;  otherwise, 
 *                  returns NULL
 */ 
MYODD_CHAR* _tcsistr(const MYODD_STRING& string, const MYODD_STRING& strCharSet)
{
  return _tcsistr( string.c_str(), strCharSet.c_str() );
}

/**
* Check if a given string is empty
* "     " even if the len of the string is not 0
* @param const MYODD_CHAR*
* @return bool if the string is empty or not.
*/
bool IsEmptyString(const MYODD_CHAR* s )
{
  if( s == NULL )
    return true;

  size_t len = _tcslen(s);
  for( ;len>0;)
  {
    if( s[--len] != ' ' )
    {
      return false;
    }
  }
  return true;
}

/**
 * Check if a given string is empty
 * "     " even if the len of the string is not 0
 * @param const MYODD_STRING
 * @return bool if the string is empty or not.
 */
bool IsEmptyString( const MYODD_STRING& s )
{
  return IsEmptyString( s.c_str() );
}

/**
 * Convert a double value to an string given a format.
 * @param MYODD_STRING& 
 * @param float the number we want to use.
 * @param const MYODD_CHAR*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool IntToString( MYODD_STRING& value, int i, const MYODD_CHAR* pszFormat )
{
  try
  {
#ifdef _UNICODE
    value = (boost::wformat(pszFormat?pszFormat:_T("%d")) % i ).str();
#else
    value = (boost::format(pszFormat?pszFormat:"%d") % i ).str();
#endif
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
 * @param MYODD_STRING& 
 * @param float the number we want to use.
 * @param const MYODD_CHAR*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool DoubleToString( MYODD_STRING& value, double d, const MYODD_CHAR* pszFormat )
{
  try
  {
#ifdef _UNICODE
    value = (boost::wformat(pszFormat?pszFormat:_T("%f")) % d ).str();
#else
    value = (boost::format(pszFormat?pszFormat:"%f") % d ).str();
#endif
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
 * @param MYODD_STRING& 
 * @param float the number we want to use.
 * @param const MYODD_CHAR*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool FloatToString( MYODD_STRING& value, float f, const MYODD_CHAR* pszFormat )
{
  try
  {
#ifdef _UNICODE
    value = (boost::wformat(pszFormat?pszFormat:_T("%f")) % f ).str();
#else
    value = (boost::format(pszFormat?pszFormat:"%f") % f ).str();
#endif
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
 * @param MYODD_STRING& the return value.
 * @param const MYODD_CHAR* the string we would like to format.
 * @param const MYODD_CHAR*|NULL the format we want to use the float with.
 * @return bool success or if there was an error.
 */
bool StringToString( MYODD_STRING& value, const MYODD_CHAR* l, const MYODD_CHAR* pszFormat )
{
  try
  {
#ifdef _UNICODE
    value = (boost::wformat(pszFormat?pszFormat:_T("%s")) % l ).str();
#else
    value = (boost::format(pszFormat?pszFormat:"%s") % l ).str();
#endif
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

MYODD_STRING Format(const MYODD_CHAR* pszFormat, ... )
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
    return MYODD_STRING(_T(""));
  }

  // _vscprintf doesn't count + 1; terminating '\0'
  ++len;
  
  MYODD_STRING result = _T("");

  // create the new buffer.
  MYODD_CHAR* buffer = new MYODD_CHAR[len];
  if( _vsntprintf_s(buffer, len, len, pszFormat, argp) > 0 )
  {
    // set the return the value
    result = MYODD_STRING(buffer);
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