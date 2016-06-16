#pragma once
#ifndef __ConfigData_h
#define __ConfigData_h 

#include "../common/includes.h"
#include "../string/string.h"

namespace myodd{ namespace config{

class Data
{
public:
  /**
   * The various types we are saving the config to/from
   */
  enum config_type
  {
    type_unknown= 0x00, 
    type_string = 0x01,
    type_long   = 0x02,
    type_double = 0x04,
    type_int    = 0x08,
    type_int64  = 0x10,
    type_all    = (type_string|type_string|type_long|type_double|type_int|type_int64),
  };

public:
  /**
   * Constructor
   * @param none
   * @return none
   */
  Data()
  {
    _Construct();
  }

  /**
   * copy Constructor
   * @see _Copy()
   * @see _Construct()
   * @param variable the value we are copying from.
   * @return none
   */
  template<typename ValueType>
  Data(const ValueType& value) 
  {
    _Construct();
    _Copy( value );
  }

  /**
   * copy Constructor
   * @see _Copy()
   * @see _Construct()
   * @param const Data& the value we are copying from.
   * @return none
   */
  Data(const Data& other)
  {
    _Construct();
    _Copy( other );
  }

  /**
   * The destructor
   * @param none
   * @return none
   */
  ~Data()
  {
  }

  /**
   * General copy Constructor
   * @see _Copy()
   * @see _Construct()
   * @param const Data& the value we are copying from.
   * @return const Data& *this
   */
  template<typename ValueType>
  Data& operator=(const ValueType& rhs)
  {
    _Copy( rhs );
    return (*this);
  }

  /**
   * Template Type cast
   * @see _get()
   * @param non
   * @return Varibale the value type that we wanted.
   */
  template<typename ValueType>
  operator ValueType() const
  {
    return _get<ValueType>( );
  }

  /**
   * Addition operator
   * @param const Data& the data we are adding to *this
   * @return Data the item added to.
   */
  Data operator+( const Data& data ) const
  {
    Data result( *this );
    result.m_dContainer = (result.m_dContainer + data.m_dContainer);
    return (result);
  }

  /**
   * Addition operator
   * @param const Data& the data we are adding to *this
   * @return Data the item added to.
   */
  template<typename ValueType>
  friend Data operator+( const ValueType& v, const Data& data )
  {
    Data dataV( v );
    dataV.m_dContainer = (dataV.m_dContainer + data.m_dContainer);
    return (dataV);
  }

  /**
   * Subtraction operator
   * @param const Data& the data we are adding to *this
   * @return Data the item subtracted to.
   */
  Data operator-( const Data& data ) const
  {
    Data result( *this );
    result.m_dContainer = (result.m_dContainer - data.m_dContainer);
    return (result);
  }

  /**
   * Substration operator
   * @param const Data& the data we are adding to *this
   * @return Data the item added to.
   */
  template<typename ValueType>
  friend Data operator-( const ValueType& v, const Data& data )
  {
    Data dataV( v );
    dataV.m_dContainer = (dataV.m_dContainer - data.m_dContainer);
    return (dataV);
  }

  /**
   * Adition operator
   * @param const Data& the data we are adding to *this
   * @return Data the item added to.
   */
  const Data& operator+=( const Data& data )
  {
    *this = (*this + data);
    return (*this);
  }

  /**
   * Substraction operator
   * @param const Data& the data we are adding to *this
   * @return Data the item substracted to.
   */
  const Data& operator-=( const Data& data )
  {
    *this = (*this - data);
    return (*this);
  }

public:
  /**
   * Get the current data type.
   * We save all the data types, but we have a 'prefered' one.
   * @return config_type the current type.
   */
  inline config_type type() const
  { 
    return m_ctType;
  }

  /**
   * Set a value and return if it was changed or not.
   * @param const Variable the value we are setting.
   * @return bool true|false if the value was changed or not. If nothing was changed we return false.
   */
  template<typename ValueType>
  bool SetValue( const ValueType& val )
  {
    // make it a Data object
    Data data = val;

    // and let it descide the return value.
    return SetValue( data );
  }

  /**
   * Set a value and return if it was changed or not.
   * @param const Data& the value we are setting.
   * @return bool true|false if the value was changed or not. If nothing was changed we return false.
   */
  template<>
  bool SetValue<Data>( const Data& val )
  {
    if( *this == val )
    {
      // they are the same.
      return false;
    }

    // set the value
    *this = val;

    // tell the system that it has changed.
    return true;
  }

