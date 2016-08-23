#pragma once

#include <typeinfo>       // std::bad_cast
#include "types.h"        // data type
#include <algorithm>      // memcpy
#include <string>

namespace myodd {
  namespace dynamic {
    class Any
    {
    public:
      /**
       * default constructor.
       */
      Any() :
        _llivalue( nullptr),
        _ldvalue( nullptr ),
        _cvalue( nullptr ),
        _lcvalue(0),
        _type( Type::type_null )
      {
      }

      /**
       * Copy constructor
       * @param const int& the value we want to copy/set
       */
      template<typename T>
      Any( const T& value) :
        Any()
      {
        CastFrom(value);
      }

      /**
      * Copy constructor
      * @param const int& the value we want to copy/set
      */
      template<typename T>
      Any( T* value) :
        Any()
      {
        CastFrom(value);
      }

      /**
       * Copy constructor
       */
      Any(const Any& any) :
        Any()
      {
        *this = any;
      }

      /**
       * Destructor.
       */
      virtual ~Any()
      {
        CleanValues();
      }

      /**
       * The T operator.
       * @return T the template operator.
       */
      template<typename T>
      operator T() const {
        return CastTo<T>();
      }

      /**
       * The equal operator
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      bool operator==(const Any& other) const
      {
        return (Compare( *this, other ) == 0);
      }

      /**
       * The friend equal operator.
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      template<typename T>
      friend bool operator==(const T& lhs , const Any& rhs)
      {
        return (Compare(Any(lhs), rhs ) == 0);
      }

      /**
      * The friend equal operator.
      * @param const Any &other the value we are comparing
      * @return bool if the values are equal
      */
      template<typename T>
      friend bool operator==(const Any& lhs, const T& rhs )
      {
        return (Compare(lhs, Any(rhs)) == 0);
      }

      /**
       * The not equal operator
       * @param const Any &other the value we are comparing
       * @return bool if the values are _not_ equal
       */
      bool operator!=(const Any &other) const
      {
        return (Compare(*this, other) != 0);
      }

      /**
       * The friend equal operator.
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      template<typename T>
      friend bool operator!=(const T& lhs , const Any& rhs)
      {
        return (Compare(Any(lhs), rhs ) != 0);
      }

      /**
       * Relational operator less than
       * @param const Any& rhs
       * @return bool if *this < rhs
       */
      bool operator< (const Any& rhs) const
      {
        // we use the double number as it is more precise
        if (Type() == dynamic::type_null && rhs.Type() == dynamic::type_null)
        {
          //  null is not smaller than null
          return false;
        }

        // is the lhs null?
        if (Type() == dynamic::type_null )
        {
          return 0 < *rhs._ldvalue;
        }

        // is the rhs null?
        if (rhs.Type() == dynamic::type_null)
        {
          return *_ldvalue < 0;
        }

        // neither values are null
        return (*_ldvalue < *rhs._ldvalue);
      }

      /**
       * Relational operator less than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs < rhs
       */
      template<typename T> friend bool operator< (const T& lhs, const Any& rhs) {
        return Any(lhs) < rhs;
      }

      /**
       * Relational operator greater than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs > rhs
       */
      template<typename T>  friend bool operator> (const T& lhs, const Any& rhs) { return rhs < lhs; }

      /**
       * Relational operator less or equal than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs <= rhs
       */
      template<typename T> friend bool operator<=(const T& lhs, const Any& rhs) { return !(lhs > rhs); }

      /**
       * Relational operator more or equal than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs >= rhs
       */
      template<typename T> friend bool operator>=(const T& lhs, const Any& rhs) { return !(lhs < rhs); }

      /**
       * Binary arithmetic operators - adition
       * @param const Any& the item we are adding to this.
       * @return Any& *this+rhs
       */
      Any& operator+=(const Any& rhs)
      {
        // add us to the other number and update our value.
        *this = *this + rhs;

        // return the result by reference
        return *this;
      }

      /**
      * Binary arithmetic operators - adition
      * @param Any the item we are adding to this.
      * @param const Any& the item we are adding to this.
      * @return Any *this+rhs
      */
      template<typename T> friend Any operator+(Any lhs, const T& rhs) { lhs += Any(rhs); return lhs; }

