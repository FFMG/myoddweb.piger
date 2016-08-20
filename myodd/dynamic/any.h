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
      * Return the data type
      * @return const Type& the data type.
      */
      const dynamic::Type& Type() const
      {
        return _type;
      }

    protected:
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
        switch (_type)
        {
        case dynamic::type_null:
          _llivalue = nullptr;
          _ldvalue = nullptr;
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

        default:
          break; 
        }

        // we cannot cast this.
        throw std::bad_cast();
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