  /**
   * Boolean operator.
   * @param const Data& the data we are comparing to
   * @return bool true|false the two values are not equal
   */
  inline bool operator!=( const Data& data ) const
  {
    //  return the oposite of ==
    return !(*this == data);
  }

  /**
   * Boolean operator.
   * @param const Data& the data we are comparing to
   * @return bool true|false the two values are equal
   */
  inline bool operator==( const Data& data ) const
  {
    return ( data.m_ctType == m_ctType && data.m_dContainer == m_dContainer );
  }

  /**
   * Boolean operator.
   * @param int the data we are comparing to
   * @return bool true|false the two values are not equal
   */
  inline bool operator!=(int data) const
  {
    //  return the oposite of ==
    return !(*this == data);
  }

  /**
   * Boolean operator.
   * @param double the data we are comparing to
   * @return bool true|false the two values are not equal
   */
  inline bool operator!=(double data) const
  {
    //  return the oposite of ==
    return !(*this == data);
  }

  /**
   * Boolean operator.
   * @param int the data we are comparing to
   * @return bool true|false the two values are equal
   */
  inline bool operator==(int data) const
  {
    switch (m_ctType)
    {
    case type_int:
      return (data == m_dContainer.iContent);

    case type_int64:
      return ((__int64)data == m_dContainer.i64Content ); // check that (__int64)1 == 123456789... rather than 1 == (int)123456789...

    case type_long:
      return ((long)data == m_dContainer.iContent);

    case type_double:
      return ((double)data == m_dContainer.dContent);     // check that (double)1 == 1.0 rather than 1 == (int)1.5

    case type_string:
      return m_dContainer.isNumeric && ((double)data == m_dContainer.dContent);  // same as above, if it is numerc, (else we might get "aaaa" == 0)

    default:
      return false;
    }
  }

  /**
  * Boolean operator.
  * @param int the data we are comparing to
  * @return bool true|false the two values are equal
  */
  inline bool operator==(double data) const
  {
    switch (m_ctType)
    {
    case type_int:
      return (data == (double)m_dContainer.iContent);

    case type_int64:
      return ((long double)data == (long double)m_dContainer.i64Content);

    case type_long:
      return (data == (double)m_dContainer.iContent);

    case type_double:
      return (data == m_dContainer.dContent);

    case type_string:
      return m_dContainer.isNumeric && (data == m_dContainer.dContent);

    default:
      return false;
    }
  }
protected:
  template<typename ValueType>
  ValueType _get( ) const
  {
    throw;  //  unknown type
    return m_dContainer.sContent;
  }
  /**
   * STD_TSTRING version of _get(...)
   * @see _get( ValueType& val )
   * @param STD_TSTRING the value we are getting.
   * @return STD_TSTRING the value.
   */
  template<>
  STD_TSTRING _get<STD_TSTRING>( ) const
  {
    return m_dContainer.sContent;
  }

  template<>
  LPCTSTR _get<LPCTSTR>( ) const
  {
    return m_dContainer.sContent.c_str();
  }

  /**
   * int version of _get(...)
   * @see _get( ValueType& val )
   * @param int the value we are getting.
   * @return int the value.
   */
  template<>
  int _get<int>( ) const
  {
    return m_dContainer.iContent;
  }

  /**
   * __int64 version of _get(...)
   * @see _get( ValueType& val )
   * @param int the value we are getting.
   * @return int the value.
   */
  template<>
  __int64 _get<__int64>( ) const
  {
    return m_dContainer.i64Content;
  }

  /**
   * unsigned __int64 version of _get(...)
   * @see _get( ValueType& val )
   * @param int the value we are getting.
   * @return int the value.
   */
  template<>
  unsigned __int64 _get<unsigned __int64>( ) const
  {
    return static_cast<unsigned __int64>(m_dContainer.i64Content);
  }

  /**
   * unsigned int version of _get(...)
   * @see _get( ValueType& val )
   * @param unsigned int the value we are getting.
   * @return unsigned int the value.
   */
  template<>
  unsigned int _get<unsigned int>( ) const
  {
    return (unsigned int)m_dContainer.iContent;
  }