      /**
       * Binary arithmetic operators - adition
       * @param const Any& the item we are adding to this.
       * @return Any *this+rhs
       */
      Any operator+(const Any& rhs) const
      {
        // we use the double number as it is more precise
        if (Type() == dynamic::type_null && rhs.Type() == dynamic::type_null)
        {
          // null+null = int(0)
          return Any( (int)0 );
        }

        // is the lhs null if it is then we have to set the value?
        if (Type() == dynamic::type_null)
        {
          return Any(0) + rhs;
        }

        // is the rhs null if it is then we have to set the value?
        if (rhs.Type() == dynamic::type_null)
        {
          // nothing changes...
          // because this + null == this
          return *this + Any(0);
        }

        // add the values.
        Any value = (*_ldvalue + *rhs._ldvalue);

        // update the type.
        value._type = CalculateType( Type(), rhs.Type() );

        // return the value.
        return value;
      }

      /** 
       * The equal operator
       * @param const Any& other the value we are trying to set.
       * @return const Any& this value.
       */
      const Any& operator = (const Any& other)
      {
        if (this != &other)
        {
          // clear everything
          CleanValues();

          // copy the values over.
          _type = other._type;

          // long long int
          _llivalue = other._llivalue ? new long long int(*other._llivalue) : nullptr;

          // long double
          _ldvalue = other._ldvalue ? new long double(*other._ldvalue) : nullptr;

          // the char value
          if (other._lcvalue > 0 && other._cvalue)
          {
            _lcvalue = other._lcvalue;
            _cvalue = new char[_lcvalue];
            std::memset(_cvalue, 0, _lcvalue);
            std::memcpy(_cvalue, other._cvalue, _lcvalue);
          }
        }
        return *this;
      }

      /**
      * Return the data type
      * @return const Type& the data type.
      */
      const dynamic::Type& Type() const
      {
        return _type;
      }

    protected:
      dynamic::Type CalculateType(const dynamic::Type& lhsOriginal, const dynamic::Type& rhsOriginal) const
      {
        //  null values become ints.
        if (is_type_null(lhsOriginal))
        {
          return CalculateType(dynamic::Integer_int, rhsOriginal);
        }

        if (is_type_null(rhsOriginal))
        {
          return CalculateType( lhsOriginal, dynamic::Integer_int);
        }

        //  char values become ints.
        if (is_type_character(lhsOriginal))
        {
          return CalculateType(dynamic::Integer_int, rhsOriginal);
        }

        if (is_type_character(rhsOriginal))
        {
          return CalculateType(lhsOriginal, dynamic::Integer_int);
        }

        //  booleans values become ints.
        if (is_type_boolean(lhsOriginal))
        {
          return CalculateType(dynamic::Integer_int, rhsOriginal);
        }

        if (is_type_boolean(rhsOriginal))
        {
          return CalculateType(lhsOriginal, dynamic::Integer_int);
        }

        // if they are both the same, then use it.
        if (rhsOriginal == lhsOriginal )
        {
          return lhsOriginal;
        }

        // if lhs is floating and rhs is not, then use floating.
        if (is_type_floating(lhsOriginal) && !is_type_floating(rhsOriginal))
        {
          return lhsOriginal;
        }

        // same the other way around.
        if (!is_type_floating(lhsOriginal) && is_type_floating(rhsOriginal) )
        {
          return rhsOriginal;
        }

        // if we are here, they are both the same type, (floating/integer)
        // so we need to return the greatest of them both.
        return lhsOriginal;
      }

