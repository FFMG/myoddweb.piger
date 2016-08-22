#pragma once

#include <typeinfo>       // std::bad_cast
#include "types.h"        // data type

namespace myodd {
  namespace dynamic {
    class Any
    {
    public:
      /**
       * default constructor.
       */
      Any() :
        _llivalue(nullptr),
        _ldvalue( nullptr ),
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
          // null+null = null
          return *this;
        }

        // is the lhs null if it is then we have to set the value?
        if (Type() == dynamic::type_null)
        {
          Any value = rhs;
          value._type = CalculateType(Type(), rhs.Type());
          return value;
        }

        // is the rhs null if it is then we have to set the value?
        if (rhs.Type() == dynamic::type_null)
        {
          // nothing changes...
          // because this + null == this
          Any value = *this;
          value._type = CalculateType(Type(), rhs.Type());
          return value;
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
        case dynamic::Character_wchar_t:

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
          CastFrom(*value);
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
          case dynamic::Character_wchar_t:

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

        case dynamic::Type::Character_char:
        case dynamic::Type::Character_unsigned_char:
        case dynamic::Type::Character_signed_char:
          return static_cast<T>(*_ldvalue);

        default:
          break; 
        }

        // we cannot cast this.
        throw std::bad_cast();
      }


      /**
       * Try and cast this to a posible value.
       * @return bool the value we are looking for.
       */
      template<>
      bool CastTo<bool>() const
      {
        switch (_type)
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

        // reset the values
        _llivalue = nullptr;
        _ldvalue = nullptr;
      }

      // the biggest integer value.
      long long int* _llivalue;

      // the biggest floating point value
      long double* _ldvalue;

      // the variable type
      dynamic::Type _type;
    };
  }
}