  /**
   * short version of _get(...)
   * @see _get( ValueType& val )
   * @param short the value we are getting.
   * @return short the value.
   */
  template<>
  short _get<short>( ) const
  {
    return (short)m_dContainer.iContent;
  }

  /**
   * BYTE version of _get(...)
   * @see _get( ValueType& val )
   * @param BYTE the value we are getting.
   * @return BYTE the value.
   */
  template<>
  BYTE _get<BYTE>( ) const
  {
    return (BYTE)m_dContainer.iContent;
  }

  /**
   * bool version of _get(...)
   * @see _get( ValueType& val )
   * @param bool the value we are getting.
   * @return bool the value.
   */
  template<>
  bool _get<bool>( ) const
  {
    return (m_dContainer.iContent == 0 ? false : true );
  }

  /**
   * unsigned long version of _get(...)
   * @see _get( ValueType& val )
   * @param DWORD the value we are getting.
   * @return DWORD the value.
   */
  template<>
  unsigned long _get<unsigned long>( ) const
  {
    return (unsigned long)m_dContainer.lContent;
  }

  /**
   * long version of _get(...)
   * @see _get( ValueType& val )
   * @param long the value we are getting.
   * @return long the value.
   */
  template<>
  long _get<long>( ) const
  {
    return m_dContainer.lContent;
  }

  /**
   * float version of _get(...)
   * @see _get( ValueType& val )
   * @param float the value we are getting.
   * @return float the value.
   */
  template<>
  float _get<float>( ) const
  {
    return (float)m_dContainer.dContent;
  }

  /**
   * double version of _get(...)
   * @see _get( ValueType& val )
   * @param double the value we are getting.
   * @return double the value.
   */
  template<>
  double _get<double>( ) const
  {
    return m_dContainer.dContent;
  }

  /**
   * Structure that contains the data.
   * Only the basic types are stored as we can convert for all those values.
   */
  struct _Data
  {
    // the data
    STD_TSTRING sContent;
    int iContent;
    __int64 i64Content;
    double dContent;
    long lContent;
    bool isNumeric;

    /**
     * Constructor.
     * @see Reset()
     * @param none.
     * @return none.
     */
    _Data( )
    {
      Reset();
    }

    /**
     * Copy constructor.
     * @param const _Data& the Data we copying
     * @return none
     */
    _Data( const _Data& data )
    {
      *this = data;
    }

    /**
     * Boolean operator
     * @param const _Data& the Data we are comparing this to.
     * @return BOOL TRUE if the two values are not equal.
     */
    inline bool operator!=( const _Data& data ) const
    {
      return !((*this) == data);
    }

    /**
     * Boolean operator
     * @param const _Data& the Data we are comparing this to.
     * @return BOOL TRUE|FALSE if the two values are equal or not.
     */
    inline bool operator==( const _Data& data ) const
    {
      return (isNumeric == data.isNumeric &&
              iContent == data.iContent   &&
              dContent == data.dContent   &&
              lContent == data.lContent   &&
              i64Content == data.i64Content &&
              sContent == data.sContent );  // leave the string last
                                            // the code should bail out as soon as not equal.
    }

    /**
     * Destructor.
     * @param none.
     * @return none.
     */
    ~_Data( )
    {
    }

    /**
     * Set/Reset all the values.
     * @param none.
     * @return none.
     */
    void Reset()
    {
      sContent  = _T( "" );
      iContent  = 0;
      i64Content = 0;
      dContent  = 0;
      lContent  = 0;
      isNumeric = true;
    }

    /**
     * Substractions operator.
     * Note nothing happens to string values.
     * @param const _Data& the Data we subtracting from *this
     * @return const _Data& *this subtracted.
     */
    _Data operator-( const _Data& data ) const
    {
      _Data result( *this );
      result.iContent -= data.iContent;
      result.i64Content -= data.i64Content;
      result.dContent -= data.dContent;
      result.lContent -= data.lContent;

      if( result.isNumeric && data.isNumeric )
      {
        // because this is a number we must keep the value.
        // and not add the two numbers one after the other.
        result.sContent  = myodd::strings::ToString( result.dContent );

        // still a number.
        result.isNumeric = true;
      }
      else
      {
        // we cannot subsctrac anything...
        
        // but we know that this is no longer a number.
        result.isNumeric = false;
      }
      return (result);
    }