      /**
       * Compare 2 values and return 0 if they are both the same.
       * @param const Any& lhs the lhs value been compared.
       * @param const Any& rhs the lhs value been compared.
       * @return short, 0 if equal, -1 if type not the same, >=1 if value not same.
       */
      static short Compare(const Any& lhs, const Any& rhs)
      {
        // if the types are not the same
        if (lhs.Type() != rhs.Type())
        {
          return -1;
        }

        // special case for null
        switch (lhs.Type())
        {
        case dynamic::type_unknown:
        case dynamic::type_null:
          //  just compare the llivalue, they should be null anyway.
          return (lhs._llivalue == nullptr && lhs._llivalue == rhs._llivalue) ? 0 : 1;
        }

        // compare the values now.
        //
        if (*lhs._llivalue != *rhs._llivalue)
        {
          return 1;
        }

        // 
        if (*lhs._ldvalue != *rhs._ldvalue)
        {
          return 2;
        }

        //  null/not null
        if (lhs._cvalue && !rhs._cvalue)
        {
          return 3;
        }

        //  null/not null
        if (!lhs._cvalue && rhs._cvalue)
        {
          return 3;
        }

        //  null/not null
        if (lhs._cvalue && rhs._cvalue)
        {
          if (lhs._lcvalue != rhs._lcvalue)
          {
            return 4;
          }

          //  the lenght is the same, so we can use the size of lhs
          if (0 != std::memcmp(lhs._cvalue, rhs._cvalue, lhs._lcvalue))
          {
            return 4;
          }
        }

        // looks the same
        return 0;
      }

      /**
       * Try and create from a given value.
       * @throw std::bad_cast() if we are trying to create from an unknwon value.
       * @param const T& value the value we are trying to create from.
       */
      template<typename T>
      void CastFrom(const T& value)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;
        switch ( Type() )
        {
        case dynamic::type_null:
          _llivalue = nullptr;
          _ldvalue = nullptr;
          return;

        case dynamic::Boolean_bool:
          _llivalue = new long long int( value ? 1: 0 );
          _ldvalue = new long double( value ? 1 : 0 );
          return;

        // characters
        case dynamic::Character_char:
        case dynamic::Character_unsigned_char:
        case dynamic::Character_signed_char:
        {
          auto c = static_cast<char>(value);
          _lcvalue = sizeof T;
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, &c, _lcvalue);

          if (c >= '0' && c <= '9')
          {
            auto number = c - '0';
            _llivalue = new long long int(number);
            _ldvalue = new long double(number);
          }
          else
          {
            _llivalue = new long long int(0);
            _ldvalue = new long double(0);
          }
        }
        return;

        case dynamic::Character_wchar_t:
        {
          auto c = static_cast<wchar_t>(value);
          _lcvalue = sizeof T;
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, &c, _lcvalue);

          if (c >= L'0' && c <= L'9')
          {
            auto number = c - L'0';
            _llivalue = new long long int(number);
            _ldvalue = new long double(number);
          }
          else
          {
            _llivalue = new long long int(0);
            _ldvalue = new long double(0);
          }
        }
        return;

        // int
        case dynamic::Integer_short_int:
        case dynamic::Integer_unsigned_short_int:
        case dynamic::Integer_int:
        case dynamic::Integer_unsigned_int:
        case dynamic::Integer_long_int:
        case dynamic::Integer_unsigned_long_int:
        case dynamic::Integer_long_long_int:
        case dynamic::Integer_unsigned_long_long_int:
          // we can cast those into long/long
          _llivalue = new long long int(static_cast<long long int>(value));
          _ldvalue = new long double(static_cast<long double>(*_llivalue));
          return;

        // double
        case dynamic::Floating_point_double:
        case dynamic::Floating_point_float:
        case dynamic::Floating_point_long_double:
          // we can cast those into long/long
          _ldvalue = new long double(static_cast<long double>(value));
          _llivalue = new long long int(static_cast<long long int>(*_ldvalue));
          return;

        case dynamic::type_unknown:
          break;

        default:
          // is this a new type that we are not handling?
          throw std::exception("Dynamic type is not handled.");
        }

