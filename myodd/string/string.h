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
  bool IsEmptyString(const wchar_t* s );
  bool IsEmptyString( const std::wstring& s );

  bool IsEmptyString(const char* s);
  bool IsEmptyString(const std::string& s);

  // converst to lower case.
  MYODD_CHAR lower(MYODD_CHAR c );
  MYODD_STRING lower(const MYODD_STRING& s);

  // search and replace
  MYODD_STRING Replace( const MYODD_STRING& haystack, const MYODD_STRING& needle, const MYODD_STRING& replace, bool caseSensitive = true );
  int32_t Find(const MYODD_STRING& haystack, const MYODD_STRING& needle, const uint32_t from = 0, bool caseSensitive = true );

  int32_t Compare( const MYODD_STRING& lhs, const MYODD_STRING& rhs, bool caseSensitive = true);

  // explode with a '\0' char
  size_t explode_by_null_char
    (
    std::vector<MYODD_STRING>& ret,
    const MYODD_CHAR* s_keys,
    size_t nLen
    );

  // explode a string with a set of delimiter characters
  size_t Explode
    (
    std::vector<MYODD_STRING>& ret,
    const MYODD_STRING& s, 
    MYODD_CHAR strDelimit,
    int nCount = MYODD_MAX_INT32,
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
  bool wildcmp(const MYODD_CHAR* wild, const MYODD_CHAR* string);
  bool wildcmp(const MYODD_STRING& wild, const MYODD_STRING& string);

  bool IntToString( MYODD_STRING& value, int i, const MYODD_CHAR* pszFormat );
  bool FloatToString( MYODD_STRING& value, float f, const MYODD_CHAR* pszFormat );
  bool DoubleToString( MYODD_STRING& value, double d, const MYODD_CHAR* pszFormat );
  bool StringToString( MYODD_STRING& value, const MYODD_CHAR* s, const MYODD_CHAR* pszFormat );

  /**
   * Convert a double variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return MYODD_STRING the converted int
   */
  template<typename T>
  inline MYODD_STRING ToString( T t, const MYODD_CHAR* pszFormat  )
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
  inline MYODD_STRING ToString<const MYODD_CHAR*>(const MYODD_CHAR* l, const MYODD_CHAR* pszFormat )
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
  inline MYODD_STRING ToString(const MYODD_CHAR* l )
  {
    return l;
  }

  /**
   * Convert a double variables to a string with the option of a format.
   * @param double the number we are converting to a string.
   * @param const MYODD_CHAR* 
   * @return MYODD_STRING the converted int
   */
  template<>
  inline MYODD_STRING ToString<double>( double d, const MYODD_CHAR* pszFormat )
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
  inline MYODD_STRING ToString<float>( float f, const MYODD_CHAR* pszFormat  )
  {
    MYODD_STRING s;
    FloatToString( s, f, pszFormat );
    return s;
  }

  MYODD_STRING Format(const MYODD_CHAR* pszFormat, ... );

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
  inline MYODD_STRING ToString<MYODD_CHAR*>( const LPTSTR& x)
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
  void Trim( MYODD_STRING& str, const MYODD_CHAR* chars = _T( " " ) );
  void TrimRight( MYODD_STRING& str, const MYODD_CHAR* chars );
  void TrimLeft( MYODD_STRING& str, const MYODD_CHAR* chars );

  MYODD_CHAR* _tcsistr(const MYODD_STRING& string, const MYODD_STRING& strCharSet);
  MYODD_CHAR* _tcsistr(const MYODD_CHAR* string, const MYODD_CHAR* strCharSet);

  // Get the string length
  template<typename T>
  size_t Length(T src);
  template<>
  size_t Length<const std::string&>(const std::string& src);
  template<>
  size_t Length<const char*>(const char* src);
  template<>
  size_t Length<const std::wstring&>(const std::wstring& src);
  template<>
  size_t Length<const wchar_t*>(const wchar_t* src);

  std::wstring String2WString( const std::string& s2Convert );
  std::string WString2String(const std::wstring& ws2Convert);
} //  strings
} //  myodd

#   define T_MAX_PATH (MAX_PATH*sizeof(MYODD_CHAR))
#   ifdef _UNICODE
#     define T_A2T(p) A2T(p)
#     define T_T2A(p) T2A(p)
#   else
#     define T_A2T(p) (p)
#     define T_T2A(p) (p)
#   endif
#endif