    /**
     * Adition operator
     * If we are adding to numeric values we will convert the string to the number
     * But if we are not then we will simply add the character at the end of it.
     * @param const _Data& the Data we substracting from *this
     * @return const _Data& *this added.
     */
    _Data operator+( const _Data& data ) const
    {
      _Data result( *this );
      result.iContent += data.iContent;
      result.dContent += data.dContent;
      result.lContent += data.lContent;
      result.i64Content += data.i64Content;

      if( result.isNumeric && data.isNumeric )
      {
        // because this is a number we must keep the value.
        // and not add the two numbers one after the other.
        result.sContent  = myodd::strings::ToString( result.dContent );

        // still a number.
        result.isNumeric = true;
      }
      else
      {
        // just add one after the other.
        result.sContent += data.sContent;

        // this is no longer a number.
        result.isNumeric = false;
      }
      return (result);
    }

    /**
     * Copy Constructor.
     * @param const _Data& the data we are setting to this
     * @return const _Data& *this
     */
    _Data& operator=( const _Data& data )
    {
      if( this != &data )
      {
        sContent = data.sContent;
        iContent = data.iContent;
        i64Content = data.i64Content;
        dContent = data.dContent;
        lContent = data.lContent;

        isNumeric= data.isNumeric;
      }
      return (*this);
    }
  };

  /**
   * Copy values to the data.
   * @param Variable the data we are setting our container to.
   * @return none
   */
  template<typename ValueType>
  void _Copy( const ValueType& v )
  {
    m_dContainer.sContent = myodd::strings::ToString( v );

    LPCTSTR lp = m_dContainer.sContent.c_str();
    m_ctType = Data::_GetType( typeid(v) );

    // check if this is indeed a number.
    m_dContainer.isNumeric = myodd::strings::IsNumeric( lp );
    m_dContainer.iContent = _tstoi( lp );
    m_dContainer.i64Content = _tstoi64( lp );
    m_dContainer.dContent = _tstof( lp );
    m_dContainer.lContent = _tstol( lp );
  }

  /**
   * Copy values to the data from another Data object.
   * @see _Copy( const ValueType& v )
   * @param const Data&
   * @return none
   */
  template<>
  void _Copy<Data>( const Data& other)
  {
    if( this != &other )
    {
      m_dContainer  = other.m_dContainer;
      m_ctType      = other.m_ctType;
    }
  }

  /**
   * Get the data type given the actual data type.
   * @param std::type_info& the value we will be setting the container type against.
   * @return config_type the configuration type for this ValueType
   */
  static config_type _GetType( const std::type_info& t )
  {
    // If you are tempted to get the name() of the typeid( variable )
    // be sure to read http://support.microsoft.com/kb/140670
    if( t == typeid(int) ||
        t == typeid(unsigned int) ||  //  same as - size_t
        t == typeid(BYTE)||
        t == typeid(bool) ||
        t == typeid(short)
       )
    {
      return type_int;
    }
    else if( t == typeid(__int64) )
    {
      return type_int64;
    }
    else if( t == typeid(float) ||
             t == typeid(double)
           )
    {
      return type_double;
    }
    else if( t == typeid(long) ||
             t == typeid(DWORD)
           )
    {
      return type_long;
    }
    else if( t == typeid(STD_TSTRING) ||
             t == typeid(LPCTSTR) ||
             t == typeid(LPTSTR)
#ifdef _UNICODE
          || t == typeid(LPCSTR) 
          || t == typeid(LPSTR) 
#endif
            )
    {
      return type_string;
    }

    //  this is not right.
    return type_string;
  }

public:
  /**
   * check if the value we have is valid or not.
   * when the default constructor is called then the value is not valid.
   *
   * This is to prevent is saving unused values.
   * @return bool if the value is valid or not.
   */
  bool IsValid( ) const
  {
    return ( m_ctType != type_unknown );
  }

private:
  /**
   * Set the default values for all the constructors.
   * @param none
   * @return none
   */
  void _Construct()
  {
    // We do not know the default value.
    m_ctType = type_unknown;

    // reset the value.
    m_dContainer.Reset();
  }

private:
  config_type m_ctType;
  _Data m_dContainer;
};

} //  config
} //  myodd
#endif //__ConfigData_h 