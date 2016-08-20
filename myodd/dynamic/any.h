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
        _llvalue(nullptr),
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
          _llvalue = nullptr;
          return;

        // int
        case dynamic::Integer_short_int:
        case dynamic::Integer_unsigned_short_int:
        case dynamic::Integer_int:
        case dynamic::Integer_unsigned_int:
        case dynamic::Integer_long_int:
        case dynamic::Integer_unsigned_long_int:
        case dynamic::type_longlong:
          // we can cast those into long/long
          _llvalue = new long long(value); 
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
        
        if (std::is_same<T, long long>::value)
        {
          return dynamic::type_longlong;
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
        case dynamic::Type::type_longlong:
          return static_cast<T>(*_llvalue);

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
        //  delete if need be
        if (_llvalue)
        {
          delete _llvalue;
        }

        // reset the values
        _llvalue = nullptr;
      }

      // the actual value
      long long* _llvalue;

      // the variable type
      dynamic::Type _type;
    };
  }
}