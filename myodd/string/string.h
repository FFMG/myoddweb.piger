#pragma once

#ifndef __Strings_h__
#define __Strings_h__

#include "../common/includes.h"

#include <atlbase.h>
#include <atlconv.h>

#include <sstream> 
#include <vector>

namespace myodd{ namespace strings{
  void Test();  //  test data

  // check if a string is empty.
  bool IsEmptyString( LPCTSTR s );
  bool IsEmptyString( const STD_TSTRING& s );

  // converst to lower case.
  TCHAR lower(TCHAR c );
  STD_TSTRING lower(const STD_TSTRING& s);

  // search and replace
  STD_TSTRING replace
    (
    const STD_TSTRING& origStr,
    const STD_TSTRING& srchStr, 
    const STD_TSTRING& replaceStr
    );

  void replace_inplace
    (
    STD_TSTRING& origStr,
    const STD_TSTRING& srchStr, 
    const STD_TSTRING& replaceStr
    );

  // search and ireplace
  STD_TSTRING ireplace
    (
    const STD_TSTRING& origStr,
    const STD_TSTRING& srchStr, 
    const STD_TSTRING& replaceStr
    );

  void ireplace_inplace
    (
    STD_TSTRING& origStr,
    const STD_TSTRING& srchStr, 
    const STD_TSTRING& replaceStr
    );

  size_t ifind( const STD_TSTRING& origStr, const STD_TSTRING& srchStr, const size_t nFrom = 0 );
  int icompare( const STD_TSTRING& origStr, const STD_TSTRING& srchStr );

  // explode with a '\0' char
  size_t explode_by_null_char
    (
    std::vector<STD_TSTRING>& ret,
    LPCTSTR s_keys, 
    size_t nLen
    );

  // explode a string with a set of delimiter characters
  size_t explode
    (
    std::vector<STD_TSTRING>& ret,
    const STD_TSTRING& s, 
    TCHAR strDelimit, 
    int nCount =-1,
    bool bAddEmpty = true
    );

  // implode a string
  STD_TSTRING implode
    (
    const std::vector<STD_TSTRING> &s, 
    const STD_TSTRING& glue,
    int iFromPiece = 0,
    int iToPiece = -1
    );

  // implode a string
  STD_TSTRING implode
    (
    const STD_TSTRING& glue,
    std::vector<STD_TSTRING>::const_iterator itStart,
    std::vector<STD_TSTRING>::const_iterator itEnd
    );

  // regex compare.
  bool wildcmp(LPCTSTR wild, LPCTSTR string);
  bool wildcmp(const STD_TSTRING& wild, const STD_TSTRING& string);

  bool IntToString( STD_TSTRING& value, int i, LPCTSTR pszFormat );
  bool FloatToString( STD_TSTRING& value, float f, LPCTSTR pszFormat );
  bool DoubleToString( STD_TSTRING& value, double d, LPCTSTR pszFormat );
  bool StringToString( STD_TSTRING& value, LPCTSTR s, LPCTSTR pszFormat );

  /**
   * Convert a double variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return STD_TSTRING the converted int
   */
  template<typename T>
  inline STD_TSTRING ToString( T t, LPCTSTR pszFormat  )
  {
    STD_TSTRING s;
    IntToString( s, static_cast<int>(t), pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return STD_TSTRING the converted int
   */
  template<>
  inline STD_TSTRING ToString<LPCTSTR>( LPCTSTR l, LPCTSTR pszFormat )
  {
    STD_TSTRING s;
    StringToString( s, l, pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return STD_TSTRING the converted int
   */
  inline STD_TSTRING ToString( LPCTSTR l )
  {
    return l;
  }

  /**
   * Convert a double variables to a string with the option of a format.
   * @param double the number we are converting to a string.
   * @param LPCTSTR 
   * @return STD_TSTRING the converted int
   */
  template<>
  inline STD_TSTRING ToString<double>( double d, LPCTSTR pszFormat )
  {
    STD_TSTRING s;
    DoubleToString( s, d, pszFormat );
    return s;
  }

  /**
   * Convert a float variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return STD_TSTRING the converted int
   */
  template<>
  inline STD_TSTRING ToString<float>( float f, LPCTSTR pszFormat  )
  {
    STD_TSTRING s;
    FloatToString( s, f, pszFormat );
    return s;
  }

  STD_TSTRING ToStringFmt( LPCTSTR pszFormat, ... );

  /**
  * Convert many variables to a string.
  *
  * @param varied the number we are converting to a string.
  * @return STD_TSTRING the converted int
  */
  template<typename T>
  inline STD_TSTRING ToString( const T& x)
  {
    try
    {
#ifdef _UNICODE
      std::wostringstream o;
#else
      std::ostringstream o;
#endif
      if (!(o << x))
      {
        return _T("");
      }
      return o.str();
    }
    catch( ... )
    {
    }
    return _T( "" );
  }

  template<>
  inline STD_TSTRING ToString<STD_TSTRING>( const STD_TSTRING& x)
  {
    return x;
  }

  template<>
  inline STD_TSTRING ToString<LPTSTR>( const LPTSTR& x)
  {
    return x;
  }

#ifdef _UNICODE
  template<>
  inline STD_TSTRING ToString<LPSTR>( const LPSTR& x)
  {
    USES_CONVERSION;
    return A2T( x );
  }
  template<>
  inline STD_TSTRING ToString<std::string>( const std::string& x)
  {
    USES_CONVERSION;
    return A2T( x.c_str() );
  }
#endif

  // check if the given string is a 
  bool IsNumeric( const STD_TSTRING& s, bool allowDecimal = true);

  // Trims
  void Trim( STD_TSTRING& str, const TCHAR* chars = _T( " " ) );
  void TrimRight( STD_TSTRING& str, const TCHAR* chars );
  void TrimLeft( STD_TSTRING& str, const TCHAR* chars );

  LPTSTR _tcsistr(const STD_TSTRING& string, const STD_TSTRING& strCharSet);
  LPTSTR _tcsistr(LPCTSTR string, LPCTSTR strCharSet);

  wchar_t *char2wchar(const char * mbcs2Convert, UINT codepage = CP_ACP );

  template<typename T>
  size_t Length(T src);
} //  strings
} //  myodd

#   define T_MAX_PATH (MAX_PATH*sizeof(TCHAR))
#   ifdef _UNICODE
#     define T_A2T(p) A2T(p)
#     define T_T2A(p) T2A(p)
#   else
#     define T_A2T(p) (p)
#     define T_T2A(p) (p)
#   endif
#endif