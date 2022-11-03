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

  /**
   * \brief get a random string given a length and a source
   * \param source the list of characters we will be randomly choosing from
   * \param len the total length we want the string t
   * \return a random string made of 'source' characters.
   */
  std::wstring RandomString(const std::wstring& source, int len);

  /**
   * \brief generate a Version 4 uuid
   * \see https://en.wikipedia.org/wiki/Universally_unique_identifier#Format for more details
   * \return the version 4 uuid.
   */
  std::wstring MakeUuid4();

  // check if a string is empty.
  bool IsEmptyString(const wchar_t* s );
  bool IsEmptyString( const std::wstring& s );

  bool IsEmptyString(const char* s);
  bool IsEmptyString(const std::string& s);

  // converst to lower case.
  const wchar_t lower( const wchar_t c );
  const std::wstring lower(const std::wstring& s);

  // search and replace
  std::wstring Replace( const std::wstring& haystack, const std::wstring& needle, const std::wstring& replace, bool caseSensitive = true );
  int32_t Find(const std::wstring& haystack, const std::wstring& needle, const uint32_t from = 0, bool caseSensitive = true );

  int32_t Compare( const std::wstring& lhs, const std::wstring& rhs, bool caseSensitive = true);

  /**
   * \brief Explode a string that is separated with '\0' chars.
   *        This is used with items like GetPrivateProfileString( ... ) when we want to list all the keys.
   * \param ret the return container.
   * \param s_keys the string with null chars '\0' finished by '\0\0'
   * \param nLen the length of the string.
   * \return int the number of items we exploded the string to.
   */
  unsigned int explode_by_null_char
    (
      std::vector<std::wstring>& ret,
      const wchar_t* s_keys,
      unsigned int nLen
    );

  /**
   * \brief Explode a given string given a delimiter string
   * \param ret the return container.
   * \param stringToExplode the string we want to explode
   * \param strDelimit Set of delimiter characters.
   * \param maxnumberOfElements the max number of items we want to return.
   *               If the limit parameter is zero, then this is treated as 1, (and only return one element).
   *               If the value is negative, we return the (total - n) number of elements, (from 0 to total - count)
   *                  If the negative number is greater than the total number of elements, we return nothing.
   * \param bAddEmpty if we want to add empty params or not.
   * \return the number of item that we found.
   */
  unsigned int Explode
    (
      std::vector<std::wstring>& ret,
      const std::wstring& stringToExplode,
      wchar_t strDelimit,
      int maxnumberOfElements =MYODD_MAX_INT32,
      bool bAddEmpty = true
    );

  // implode a string
  std::wstring implode
    (
    const std::vector<std::wstring> &s, 
    const std::wstring& glue,
    int iFromPiece = 0,
    int iToPiece = -1
    );

  // implode a string
  std::wstring implode
    (
    const std::wstring& glue,
    std::vector<std::wstring>::const_iterator itStart,
    std::vector<std::wstring>::const_iterator itEnd
    );

  // regex compare.
  bool wildcmp(const wchar_t* wild, const wchar_t* string);
  bool wildcmp(const std::wstring& wild, const std::wstring& string);

  bool IntToString( std::wstring& value, int i, const wchar_t* pszFormat );
  bool FloatToString( std::wstring& value, float f, const wchar_t* pszFormat );
  bool DoubleToString( std::wstring& value, double d, const wchar_t* pszFormat );
  bool StringToString( std::wstring& value, const wchar_t* s, const wchar_t* pszFormat );

  /**
   * Convert a double variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return std::wstring the converted int
   */
  template<typename T>
  inline std::wstring ToString( T t, const wchar_t* pszFormat  )
  {
    std::wstring s;
    IntToString( s, static_cast<int>(t), pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return std::wstring the converted int
   */
  template<>
  inline std::wstring ToString<const wchar_t*>(const wchar_t* l, const wchar_t* pszFormat )
  {
    std::wstring s;
    StringToString( s, l, pszFormat );
    return s;
  }

  /**
   * Allow a string to be formated.
   * @param double the number we are converting to a string.
   * @return std::wstring the converted int
   */
  inline std::wstring ToString(const wchar_t* l )
  {
    return l;
  }

  /**
   * Convert a double variables to a string with the option of a format.
   * @param double the number we are converting to a string.
   * @param const wchar_t* 
   * @return std::wstring the converted int
   */
  template<>
  inline std::wstring ToString<double>( double d, const wchar_t* pszFormat )
  {
    std::wstring s;
    DoubleToString( s, d, pszFormat );
    return s;
  }

  /**
   * Convert a float variables to a string with the option of a format.
   *
   * @param double the number we are converting to a string.
   * @return std::wstring the converted int
   */
  template<>
  inline std::wstring ToString<float>( float f, const wchar_t* pszFormat  )
  {
    std::wstring s;
    FloatToString( s, f, pszFormat );
    return s;
  }

  std::wstring Format(const wchar_t* pszFormat, ... );

  /**
  * Convert many variables to a string.
  *
  * @param varied the number we are converting to a string.
  * @return std::wstring the converted int
  */
  template<typename T>
  inline std::wstring ToString( const T& x)
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
        return L"";
      }
      return o.str();
    }
    catch( ... )
    {
    }
    return _T( "" );
  }

  template<>
  inline std::wstring ToString<std::wstring>( const std::wstring& x)
  {
    return x;
  }

  template<>
  inline std::wstring ToString<wchar_t*>( const LPTSTR& x)
  {
    return x;
  }

#ifdef _UNICODE
  template<>
  inline std::wstring ToString<LPSTR>( const LPSTR& x)
  {
    USES_CONVERSION;
    return A2T( x );
  }
  template<>
  inline std::wstring ToString<std::string>( const std::string& x)
  {
    USES_CONVERSION;
    return A2T( x.c_str() );
  }
#endif

  // check if the given string is a 
  bool IsNumeric( const std::wstring& s, bool allowDecimal = true);

  // Trims
  void Trim( std::wstring& str, const wchar_t* chars = _T( " " ) );
  void TrimRight( std::wstring& str, const wchar_t* chars );
  void TrimLeft( std::wstring& str, const wchar_t* chars );

  wchar_t* _tcsistr(const std::wstring& string, const std::wstring& strCharSet);
  wchar_t* _tcsistr(const wchar_t* string, const wchar_t* strCharSet);

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

#   define T_MAX_PATH (MAX_PATH*sizeof(wchar_t))
#   ifdef _UNICODE
#     define T_A2T(p) A2T(p)
#     define T_T2A(p) T2A(p)
#   else
#     define T_A2T(p) (p)
#     define T_T2A(p) (p)
#   endif
#endif