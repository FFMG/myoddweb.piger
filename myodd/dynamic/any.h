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
          _llivalue = new long long int(*other._llivalue);

          // long double
          _ldvalue = new long double(*other._ldvalue);
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

        // guess the data type to see if we can handle it.
        _type = GuessType( value );
        switch ( Type() )
        {
        case dynamic::type_null:
          _llivalue = nullptr;
          _ldvalue = nullptr;
          return;

        case Boolean_bool:
          _llivalue = new long long int( value ? 1: 0 );
          _ldvalue = new long double( value ? 1 : 0 );
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
          throw std::exception("Dynamice type is not handled.");
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

        // if not null then we can set it.
        if (nullptr != value)
        {
          CastFrom(*value);
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
       * Try and guess the data type to convert to a dynamic::Type enum
       * We return type_unknown if not handled.
       * @param const T& the value we are trying to guess.
       * @return dynamic::Type the guessed type.
       */
      template<typename T>
      dynamic::Type GuessType(const T&) const
      {
        //
        if (std::is_same<T, short int>::value)
        {
          return dynamic::Integer_short_int;
        }

        //
        if (std::is_same<T, unsigned short int>::value)
        {
          return dynamic::Integer_unsigned_short_int;
        }

        //
        if (std::is_same<T, int>::value)
        {
          return dynamic::Integer_int;
        }

        //
        if (std::is_same<T, unsigned int>::value)
        {
          return dynamic::Integer_unsigned_int;
        }

        //
        if (std::is_same<T, long int>::value)
        {
          return dynamic::Integer_long_int;
        }

        //
        if (std::is_same<T, unsigned long int>::value)
        {
          return dynamic::Integer_unsigned_long_int;
        }
        
        //
        if (std::is_same<T, long long int>::value)
        {
          return dynamic::Integer_long_long_int;
        }

        //
        if (std::is_same<T, unsigned long long int>::value)
        {
          return dynamic::Integer_unsigned_long_long_int;
        }

        //
        if (std::is_same<T, double>::value)
        {
          return dynamic::Floating_point_double;
        }

        //
        if (std::is_same<T, float>::value)
        {
          return dynamic::Floating_point_float;
        }

        //
        if (std::is_same<T, long double>::value)
        {
          return dynamic::Floating_point_long_double;
        }

        //
        if (std::is_same<T, bool>::value)
        {
          return dynamic::Boolean_bool;
        }

        // we do not know how to handle this.
        return dynamic::type_unknown;
      }

      /**
       * Try and cast this to a posible value.
       * @return T the value we are looking for.
       */
      template<typename T>
      T CastTo() const
      {
        switch (_type)
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