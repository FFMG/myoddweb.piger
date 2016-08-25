#pragma once

#include <typeinfo>       // std::bad_cast
#include <algorithm>      // memcpy
#include <math.h>         // modf
#include <string>
#include <codecvt>        //  string <-> wstring

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
        _llivalue(0),
        _ldvalue(0),
        _cvalue(nullptr),
        _svalue(nullptr),
        _swvalue(nullptr),
        _lcvalue(0),
        _type(Type::Misc_null)
      {
      }

      /**
       * Copy constructor
       * @param const int& the value we want to copy/set
       */
      template<class T>
      Any(const T& value) :
        Any()
      {
        CastFrom(value);
      }

      /**
       * Copy constructor
       * @param const int& the value we want to copy/set
       */
      template<class T>
      Any(T* value) :
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
      template<class T>
      operator T() const {
        return CastTo<T>();
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
          _llivalue = other._llivalue;

          // long double
          _ldvalue = other._ldvalue;

          // copy the character value
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
       * The equal operator
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      bool operator==(const Any& other) const
      {
        return (Compare(*this, other) == 0);
      }

      /**
       * The friend equal operator.
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      template<class T>
      friend bool operator==(const T& lhs, const Any& rhs)
      {
        return (Compare(Any(lhs), rhs) == 0);
      }

      /**
       * The friend equal operator.
       * @param const Any &other the value we are comparing
       * @return bool if the values are equal
       */
      template<class T>
      friend bool operator==(const Any& lhs, const T& rhs)
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
      template<class T>
      friend bool operator!=(const T& lhs, const Any& rhs)
      {
        return (Compare(Any(lhs), rhs) != 0);
      }

      /**
       * Relational operator less than
       * @param const Any& rhs
       * @return bool if *this < rhs
       */
      bool operator< (const Any& rhs) const
      {
        // we use the double number as it is more precise
        if (Type() == dynamic::Misc_null && rhs.Type() == dynamic::Misc_null)
        {
          //  null is not smaller than null
          return false;
        }

        // is the lhs null?
        if (Type() == dynamic::Misc_null)
        {
          return 0 < rhs._ldvalue;
        }

        // is the rhs null?
        if (rhs.Type() == dynamic::Misc_null)
        {
          return _ldvalue < 0;
        }

        // neither values are null
        return (_ldvalue < rhs._ldvalue);
      }

      /**
       * Relational operator less than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs < rhs
       */
      template<class T> friend bool operator< (const T& lhs, const Any& rhs) {
        return Any(lhs) < rhs;
      }

      /**
       * Relational operator greater than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs > rhs
       */
      template<class T>  friend bool operator> (const T& lhs, const Any& rhs) { return rhs < lhs; }

      /**
       * Relational operator less or equal than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs <= rhs
       */
      template<class T> friend bool operator<=(const T& lhs, const Any& rhs) { return !(lhs > rhs); }

      /**
       * Relational operator more or equal than
       * @param const T& lhs
       * @param const Any& rhs
       * @return bool if lhs >= rhs
       */
      template<class T> friend bool operator>=(const T& lhs, const Any& rhs) { return !(lhs < rhs); }

      //
      // +operators
      //  
      #pragma region
      /**
 * Binary arithmetic operators - addition
 * @param const Any& the item we are adding to this.
 * @return Any& *this+rhs
 */
      Any& operator+=(const Any& rhs)
      {
        if (dynamic::is_type_floating(rhs.Type()))
        {
          return AddNumber(CalculateType(Type(), rhs.Type()), rhs._ldvalue);
        }
        return AddNumber(CalculateType(Type(), rhs.Type()), rhs._llivalue);
      }

      /**
       * Default += function add the rhs to *this.
       * @param const T& rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<class T>
      Any& operator+=(T rhs)
      {
        *this += Any(rhs);
        return *this;
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param short rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(short int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned short int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned short int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned long int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param long long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long long int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned long long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned long long int rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param float rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(float rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param double rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(double rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param float rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long double rhs)
      {
        return AddNumber(CalculateType(Type(), dynamic::Floating_point_long_double), rhs);
      }

      /**
       * Binary arithmetic operators - addition
       * @param const Any& the item we are adding to this.
       * @return Any *this+rhs
       */
      Any operator+(const Any& rhs) const
      {
        // copy the value
        Any value = *this;

        // add the rhs
        value += rhs;

        // return the result.
        return value;
      }

      /**
       * Binary arithmetic operators - addition
       * @param Any the item we are adding to this.
       * @param const Any& the item we are adding to this.
       * @return Any *this+rhs
       */
      template<class T> friend Any operator+(Any lhs, const T& rhs) { lhs += rhs; return lhs; }

      /**
       * Add one to the current value.
       * @return Any *this +1
       */
      Any& operator++()
      {
        // save the current type.
        dynamic::Type type = Type();

        // multiply the values.
        CastFrom(_ldvalue + 1);

        // update the type.
        _type = CalculateType(type, dynamic::Integer_int);

        // return this.
        return *this;
      }

      /**
       * Add one to the current value.
       * @return Any *this +1
       */
      Any operator++(int)
      {
        Any tmp(*this);
        operator++();
        return tmp;
      }

      //
      // +operators
      //
      #pragma endregion +=operators

      //
      // -operators
      //  
      #pragma region
      /**
       * Binary arithmetic operators - substraction
       * @param const Any& the item we are subtracting to this.
       * @return Any& *this-rhs
       */
      Any& operator-=(const Any& rhs)
      {
        if (dynamic::is_type_floating(rhs.Type()))
        {
          return SubtractNumber(CalculateType(Type(), rhs.Type()), rhs._ldvalue);
        }
        return SubtractNumber(CalculateType(Type(), rhs.Type()), rhs._llivalue);
      }

      /**
       * Default -= function substract the rhs to *this.
       * @param const T& rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<class T>
      Any& operator-=(T rhs)
      {
        *this -= Any(rhs);
        return *this;
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param short rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(short int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned short int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned short int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned long int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param long long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long long int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned long long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned long long int rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param float rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(float rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param double rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(double rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param float rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long double rhs)
      {
        return SubtractNumber(CalculateType(Type(), dynamic::Floating_point_long_double), rhs);
      }

      /**
       * Binary arithmetic operators - substraction
       * @param const Any& the item we are subtracting from *this.
       * @return Any *this-rhs
       */
      Any operator-(const Any& rhs) const
      {
        // copy the value
        Any value = *this;

        // substract the rhs
        value -= rhs;

        // return the result.
        return value;
      }

      /**
       * Binary arithmetic operators - substraction
       * @param Any the item we are subtracting from *this.
       * @param const Any& the item we are subtracting from *this.
       * @return Any *this-rhs
       */
      template<class T> friend Any operator-(Any lhs, const T& rhs) { lhs -= rhs; return lhs; }

      /**
       * substract one from the current value.
       * @return Any *this -1
       */
      Any& operator--()
      {
        // save the current type.
        dynamic::Type type = Type();

        // multiply the values.
        CastFrom(_ldvalue - 1);

        // update the type.
        _type = CalculateType(type, dynamic::Integer_int);

        // return this.
        return *this;
      }

      /**
       * substract one from the current value.
       * @return Any *this -1
       */
      Any operator--(int)
      {
        Any tmp(*this);
        operator--();
        return tmp;
      }

      //
      // -operators
      //
      #pragma endregion -=operators
      
      //
      // *operators
      //  
      #pragma region
      /**
       * Binary arithmetic operators - multiplication
       * @param const Any& the item we are multiplying from this.
       * @return Any& *this*rhs
       */
      Any& operator*=(const Any& rhs)
      {
        // save the current type.
        dynamic::Type type = Type();

        // multiply the values.
        CastFrom(_ldvalue * rhs._ldvalue);

        // update the type.
        _type = CalculateType(type, rhs.Type());

        // return the value.
        return *this;
      }

      /**
       * Binary arithmetic operators - multiplication
       * @param const Any& the item we are multiplying from this.
       * @return Any *this*rhs
       */
      Any operator*(const Any& rhs) const
      {
        // copy the value
        Any value = *this;

        // multiply the rhs
        value *= rhs;

        // return the result.
        return value;
      }

      /**
       * Binary arithmetic operators - multiplication
       * @param Any the item we are multiplying from this.
       * @param const Any& the item we are multiplying from this.
       * @return Any *this*rhs
       */
      template<class T> friend Any operator*(Any lhs, const T& rhs) { lhs *= Any(rhs); return lhs; }
      
      //
      // *operators
      //
      #pragma endregion /=operators
      
      //
      // /operators
      //  
      #pragma region

      /**
      * Binary arithmetic operators - division
      * @param const Any& the item we are deviding this by
      * @return Any& *this/rhs
      */
      Any& operator/=(const Any& rhs)
      {
        // we use the double number as it is more precise
        if (Type() == dynamic::Misc_null && rhs.Type() == dynamic::Misc_null)
        {
          // null / null = std::overflow_error
          throw std::overflow_error("Division by zero.");
        }

        // if the rhs is null, then we cannot calculate it.
        // or if the value is zero.
        if (rhs.Type() == dynamic::Misc_null || rhs._ldvalue == 0)
        {
          // *this / null = std::overflow_error
          throw std::overflow_error("Division by zero.");
        }
        
        // save the current type.
        dynamic::Type type = dynamic::Floating_point_double;
        if (Type() == dynamic::Floating_point_long_double || rhs.Type() == dynamic::Floating_point_long_double)
        {
          type = dynamic::Floating_point_long_double;
        }
        else if (Type() == dynamic::Integer_long_long_int || rhs.Type() == dynamic::Integer_long_long_int)
        {
          type = dynamic::Floating_point_long_double;
        }
        else if (Type() == dynamic::Integer_unsigned_long_long_int || rhs.Type() == dynamic::Integer_unsigned_long_long_int)
        {
          type = dynamic::Floating_point_long_double;
        }
        else if (Type() == dynamic::Integer_long_int || rhs.Type() == dynamic::Integer_long_int)
        {
          type = dynamic::Floating_point_long_double;
        }
        else if (Type() == dynamic::Integer_unsigned_long_int || rhs.Type() == dynamic::Integer_unsigned_long_int)
        {
          type = dynamic::Floating_point_long_double;
        }

        // multiply the values.
        CastFrom(_ldvalue / rhs._ldvalue);

        // update the type.
        _type = CalculateType(type, rhs.Type() );

        // return the value.
        return *this;
      }

      /**
       * Binary arithmetic operators - division
       * @param const Any& the item we are deviding from this.
       * @return Any *this/ rhs
       */
      Any operator/(const Any& rhs) const
      {
        // copy the value
        Any value = *this;

        // multiply the rhs
        value /= rhs;

        // return the result.
        return value;
      }

      /**
       * Binary arithmetic operators - division
       * @param Any the item we are dividing from this.
       * @param const Any& the item we are dividing from this.
       * @return Any *this / rhs
       */
      template<class T> friend Any operator/(Any lhs, const T& rhs) { lhs /= Any(rhs); return lhs; }
      
      //
      // /operators
      //
      #pragma endregion /=operators

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
       * This function is used to re-calculate the 'best' type after an arithmetic opereation 
       * For example int*int could give us a long long
       *          or int / int could give us a double.
       * NB: this function does not _set_ the type, it only calculates the posible value.
       *     it is up to the call function to set the new type.
       * @param const dynamic::Type& lhsOriginal the original type on the lhs of the operation
       * @param const dynamic::Type& rhsOriginal the original type on the rhs of the operation
       * @return dynamic::Type the posible new type.
       */
      static dynamic::Type CalculateType(const dynamic::Type& lhsOriginal, const dynamic::Type& rhsOriginal)
      {
        //  null values become ints.
        if (is_Misc_null(lhsOriginal))
        {
          return CalculateType(dynamic::Integer_int, rhsOriginal);
        }

        if (is_Misc_null(rhsOriginal))
        {
          return CalculateType(lhsOriginal, dynamic::Integer_int);
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

        //  the possible type
        dynamic::Type type = lhsOriginal;

        // first we check for floating points.
        //
        // if either is long double, they both long double.
        if (lhsOriginal == dynamic::Floating_point_long_double || rhsOriginal == dynamic::Floating_point_long_double)
        {
          type = dynamic::Floating_point_long_double;
        }
        // if either is a double
        else if (lhsOriginal == dynamic::Floating_point_double || rhsOriginal == dynamic::Floating_point_double)
        {
          type = dynamic::Floating_point_double;
        }
        // if either is a float
        else if (lhsOriginal == dynamic::Floating_point_float || rhsOriginal == dynamic::Floating_point_float)
        {
          type = dynamic::Floating_point_float;
        }
        // if either is an unsigned long long int
        else if (lhsOriginal == dynamic::Integer_unsigned_long_long_int || rhsOriginal == dynamic::Integer_unsigned_long_long_int )
        {
          type = dynamic::Integer_unsigned_long_long_int;
        }
        // if either is an long long int
        else if (lhsOriginal == dynamic::Integer_long_long_int || rhsOriginal == dynamic::Integer_long_long_int)
        {
          type = dynamic::Integer_long_long_int;
        }
        else if (lhsOriginal == dynamic::Integer_unsigned_long_int || rhsOriginal == dynamic::Integer_unsigned_long_int)
        {
          type = dynamic::Integer_unsigned_long_int;
        }
        // if either is an long and other is unsigned int
        else if ((lhsOriginal == dynamic::Integer_long_int && rhsOriginal == dynamic::Integer_unsigned_int)
                 ||
                 (rhsOriginal == dynamic::Integer_long_int && lhsOriginal == dynamic::Integer_unsigned_int)
                )
        {
          type = dynamic::Integer_unsigned_long_int;
        }
        // if either is long
        else if (lhsOriginal == dynamic::Integer_long_int ||
                 rhsOriginal == dynamic::Integer_long_int)
        {
          type = dynamic::Integer_long_int;
        }
        // if either is unsigned int
        else if (lhsOriginal == dynamic::Integer_unsigned_int ||
                 rhsOriginal == dynamic::Integer_unsigned_int)
        {
          type = dynamic::Integer_unsigned_int;
        }
        else
        {
          type = dynamic::Integer_int;
        }

        // if we are here, they are both the same type, (floating/integer)
        // so we need to return the greatest of them both.
        return type;
      }

      /**
       * Compare 2 values and return 0 if they are both the same.
       * @param const Any& lhs the lhs value been compared.
       * @param const Any& rhs the lhs value been compared.
       * @return short, 0 if equal, -1 if type not the same, >=1 if value not same.
       */
      static short Compare(const Any& lhs, const Any& rhs)
      {
        // special case for null
        switch (lhs.Type())
        {
        case dynamic::Misc_unknown:
        case dynamic::Misc_null:
          // both are the same, (as per above), so if both null then they are the same.
          // all the values should be the same but there is no point in checking.
          return 0;
        }
        
        //  find the 'common' type
        auto type = CalculateType(lhs.Type(), rhs.Type());
        switch ( type )
        {
        // bool
        case Boolean_bool:
          //  should both be == 1
          if (lhs._llivalue != rhs._llivalue)
          {
            return 1;
          }
          break;

        // character
        case Character_signed_char:
        case Character_unsigned_char:
        case Character_char:
        case Character_wchar_t:
          break;

        // Integer
        case Integer_short_int:
        case Integer_unsigned_short_int:
          if ((short int)lhs._llivalue != (short int)rhs._llivalue)
          {
            return 1;
          }
          break;

        case Integer_int:
        case Integer_unsigned_int:
          if ((int)lhs._llivalue != (int)rhs._llivalue)
          {
            return 1;
          }
          break;

        case Integer_long_int:
        case Integer_unsigned_long_int:
          if ((long int)lhs._llivalue != (long int)rhs._llivalue)
          {
            return 1;
          }
          break;

        case Integer_long_long_int:
        case Integer_unsigned_long_long_int:
          if (lhs._llivalue != rhs._llivalue)
          {
            return 1;
          }
          break;

        // Floating point
        case Floating_point_float:
          if ((float)lhs._llivalue != (float)rhs._llivalue)
          {
            return 1;
          }
          break;

        case Floating_point_double:
          if ((double)lhs._llivalue != (double)rhs._llivalue)
          {
            return 1;
          }
          break;

        case Floating_point_long_double:
          if (lhs._llivalue != rhs._llivalue)
          {
            return 1;
          }
          break;

        default:
          throw std::bad_cast();
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
      template<class T>
      void CastFrom(const T& value)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;
        switch ( Type() )
        {
        case dynamic::Misc_null:
          _llivalue = 0;
          _ldvalue = 0;
          return;

        case dynamic::Boolean_bool:
          _llivalue = ( value ? 1: 0 );
          _ldvalue = ( value ? 1 : 0 );
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
          _llivalue = static_cast<long long int>(value);
          _ldvalue = static_cast<long double>(_llivalue);
          return;

        // double
        case dynamic::Floating_point_double:
        case dynamic::Floating_point_float:
        case dynamic::Floating_point_long_double:
          // we can cast those into long/long
          _ldvalue = static_cast<long double>(value);
          _llivalue = static_cast<long long int>(_ldvalue);
          return;

        case dynamic::Misc_unknown:
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
      template<class T>
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
          case dynamic::Misc_null:
            _llivalue = 0;
            _ldvalue = 0;
            return;

          // boolean
          case dynamic::Boolean_bool:

          // int
          case dynamic::Integer_short_int:
          case dynamic::Integer_unsigned_short_int:
          case dynamic::Integer_int:
          case dynamic::Integer_unsigned_int:
          case dynamic::Integer_long_int:
          case dynamic::Integer_unsigned_long_int:
          case dynamic::Integer_long_long_int:
          case dynamic::Integer_unsigned_long_long_int:

          // floating points.
          case dynamic::Floating_point_double:
          case dynamic::Floating_point_float:
          case dynamic::Floating_point_long_double:
            _llivalue = 0;
            _ldvalue = 0;
            return;

          case dynamic::Misc_unknown:
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
       * Create from a character pointer.
       * @param char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<char>(char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<const char>(const char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param signed char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<signed char>(signed char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param signed char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<const signed char>(const signed char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param unsigned char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<unsigned char>(unsigned char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param unsigned char* value the value we are trying to create from.
       */
      template<>
      void CastFrom<const unsigned char>(const unsigned char* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param wchar_t* value the value we are trying to create from.
       */
      template<>
      void CastFrom<wchar_t>(wchar_t* value)
      {
        CreateFromCharacters(value);
      }

      /**
       * Create from a character pointer.
       * @param wchar_t* value the value we are trying to create from.
       */
      template<>
      void CastFrom<const wchar_t>(const wchar_t* value)
      {
        CreateFromCharacters(value);
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
       * Create from a given value.
       * @param const char& value the value we are trying to create from.
       */
      template<>
      void CastFrom<char>(const char& value)
      {
        CreateFromCharacter(value);
      }

      /**
       * Create from a given value.
       * @param const signed char& value the value we are trying to create from.
       */
      template<>
      void CastFrom<signed char>(const signed char& value)
      {
        CreateFromCharacter(value);
      }

      /**
       * Create from a given value.
       * @param const unsigned char& value the value we are trying to create from.
       */
      template<>
      void CastFrom<unsigned char>(const unsigned char& value)
      {
        CreateFromCharacter(value);
      }

      /**
       * Create from a given value.
       * @param const wchar_t& value the value we are trying to create from.
       */
      template<>
      void CastFrom<wchar_t>(const wchar_t& value)
      {
        CreateFromCharacter(value);
      }
      
      /**
       * Create from a const std::string.
       * @param std::string& value the value we are trying to create from.
       */
      void CastFrom(std::string& value)
      {
        CreateFromCharacters(value.c_str());
      }

      /**
       * Create from a const std::string*.
       * @param std::string* value the value we are trying to create from.
       */
      void CastFrom(std::string* value)
      {
        CreateFromCharacters(value ? value->c_str() : (const char*)nullptr);
      }

      /**
       * Create from a const std::string*.
       * @param const std::string* value the value we are trying to create from.
       */
      void CastFrom( const std::string* value)
      {
        CreateFromCharacters(value ? value->c_str() : (const char*)nullptr);
      }

      /**
       * Create from a std::wstring.
       * @param std::wstring& value the value we are trying to create from.
       */
      void CastFrom(std::wstring& value)
      {
        CreateFromCharacters(value.c_str());
      }

      /**
       * Create from a const std::wstring*.
       * @param std::wstring* value the value we are trying to create from.
       */
      void CastFrom(std::wstring* value)
      {
        CreateFromCharacters(value ? value->c_str() : (wchar_t*)nullptr);
      }

      /**
       * Create from a const std::wstring*.
       * @param const std::wstring* value the value we are trying to create from.
       */
      void CastFrom( const std::wstring* value)
      {
        CreateFromCharacters(value ? value->c_str() : (wchar_t*)nullptr);
      }

      /**
       * Create from a const std::string.
       * @param const std::string& value the value we are trying to create from.
       */
      void CastFrom(const std::string& value)
      {
        CreateFromCharacters(value.c_str());
      }

      /**
      * Create from a const std::wstring.
      * @param const std::wstring& value the value we are trying to create from.
      */
      void CastFrom(const std::wstring& value)
      {
        CreateFromCharacters(value.c_str());
      }

      /**
       * Try and cast this to a posible value.
       * @return T the value we are looking for.
       */
      template<class T>
      T CastTo() const
      {
        switch ( Type() )
        {
        case dynamic::Type::Misc_null:
          return 0;

        // char
        case dynamic::Type::Character_char:
        case dynamic::Type::Character_unsigned_char:
        case dynamic::Type::Character_signed_char:
        case dynamic::Type::Character_wchar_t:
          if (dynamic::is_type_floating(dynamic::get_type<T>::value))
          {
            return static_cast<T>(_ldvalue);
          }
          return static_cast<T>(_llivalue);

        // Integer
        case dynamic::Type::Integer_unsigned_int:
        case dynamic::Type::Integer_int:
        case dynamic::Type::Integer_short_int:
        case dynamic::Type::Integer_unsigned_short_int:
        case dynamic::Type::Integer_long_int:
        case dynamic::Type::Integer_unsigned_long_int:
        case dynamic::Type::Integer_long_long_int:
        case dynamic::Type::Integer_unsigned_long_long_int:
          return static_cast<T>(_llivalue);

        case dynamic::Type::Floating_point_double:
        case dynamic::Type::Floating_point_float:
        case dynamic::Type::Floating_point_long_double:
          return static_cast<T>(_ldvalue);

        case dynamic::Type::Boolean_bool:
          return static_cast<T>(_ldvalue);

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
        return ReturnFromCharacters<const char>();
      }

      /**
       * Try and cast this to a posible value.
       * @return char* the value we are looking for.
       */
      template<>
      char* CastTo<char*>() const
      {
        return ReturnFromCharacters<char>();
      }

      /**
       * Try and cast this to a posible value.
       * @return wchar_t* the value we are looking for.
       */
      template<>
      wchar_t* CastTo<wchar_t*>() const
      {
        return (wchar_t*)ReturnFromCharacters<const wchar_t>();
      }

      /**
       * Try and cast this to a posible value.
       * @return const wchar_t* the value we are looking for.
       */
      template<>
      const wchar_t* CastTo<const wchar_t*>() const
      {
        return ReturnFromCharacters<const wchar_t>();
      }

      /**
       * Return a character
       * @return T* the character we want to return no.
       */
      template<class T>
      T* ReturnFromCharacters() const
      {
        switch (Type())
        {
        case dynamic::Type::Misc_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          if (nullptr == _svalue)
          {
            const_cast<Any*>(this)->CreateString();
          }
          return (T*)_svalue->c_str();

        case dynamic::Type::Character_char:
        case dynamic::Type::Character_signed_char:
        case dynamic::Type::Character_unsigned_char:
          return static_cast<char*>(_cvalue);
        }

        // do we need to create the string representation?
        if (nullptr == _svalue)
        {
          const_cast<Any*>(this)->CreateString();
        }
        return (T*)_svalue->c_str();
      }

      /**
      * Return a character
      * @return T* the character we want to return no.
      */
      template<>
      const wchar_t* ReturnFromCharacters<const wchar_t>() const
      {
        switch (Type())
        {
        case dynamic::Type::Misc_null:
          return '\0';

        case dynamic::Type::Character_wchar_t:
          return static_cast<wchar_t*>( (void*)_cvalue);

        case dynamic::Type::Character_char:
        case dynamic::Type::Character_signed_char:
        case dynamic::Type::Character_unsigned_char:
          if (nullptr == _swvalue)
          {
            const_cast<Any*>(this)->CreateWideString();
          }
          return _swvalue->c_str();
        }

        // do we need to create the string representation?
        if (nullptr == _swvalue)
        {
          const_cast<Any*>(this)->CreateWideString();
        }
        return _swvalue->c_str();
      }

      /**
       * Create the cosmetic representation of the string.
       */
      void CreateWideString()
      {
        //  do we need to do anyting?
        if (nullptr != _swvalue)
        {
          return;
        }

        // create the new string
        _swvalue = new std::wstring();

        // are we a char?
        switch (Type())
        {
          case dynamic::Character_char:
          case dynamic::Character_unsigned_char:
          case dynamic::Character_signed_char:
          {
            if (nullptr == _cvalue)
            {
              *_swvalue = L"";
              return;
            }

            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;
            *_swvalue = converterX.from_bytes((const char*)_cvalue);
          }
          return;
        }

        if (dynamic::is_type_floating(Type()))
        {
          *_swvalue = std::to_wstring(_ldvalue);
        }
        else
        {
          *_swvalue = std::to_wstring(_llivalue);
        }
      }
      /**
       * Create the cosmetic representation of the string.
       */
      void CreateString()
      {
        //  do we need to do anyting?
        if (nullptr != _svalue)
        {
          return;
        }

        //  we will need a new string
        _svalue = new std::string();

        // are we a wchar_t?
        if (Type() == dynamic::Character_wchar_t)
        {
          if (nullptr == _cvalue)
          {
            *_svalue = "";
            return;
          }

          using convert_typeX = std::codecvt_utf8<wchar_t>;
          std::wstring_convert<convert_typeX, wchar_t> converterX;
          *_svalue = converterX.to_bytes( (const wchar_t*)_cvalue);
          return;
        }

        if (dynamic::is_type_floating(Type()))
        {
          *_svalue = std::to_string(_ldvalue);
        }
        else
        {
          *_svalue = std::to_string(_llivalue);
        }
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
        case dynamic::Type::Misc_null:
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
        return static_cast<char>(_llivalue);
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
        case dynamic::Type::Misc_null:
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
        return static_cast<wchar_t>(_llivalue);
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
        case dynamic::Type::Misc_null:
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
        return static_cast<unsigned char>(_llivalue);
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
        case dynamic::Type::Misc_null:
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
        return static_cast<signed char>(_llivalue);
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
        case dynamic::Type::Misc_null:
          return false;
        }

        //  value to true/false
        return (_llivalue != 0 ? true : false);
      }

      /**
       * Clean up the value(s)
       */
      void CleanValues()
      {
        // delete the char if need be
        delete _cvalue;

        // delete the cosmetic strings
        delete _svalue;
        delete _swvalue;

        // reset the values
        _llivalue = 0;
        _ldvalue = 0;
        _cvalue = nullptr;
        _svalue = nullptr;
        _swvalue = nullptr;
      }

      /**
       * Create a value from a multiple characters..
       * @param const T* value the character we are creating from.
       */
      template<class T>
      void CreateFromCharacters(const T* value)
      {
        // clean the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;

        if (nullptr != value)
        {
          // default values.
          _lcvalue = (std::strlen((const char*)value)+1) * sizeof(T);
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, value, _lcvalue);

          _llivalue = std::strtoll(_cvalue, nullptr, 0);
          _ldvalue = std::strtold(_cvalue, nullptr);
        }
        else
        {
          // create a default value for the string.
          const char c = '\0';

          // default values.
          _lcvalue = sizeof(T);
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, &c, _lcvalue);

          // default values are 0
          _llivalue = 0;
          _ldvalue = 0;
        }
      }

      /**
       * Create a value from a multiple characters..
       * @param const T* value the character we are creating from.
       */
      template<>
      void CreateFromCharacters<wchar_t>( const wchar_t* value)
      {
        // clean the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<wchar_t>::value;

        if (nullptr != value)
        {
          // default values.
          _lcvalue = (std::wcslen(value)+1) * sizeof(wchar_t);
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, value, _lcvalue);

          _llivalue = std::wcstoll(value, nullptr, 0);
          _ldvalue = std::wcstold(value, nullptr);
        }
        else
        {
          // create a default value for the string.
          const wchar_t wide = L'\0';
          _lcvalue = sizeof(wchar_t);
          _cvalue = new char[_lcvalue];
          std::memset(_cvalue, 0, _lcvalue);
          std::memcpy(_cvalue, &wide, _lcvalue);

          // default values are 0
          _llivalue = 0;
          _ldvalue = 0;
        }
      }

      /**
       * Create a value from a single character.
       * @param const char value the character we are creating from.
       */
      template<class T>
      void CreateFromCharacter(const T value )
      {
        // clean the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;

        // create the character.
        _lcvalue = sizeof(T);
        _cvalue = new char[_lcvalue];
        std::memset(_cvalue, 0, _lcvalue);
        std::memcpy(_cvalue, &value, _lcvalue);

        if (value >= '0' && value <= '9')
        {
          auto number = value - '0';
          _llivalue = number;
          _ldvalue = number;
        }
        else
        {
          _llivalue = 0;
          _ldvalue = 0;
        }
      }

      /**
       * Create a value from a single wide character.
       * @param const wchar_t value the character we are creating from.
       */
      template<>
      void CreateFromCharacter<const wchar_t>( const wchar_t value)
      {
        // clean the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<wchar_t>::value;

        // create the character.
        _lcvalue = sizeof(wchar_t);
        _cvalue = new char[_lcvalue];
        std::memset(_cvalue, 0, _lcvalue);
        std::memcpy(_cvalue, &value, _lcvalue);

        // copy it.
        if (value >= L'0' && value <= L'9')
        {
          auto number = value - L'0';
          _llivalue = number;
          _ldvalue = number;
        }
        else
        {
          _llivalue = 0;
          _ldvalue = 0;
        }
      }

      template<class T>
      Any& AddNumber(dynamic::Type type, T number)
      {
        // add the values.
        if (dynamic::is_type_floating(type))
        {
          CastFrom(_ldvalue + number);
        }
        else
        {
          CastFrom(_llivalue + number);
        }

        // update the type.
        _type = type;

        // done
        return *this;
      }

      template<class T>
      Any& SubtractNumber(dynamic::Type type, T number)
      {
        // add the values.
        if (dynamic::is_type_floating(type))
        {
          CastFrom(_ldvalue - number);
        }
        else
        {
          CastFrom(_llivalue - number);
        }

        // update the type.
        _type = type;

        // done
        return *this;
      }

      // the biggest integer value.
      long long int _llivalue;

      // the biggest floating point value
      long double _ldvalue;

      // this is the given character value either char/signed char/unsigned char/wide
      char* _cvalue;
      size_t _lcvalue;

      // 'cosmetic' representations of the numbers, both wide and non wide strings.
      // the values are only created if/when the caller call a to string function.
      std::string* _svalue;
      std::wstring* _swvalue;

      // the variable type
      dynamic::Type _type;
    };
  }
}