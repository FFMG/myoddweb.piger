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
  bool IsEmptyString( const MYODD_STRING& s );

  // converst to lower case.
  TCHAR lower(TCHAR c );
  MYODD_STRING lower(const MYODD_STRING& s);

  // search and replace
  MYODD_STRING replace
    (
    const MYODD_STRING& origStr,
    const MYODD_STRING& srchStr, 
    const MYODD_STRING& replaceStr
    );

  void replace_inplace
    (
    MYODD_STRING& origStr,
    const MYODD_STRING& srchStr, 
    const MYODD_STRING& replaceStr
    );

  // search and ireplace
  MYODD_STRING ireplace
    (
    const MYODD_STRING& origStr,
    const MYODD_STRING& srchStr, 
    const MYODD_STRING& replaceStr
    );

  void ireplace_inplace
    (
    MYODD_STRING& origStr,
    const MYODD_STRING& srchStr, 
    const MYODD_STRING& replaceStr
    );

  size_t ifind( const MYODD_STRING& origStr, const MYODD_STRING& srchStr, const size_t nFrom = 0 );
  int icompare( const MYODD_STRING& origStr, const MYODD_STRING& srchStr );

  // explode with a '\0' char
  size_t explode_by_null_char
    (
    std::vector<MYODD_STRING>& ret,
    LPCTSTR s_keys, 
    size_t nLen
    );

  // explode a string with a set of delimiter characters
  size_t explode
    (
    std::vector<MYODD_STRING>& ret,
    const MYODD_STRING& s, 
    TCHAR strDelimit, 
    int nCount =-1,
    bool bAddEmpty = true
    );

  // implode a string
  MYODD_STRING implode
    (
    const std::vector<MYODD_STRING> &s, 
    const MYODD_STRING& glue,
    int iFromPiece = 0,
    int iToPiece = -1
    );

  // implode a string
  MYODD_STRING implode
    (
    const MYODD_STRING& glue,
    std::vector<MYODD_STRING>::const_iterator itStart,
    std::vector<MYODD_STRING>::const_iterator itEnd
    );

  // regex compare.
  bool wildcmp(LPCTSTR wild, LPCTSTR string);
  bool wildcmp(const MYODD_STRING& wild, const MYODD_STRING& string);

  bool IntToString( MYODD_STRING& value, int i, LPCTSTR pszFormat );
  bool FloatToString( MYODD_STRING& value, float f, LPCTSTR pszFormat );
  bool DoubleToString( MYODD_STRING& value, double d, LPCTSTR pszFormat );
  bool StringToString( MYODD_STRING& value, LPCTSTR s, LPCTSTR pszFormat );

  /**
   * Convert a double variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return MYODD_STRING the converted int
   */
  template<typename T>
  inline MYODD_STRING ToString( T t, LPCTSTR pszFormat  )
  {
    MYODD_STRING s;
    IntToString( s, static_cast<int>(t), pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return MYODD_STRING the converted int
   */
  template<>
  inline MYODD_STRING ToString<LPCTSTR>( LPCTSTR l, LPCTSTR pszFormat )
  {
    MYODD_STRING s;
    StringToString( s, l, pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return MYODD_STRING the converted int
   */
  inline MYODD_STRING ToString( LPCTSTR l )
  {
    return l;
  }

  /**
   * Convert a double variables to a string with the option of a format.
   * @param double the number we are converting to a string.
   * @param LPCTSTR 
   * @return MYODD_STRING the converted int
   */
  template<>
  inline MYODD_STRING ToString<double>( double d, LPCTSTR pszFormat )
  {
    MYODD_STRING s;
    DoubleToString( s, d, pszFormat );
    return s;
  }

  /**
   * Convert a float variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return MYODD_STRING the converted int
   */
  template<>
  inline MYODD_STRING ToString<float>( float f, LPCTSTR pszFormat  )
  {
    MYODD_STRING s;
    FloatToString( s, f, pszFormat );
    return s;
  }

  MYODD_STRING ToStringFmt( LPCTSTR pszFormat, ... );

  /**
  * Convert many variables to a string.
  *
  * @param varied the number we are converting to a string.
  * @return MYODD_STRING the converted int
  */
  template<typename T>
  inline MYODD_STRING ToString( const T& x)
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
  inline MYODD_STRING ToString<MYODD_STRING>( const MYODD_STRING& x)
  {
    return x;
  }

  template<>
  inline MYODD_STRING ToString<LPTSTR>( const LPTSTR& x)
  {
    return x;
  }

#ifdef _UNICODE
  template<>
  inline MYODD_STRING ToString<LPSTR>( const LPSTR& x)
  {
    USES_CONVERSION;
    return A2T( x );
  }
  template<>
  inline MYODD_STRING ToString<std::string>( const std::string& x)
  {
    USES_CONVERSION;
    return A2T( x.c_str() );
  }
#endif

  // check if the given string is a 
  bool IsNumeric( const MYODD_STRING& s, bool allowDecimal = true);

  // Trims
  void Trim( MYODD_STRING& str, const TCHAR* chars = _T( " " ) );
  void TrimRight( MYODD_STRING& str, const TCHAR* chars );
  void TrimLeft( MYODD_STRING& str, const TCHAR* chars );

  LPTSTR _tcsistr(const MYODD_STRING& string, const MYODD_STRING& strCharSet);
  LPTSTR _tcsistr(LPCTSTR string, LPCTSTR strCharSet);

  template<typename T>
  size_t Length(T src);

  std::wstring String2WString( const std::string& s2Convert );
  std::string WString2String(const std::wstring& ws2Convert);
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