        // we could not deduce the value from this.
        throw std::bad_cast();
      }

      /**
       * Try and create from a given value.
       * @throw std::bad_cast() if we are trying to create from an unknwon value.
       * @param const T& value the value we are trying to create from.
       */
      template<typename T>
      void CastFrom(T* value)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;

        // if not null then we can set it.
        if (nullptr != value)
        {
          switch (Type())
          {
            // characters
          case dynamic::Character_char:
          case dynamic::Character_unsigned_char:
          case dynamic::Character_signed_char:
          {
            const char* c = (const char*)value;

            // default values.
            _lcvalue = std::strlen(c) + sizeof T;
            _cvalue = new char[_lcvalue];
            std::memset(_cvalue, 0, _lcvalue);
            std::memcpy(_cvalue, c, _lcvalue);

            _llivalue = new long long int(std::strtoll(c, nullptr, 0));
            _ldvalue = new long double(std::strtold(c,nullptr));
          }
          break;

          case dynamic::Character_wchar_t:
          {
          }
          break;

          default:
            CastFrom(*value);
            break;
          }
        }
        else
        {
          // if it is null we must still set the type, but default the values to zeros.
          switch ( Type() )
          {
          case dynamic::type_null:
            _llivalue = nullptr;
            _ldvalue = nullptr;
            return;

          // boolean
          case dynamic::Boolean_bool:

          // characters
          case dynamic::Character_char:
          case dynamic::Character_unsigned_char:
          case dynamic::Character_signed_char:
          {
            const char c = '\0';

            // default values.
            _lcvalue = sizeof T;
            _cvalue = new char[_lcvalue];
            std::memset(_cvalue, 0, _lcvalue);
            std::memcpy(_cvalue, &c, _lcvalue);

            _llivalue = new long long int(0);
            _ldvalue = new long double(0);
          }
          return;

          case dynamic::Character_wchar_t:
          {
            // default values.
            const wchar_t w = L'\0';
            _lcvalue = sizeof w;
            _cvalue = new char[_lcvalue];
            std::memset(_cvalue, 0, _lcvalue);
            std::memcpy(_cvalue, &w, _lcvalue);

            _llivalue = new long long int(0);
            _ldvalue = new long double(0);
          }
          return;

          // int
          case dynamic::Integer_short_int:
          case dynamic::Integer_unsigned_short_int:
          case dynamic::Integer_int:
          case dynamic::Integer_unsigned_int:
          case dynamic::Integer_long_int:
          case dynamic::Integer_unsigned_long_int:
          case dynamic::Integer_long_long_int:
          case dynamic::Integer_unsigned_long_long_int:
          case dynamic::Floating_point_double:
          case dynamic::Floating_point_float:
          case dynamic::Floating_point_long_double:
            // default values.
            _llivalue = new long long int(0);
            _ldvalue = new long double(0);
            return;

          case dynamic::type_unknown:
            break;

          default:
            // is this a new type that we are not handling?
            throw std::exception("Dynamic type is not handled.");
          }

          // we could not deduce the value from this.
          throw std::bad_cast();
        }
      }

      /**
      * Try and create from a given value.
      * @throw std::bad_cast() if we are trying to create from an unknwon value.
      * @param const T& value the value we are trying to create from.
      */
      void CastFrom(nullptr_t)
      {
        // clear all the values.
        CleanValues();
      }

      /**
       * Try and cast this to a posible value.
       * @return T the value we are looking for.
       */
      template<typename T>
      T CastTo() const
      {
        switch ( Type() )
        {
        case dynamic::Type::type_null:
          return 0;

        // char
        case dynamic::Type::Character_char:
        case dynamic::Type::Character_unsigned_char:
        case dynamic::Type::Character_signed_char:
        case dynamic::Type::Character_wchar_t:
          if (dynamic::is_type_floating(dynamic::get_type<T>::value))
          {
            return static_cast<T>(*_ldvalue);
          }
          return static_cast<T>(*_llivalue);

        // Integer
        case dynamic::Type::Integer_int:
        case dynamic::Type::Integer_long_int:
        case dynamic::Type::Integer_unsigned_long_int:
        case dynamic::Type::Integer_long_long_int:
        case dynamic::Type::Integer_unsigned_long_long_int:
          return static_cast<T>(*_llivalue);

        case dynamic::Type::Floating_point_double:
        case dynamic::Type::Floating_point_float:
        case dynamic::Type::Floating_point_long_double:
          return static_cast<T>(*_ldvalue);

        case dynamic::Type::Boolean_bool:
          return static_cast<T>(*_ldvalue);

        default:
          break; 
        }

        // we cannot cast this.
        throw std::bad_cast();
      }

      /**
       * Try and cast this to a posible value.
       * @return char* the value we are looking for.
       */
      template<>
      const char* CastTo<const char*>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          throw std::bad_cast();

        case dynamic::Type::Character_char:
        case dynamic::Type::Character_signed_char:
        case dynamic::Type::Character_unsigned_char:
          return static_cast<char*>(_cvalue);
        }

        if (dynamic::is_type_floating(Type()))
        {

        }
        else
        {
          const char *fmt = "%i";
          auto lcvalue = std::snprintf(nullptr, 0, fmt, *_llivalue);
          auto cvalue = new char[lcvalue+1];
          std::memset(cvalue, 0, lcvalue);
          std::snprintf(cvalue, lcvalue, fmt, *_llivalue);
        }
        return static_cast<char*>(_cvalue);
      }

      /**
       * Try and cast this to a posible value.
       * @return char the value we are looking for.
       */
      template<>
      char CastTo<char>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          return static_cast<char>(*(wchar_t*)_cvalue);

        case dynamic::Type::Character_char:
          return static_cast<char>(*(char*)_cvalue);

        case dynamic::Type::Character_signed_char:
          return static_cast<char>(*(signed char*)_cvalue);

        case dynamic::Type::Character_unsigned_char:
          return static_cast<char>(*(unsigned char*)_cvalue);
        }
        return static_cast<char>(*_llivalue);
      }

      /**
      * Try and cast this to a posible value.
      * @return wchar_t the value we are looking for.
      */
      template<>
      wchar_t CastTo<wchar_t>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          return static_cast<wchar_t>(*(wchar_t*)_cvalue);

        case dynamic::Type::Character_char:
          return static_cast<wchar_t>(*(char*)_cvalue);

        case dynamic::Type::Character_signed_char:
          return static_cast<wchar_t>(*(signed char*)_cvalue);

        case dynamic::Type::Character_unsigned_char:
          return static_cast<wchar_t>(*(unsigned char*)_cvalue);
        }
        return static_cast<wchar_t>(*_llivalue);
      }

      /**
       * Try and cast this to a posible value.
       * @return unsigned char the value we are looking for.
       */
      template<>
      unsigned char CastTo<unsigned char>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          return static_cast<unsigned char>(*(wchar_t*)_cvalue);

        case dynamic::Type::Character_char:
          return static_cast<unsigned char>(*(char*)_cvalue);

        case dynamic::Type::Character_signed_char:
          return static_cast<unsigned char>(*(signed char*)_cvalue);

        case dynamic::Type::Character_unsigned_char:
          return static_cast<unsigned char>(*(unsigned char*)_cvalue);
        }
        return static_cast<unsigned char>(*_llivalue);
      }

      /**
      * Try and cast this to a posible value.
      * @return unsigned char the value we are looking for.
      */
      template<>
      signed char CastTo<signed char>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          return static_cast<signed char>(*(wchar_t*)_cvalue);

        case dynamic::Type::Character_char:
          return static_cast<signed char>(*(char*)_cvalue);

        case dynamic::Type::Character_signed_char:
          return static_cast<signed char>(*(signed char*)_cvalue);

        case dynamic::Type::Character_unsigned_char:
          return static_cast<signed char>(*(unsigned char*)_cvalue);
        }
        return static_cast<signed char>(*_llivalue);
      }

      /**
       * Try and cast this to a posible value.
       * @return bool the value we are looking for.
       */
      template<>
      bool CastTo<bool>() const
      {
        switch (Type())
        {
        case dynamic::Type::type_null:
          return false;
        }

        //  value to true/false
        return (*_llivalue != 0 ? true : false);
      }

      /**
       * Clean up the value(s)
       */
      void CleanValues()
      {
        //  delete long long integer if need be
        delete _llivalue;

        //  delete long double if need be
        delete _ldvalue;

        // delete the char if need be
        delete _cvalue;

        // reset the values
        _llivalue = nullptr;
        _ldvalue = nullptr;
        _cvalue = nullptr;
      }

      // the biggest integer value.
      long long int* _llivalue;

      // the biggest floating point value
      long double* _ldvalue;

      // the char
      char* _cvalue;
      size_t _lcvalue;

      // the variable type
      dynamic::Type _type;
    };
  }
}