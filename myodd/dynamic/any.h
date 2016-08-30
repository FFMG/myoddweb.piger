// ***********************************************************************
// Copyright (c) 2016 Florent Guelfucci
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// @see https://opensource.org/licenses/MIT
// ***********************************************************************
#pragma once

// string representation of the version number
#define MYODD_ANY_VERSION        "0.1.1"

// the version number is #.###.###
// first number is major
// then 3 numbers for minor
// and 3 numbers for tiny
#define MYODD_ANY_VERSION_NUMBER 0001000 

#include <typeinfo>       // std::bad_cast
#include <algorithm>      // memcpy
#include <math.h>         // modf
#include <string>
#include <cctype>         //  isdigit
#include <codecvt>        //  string <-> wstring
#include <stdlib.h>       //  std::strtoll / std::strtoull
#include <type_traits>    //  std::is_trivially_copyable
                          //  std::is_pointer

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
        _ltvalue(0),
        _tvalue(nullptr),
        _ptvalue(nullptr),
        _type(Type::Misc_null),
        _stringStatus(StringStatus_Not_A_Number)
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
            std::memset(_cvalue, '\0', _lcvalue);
            std::memcpy(_cvalue, other._cvalue, _lcvalue);
            _stringStatus = other._stringStatus;
          }

          // copy the trivial pointer
          _ptvalue = other._ptvalue;

          // copy the trivial value
          if (other._ltvalue > 0 && other._tvalue)
          {
            _ltvalue = other._ltvalue;
            _tvalue = new char[_ltvalue];
            std::memset(_tvalue, '0', _ltvalue);
            std::memcpy(_tvalue, other._tvalue, _ltvalue);
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
        if (rhs.UseUnsignedInteger())
        {
          return AddNumber(CalculateType(*this, rhs), (unsigned long long int)rhs._llivalue);
        }
        if (rhs.UseSignedInteger())
        {
          return AddNumber(CalculateType(*this, rhs), (long long int)rhs._llivalue);
        }
        return AddNumber(CalculateType(*this, rhs), rhs._ldvalue);
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
        return AddNumber(CalculateType(*this, dynamic::Integer_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param short rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(short int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned short int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned short int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned long int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param long long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long long int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param unsigned long long int rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(unsigned long long int rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param float rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(float rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param double rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(double rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized += function add the rhs to *this.
       * @param float rhs the value we are adding to *this
       * @param *this + rhs.
       */
      template<>
      Any& operator+=(long double rhs)
      {
        return AddNumber(CalculateType(*this, dynamic::Floating_point_long_double), rhs);
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
        dynamic::Type type = NumberType();

        // add an int.
        if (dynamic::is_type_floating(type))
        {
          // we cannot call ++_ldvalue as the value is passed by reference.
          // to CastFrom( cons T& ) and the first thing we do is clear the value
          // so _ldvalue/_llivalue become 0
          CastFrom( _ldvalue+1 );
        }
        else
        {
          // we cannot call ++_ldvalue as the value is passed by reference.
          // to CastFrom( cons T& ) and the first thing we do is clear the value
          // so _ldvalue/_llivalue become 0
          CastFrom( _llivalue+1 );
        }

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
        if (rhs.UseUnsignedInteger())
        {
          return SubtractNumber(CalculateType(*this, rhs), (unsigned long long int)rhs._llivalue);
        }
        
        if (rhs.UseSignedInteger())
        {
          return SubtractNumber(CalculateType(*this, rhs), (long long int)rhs._llivalue);
        }
        return SubtractNumber(CalculateType(*this, rhs), rhs._ldvalue);
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
        return SubtractNumber(CalculateType(*this, dynamic::Integer_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param short rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(short int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned short int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned short int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned long int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param long long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long long int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param unsigned long long int rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(unsigned long long int rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param float rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(float rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param double rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(double rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized -= function substract the rhs to *this.
       * @param float rhs the value we are subtracting from *this
       * @param *this - rhs.
       */
      template<>
      Any& operator-=(long double rhs)
      {
        return SubtractNumber(CalculateType(*this, dynamic::Floating_point_long_double), rhs);
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
        dynamic::Type type = NumberType();

        // substract an int
        // add an int.
        if (dynamic::is_type_floating(type))
        {
          // we cannot call ++_ldvalue as the value is passed by reference.
          // to CastFrom( cons T& ) and the first thing we do is clear the value
          // so _ldvalue/_llivalue become 0
          CastFrom( _ldvalue-1 );
        }
        else
        {
          // we cannot call ++_ldvalue as the value is passed by reference.
          // to CastFrom( cons T& ) and the first thing we do is clear the value
          // so _ldvalue/_llivalue become 0
          CastFrom( _llivalue-1 );
        }

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
        if (dynamic::is_type_floating(rhs.NumberType()))
        {
          return MultiplyNumber(CalculateType(*this, rhs), rhs._ldvalue);
        }
        return MultiplyNumber(CalculateType(*this, rhs), rhs._llivalue);
      }

      /**
       * Default *= function multiply the rhs and *this.
       * @param const T& rhs the value we are multiplying with *this
       * @param *this * rhs.
       */
      template<class T>
      Any& operator*=(T rhs)
      {
        *this *= Any(rhs);
        return *this;
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param short int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( short int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param unsigned short int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( unsigned short int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=(int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param unsigned int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( unsigned int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param long int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( long int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param unsigned long int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( unsigned long int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param long long int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( long long int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param unsigned long long int rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( unsigned long long int rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param float rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( float rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param double rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( double rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized *= function multiply rhs and *this.
       * @param long double rhs the value we are multiplying to *this
       * @param *this * rhs.
       */
      template<>
      Any& operator*=( long double rhs)
      {
        return MultiplyNumber(CalculateType(*this, dynamic::Floating_point_long_double), rhs);
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
      template<class T> friend Any operator*(Any lhs, const T& rhs) { lhs *= rhs; return lhs; }

      //
      // *operators
      //
      #pragma endregion *=operators
      
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
        if ( rhs.Type() == dynamic::Misc_null)
        {
          // anything over null is the same as zero
          throw std::overflow_error("Division by zero.");
        }

        if (rhs.UseUnsignedInteger())
        {
          return DivideNumber(CalculateDivideType(Type(), rhs.Type()), (unsigned long long int)rhs._llivalue);
        }
        if (rhs.UseSignedInteger())
        {
          return DivideNumber(CalculateDivideType(Type(), rhs.Type()), (long long int)rhs._llivalue);
        }

        // use long double
        return DivideNumber(CalculateDivideType(Type(), rhs.Type()), rhs._ldvalue);
      }

      /**
       * Default /= function divide *this by rhs.
       * @param const T& rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<class T>
      Any& operator/=(T rhs)
      {
        *this /= Any(rhs);
        return *this;
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param short int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( short int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_short_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param unsigned short int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( unsigned short int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_unsigned_short_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=(int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param unsigned int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( unsigned int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_unsigned_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param long int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( long int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_long_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param unsigned long int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( unsigned long int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_unsigned_long_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param long long int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( long long int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_long_long_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param unsigned long long int rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( unsigned long long int rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Integer_unsigned_long_long_int), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param float rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( float rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Floating_point_float), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param double rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( double rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Floating_point_double), rhs);
      }

      /**
       * Specialized /= function divide *this by rhs.
       * @param long double rhs the value we are dividing *this with
       * @param *this / rhs.
       */
      template<>
      Any& operator/=( long double rhs)
      {
        return DivideNumber(CalculateDivideType(Type(), dynamic::Floating_point_long_double), rhs);
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
       * Regadless the data type, we try and guess that the number type could be.
       * mainly used for string, so we can guess the string type.
       * If this is a number we will return the number type, otherwse we will try and work it out.
       * If we do not know we will return Integer_int
       * @return dynamic::Type the string type.
       */ 
      dynamic::Type NumberType() const
      {
        //  if it is not a character then just use whatever we have
        if (!dynamic::is_type_character(Type()))
        {
          return Type();
        }

        // if the string is not a number, just use an int.
        if (!IsStringNumber(true))
        {
          // this is not a number, so zero will be used.
          // we might as well use an int.
          return dynamic::Integer_int;
        }

        // it seems to be a number, so we need to calculate what type of number it is.
        switch (_stringStatus)
        {
        case myodd::dynamic::Any::StringStatus_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Partial_Pos_Number:
          if (_llivalue < 0)
          {
            return dynamic::Integer_unsigned_long_long_int;
          }
          return dynamic::Integer_long_long_int;

        case myodd::dynamic::Any::StringStatus_Neg_Number:
        case myodd::dynamic::Any::StringStatus_Partial_Neg_Number:
          return dynamic::Integer_long_long_int;

        case myodd::dynamic::Any::StringStatus_Floating_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Neg_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Partial_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Partial_Neg_Number:
          return dynamic::Floating_point_long_double;

        default:
          throw std::exception("Unknown string type!");
        }
      }

      /**
       * Calculate the type for division
       * @param const dynamic::Type& lhs the lhs type
       * @param const dynamic::Type& rhs the rhs type
       * @return dynamic::Type the best type for division
       */
      static dynamic::Type CalculateDivideType(const dynamic::Type& lhs, const dynamic::Type& rhs)
      {
        if (lhs == dynamic::Floating_point_long_double || rhs == dynamic::Floating_point_long_double)
        {
          return dynamic::Floating_point_long_double;
        }
        
        if (lhs == dynamic::Integer_long_long_int || rhs == dynamic::Integer_long_long_int)
        {
          return dynamic::Floating_point_long_double;
        }
        
        if (lhs == dynamic::Integer_unsigned_long_long_int || rhs == dynamic::Integer_unsigned_long_long_int)
        {
          return dynamic::Floating_point_long_double;
        }

        else if (lhs == dynamic::Integer_long_int || rhs == dynamic::Integer_long_int)
        {
          return dynamic::Floating_point_long_double;
        }
        
        if (lhs == dynamic::Integer_unsigned_long_int || rhs == dynamic::Integer_unsigned_long_int)
        {
          return dynamic::Floating_point_long_double;
        }

        // default value.
        return dynamic::Floating_point_double;
      }

      /**
      * This function is used to re-calculate the 'best' type after an arithmetic opereation
      * For example int*int could give us a long long
      *          or int / int could give us a double.
      * NB: this function does not _set_ the type, it only calculates the posible value.
      *     it is up to the call function to set the new type.
      * @param const Any& lhs the original number on the lhs of the operation
      * @param const dynamic::Type& rhsOriginal the original type on the rhs of the operation
      * @return dynamic::Type the posible new type.
      */
      static dynamic::Type CalculateType(const Any& lhs, const Any& rhs)
      {
        auto lhsOriginal = lhs.NumberType();
        auto rhsOriginal = rhs.NumberType();
        return CalculateType(lhsOriginal, rhsOriginal);
      }

      /**
       * This function is used to re-calculate the 'best' type after an arithmetic opereation 
       * For example int*int could give us a long long
       *          or int / int could give us a double.
       * NB: this function does not _set_ the type, it only calculates the posible value.
       *     it is up to the call function to set the new type.
       * @param const Any& lhs the original number on the lhs of the operation
       * @param const dynamic::Type& rhsOriginal the original type on the rhs of the operation
       * @return dynamic::Type the posible new type.
       */
      static dynamic::Type CalculateType(const Any& lhs, const dynamic::Type& rhsOriginal)
      {
        return CalculateType(lhs.NumberType(), rhsOriginal);
      }

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

        // are we comparing trivial structures
        if (lhs.IsTrivial() || rhs.IsTrivial() )
        {
          return CompareTrivial( lhs, rhs);
        }
        
        //  find the 'common' type
        // in the case of 2 characters we could end up comparing 2xzeros
        // but it is fine as we will compare them further later.
        auto type = CalculateType(lhs, rhs);
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
          if ((float)lhs._ldvalue != (float)rhs._ldvalue)
          {
            return 1;
          }
          break;

        case Floating_point_double:
          if ((double)lhs._ldvalue != (double)rhs._ldvalue)
          {
            return 1;
          }
          break;

        case Floating_point_long_double:
          if (lhs._ldvalue != rhs._ldvalue)
          {
            return 1;
          }
          break;

        default:
          throw std::bad_cast();
        }

        //  if they are both characters then we need to test further.
        if (dynamic::is_type_character(lhs.Type()) && dynamic::is_type_character(rhs.Type()) &&
           (!lhs.IsStringNumber( false ) || !rhs.IsStringNumber(false) ))
        {
          //  null/not null
          if (lhs._cvalue && !rhs._cvalue)
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
        }

        // looks the same
        return 0;
      }

      /**
       * Compare one or more trivial cases.
       * @throw if we are unable to compare, (not same types, not same sizes etc...)
       * @return short 0 if they are the same or -1 if not.
       */
      static short CompareTrivial(const Any& lhs, const Any& rhs)
      {
        // are they both trivial?
        if (!lhs.IsTrivial() || !rhs.IsTrivial())
        {
          // We cannot compare non trivials.
          throw std::bad_cast();
        }

        // are they the same size
        if (lhs._ltvalue != rhs._ltvalue)
        {
          return 1;
        }

        // they look the same, so we can now compare them both,
        return (std::memcmp(lhs._tvalue, rhs._tvalue, lhs._ltvalue) != 0 ? -1 : 0);
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

        case dynamic::Misc_unknown:
          // Objects of trivially - copyable types are the only C++ objects that 
          // may be safely copied with std::memcpy
          CreateFromTrivial( value );

          // done
          return;

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

        // if unknown try and set is as a pointer.
        if (_type == dynamic::Misc_unknown)
        {
          CreateFromTrivial<T*>(value);
          return;
        }

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
       * Create from a boolean value.
       * @param const bool& value the bool value.
       */
      template<>
      void CastFrom<bool>(const bool& value)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<bool>::value;

        // set the values
        _llivalue = (value ? 1 : 0);
        _ldvalue = (value ? 1 : 0);
      }

      /**
       * Create from a float value.
       * @param const float& value the number value.
       */
      template<>
      void CastFrom<float>(const float& value)
      {
        CreateFromDouble(value);
      }

      /**
       * Create from a double value.
       * @param const double& value the number value.
       */
      template<>
      void CastFrom<double>(const double& value)
      {
        CreateFromDouble(value);
      }

      /**
       * Create from a long double value.
       * @param const long double& value the number value.
       */
      template<>
      void CastFrom <long double> (const long double& value)
      {
        CreateFromDouble(value);
      }

      /**
       * Create from a short int value.
       * @param const short int& value the number value.
       */
      template<>
      void CastFrom <short int> (const short int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from an unsigned short int value.
       * @param const unsigned short int& value the number value.
       */
      template<>
      void CastFrom <unsigned short int> (const unsigned short int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from an int value.
       * @param const int& value the number value.
       */
      template<>
      void CastFrom <int> (const int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from an unsigned int value.
       * @param const unsigned int& value the number value.
       */
      template<>
      void CastFrom <unsigned int> (const unsigned int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from a long int value.
       * @param const long int& value the number value.
       */
      template<>
      void CastFrom < long int> (const long int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from a unsigned long int value.
       * @param const unsigned long int& value the number value.
       */
      template<>
      void CastFrom <unsigned long int> (const unsigned long int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from a long long int value.
       * @param const long long int& value the number value.
       */
      template<>
      void CastFrom <long long int> (const long long int& value)
      {
        CreateFromInteger(value);
      }

      /**
       * Create from a unsigned long int value.
       * @param const unsigned long long int& value the number value.
       */
      template<>
      void CastFrom <unsigned long long int> (const unsigned long long int& value)
      {
        CreateFromInteger(value);
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          return CastToTrivial<T>();

        // none of the fundamental types are handled here.
        // each has its own function
        default:
          break; 
        }

        // we cannot cast this.
        throw std::bad_cast();
      }

      /**
       * Cast this to a fundamental type
       * @return short int the value.
       */
      template<>
      float CastTo() const
      {
        return CastToFundamental<float>();
      }

      /**
       * Cast this to a fundamental type
       * @return double the value.
       */
      template<>
      double CastTo() const
      {
        return CastToFundamental<double>();
      }

      /**
       * Cast this to a fundamental type
       * @return long double the value.
       */
      template<>
      long double CastTo() const
      {
        return CastToFundamental<long double>();
      }

      /**
       * Cast this to a fundamental type
       * @return short int the value.
       */
      template<>
      short int CastTo() const
      {
        return CastToFundamental<short int>();
      }

      /**
       * Cast this to a fundamental type
       * @return unsigned short int the value.
       */
      template<>
      unsigned short int CastTo() const
      {
        return CastToFundamental<unsigned short int>();
      }

      /**
       * Cast this to a fundamental type
       * @return int the value.
       */
      template<>
      int CastTo() const
      {
        return CastToFundamental<int>();
      }

      /**
       * Cast this to a fundamental type
       * @return unsigned int the value.
       */
      template<>
      unsigned int CastTo() const
      {
        return CastToFundamental<unsigned int>();
      }

      /**
       * Cast this to a fundamental type
       * @return long the value.
       */
      template<>
      long CastTo() const
      {
        return CastToFundamental<long>();
      }

      /**
       * Cast this to a fundamental type
       * @return unsigned long the value.
       */
      template<>
      unsigned long CastTo() const
      {
        return CastToFundamental<unsigned long> ();
      }

      /**
       * Cast this to a fundamental type
       * @return long long the value.
       */
      template<>
      long long CastTo() const
      {
        return CastToFundamental<long long>();
      }

      /**
       * Cast this to a fundamental type
       * @return unsigned long long the value.
       */
      template<>
      unsigned long long CastTo() const
      {
        return CastToFundamental<unsigned long long> ();
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
       * @return wchar_t* the value we are looking for.
       */
      template<>
      std::wstring CastTo<std::wstring>() const
      {
        return std::wstring( (wchar_t*)ReturnFromCharacters<const wchar_t>());
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
      * Try and cast this to a posible value.
      * @return char the value we are looking for.
      */
      template<>
      char CastTo<char>() const
      {
        switch (Type())
        {
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          return static_cast<char>(*(wchar_t*)_cvalue);

        case dynamic::Character_char:
          return static_cast<char>(*(char*)_cvalue);

        case dynamic::Character_signed_char:
          return static_cast<char>(*(signed char*)_cvalue);

        case dynamic::Character_unsigned_char:
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          return static_cast<wchar_t>(*(wchar_t*)_cvalue);

        case dynamic::Character_char:
          return static_cast<wchar_t>(*(char*)_cvalue);

        case dynamic::Character_signed_char:
          return static_cast<wchar_t>(*(signed char*)_cvalue);

        case dynamic::Character_unsigned_char:
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          return static_cast<unsigned char>(*(wchar_t*)_cvalue);

        case dynamic::Character_char:
          return static_cast<unsigned char>(*(char*)_cvalue);

        case dynamic::Character_signed_char:
          return static_cast<unsigned char>(*(signed char*)_cvalue);

        case dynamic::Character_unsigned_char:
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          return static_cast<signed char>(*(wchar_t*)_cvalue);

        case dynamic::Character_char:
          return static_cast<signed char>(*(char*)_cvalue);

        case dynamic::Character_signed_char:
          return static_cast<signed char>(*(signed char*)_cvalue);

        case dynamic::Character_unsigned_char:
          return static_cast<signed char>(*(unsigned char*)_cvalue);
        }
        return static_cast<signed char>(_llivalue);
      }

      /**
      * Try and cast this to a posible value.
      * we have a specialised function as casting to bool can be inefficent.
      * @return bool the value we are looking for.
      */
      template<>
      bool CastTo<bool>() const
      {
        switch (Type())
        {
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          // null is false/
          return false;
        }

        // if we are a float we must use it, in case we have 0.0001
        // if we were using the long long int then we would only have 0
        if (dynamic::is_type_floating(NumberType()))
        {
          return (_ldvalue != 0);
        }

        // but we use the int if we are told to
        // in case the long double is not valid.
        return (_llivalue != 0);
      }

      /**
      * Cast this to a fundamental type
      * @return short int the value.
      */
      template<class T>
      std::enable_if_t<!std::is_pointer<T>::value, T> CastToTrivial() const
      {
        // as it is not a pointer, it has to be trivially copyable.
        if (!std::is_trivially_copyable<T>::value)
        {
          // we cannot convert this T to a trivial type.
          throw std::bad_cast();
        }

        // As we are looking for the non pointer value we can 
        // only return the non pointer version of the trivial
        // we cannot cast our pointer into whatever decltype(T) was passed to us.
        if (Type() != dynamic::Misc_trivial)
        {
          // we cannot convert this to a trivial type.
          throw std::bad_cast();
        }

        // can we fit our data exactly inside the structure that they are trying to make us use.
        if (sizeof T != _ltvalue)
        {
          throw std::bad_cast();
        }

        // copy the trival value.
        T trivial = {};
        std::memcpy(&trivial, _tvalue, _ltvalue);

        // done
        return trivial;
      }

      /**
       * Cast this to a fundamental type
       * @return short int the value.
       */
      template<class T>
      std::enable_if_t<std::is_pointer<T>::value, T> CastToTrivial() const
      {
        if (!IsTrivial())
        {
          // we cannot convert this to a trivial type.
          throw std::bad_cast();
        }

        // we konw, that we handle certain pointers, (strings, ints etc)
        // so there is no way that we can cast a trivial value to something
        // we know it cannot be, only unknown types are 'trivial'
        if(dynamic::Misc_unknown != dynamic::get_type<std::remove_pointer<T>::type>::value )
        {
          // we cannot cast to this T* as we know
          // that it was not what it was created with, (as we handle known pointers).
          throw std::bad_cast();
        }

        // are _we_ a non pointer trivial?
        // in that case we can return our address.
        // the user should not be allowed to delete
        //  it as they did not create this value.
        if (dynamic::Misc_trivial == Type())
        {
          return (T)_tvalue;
        }

        // we are a pointer value so we can return it.
        return (T)_ptvalue;
      }

      /**
      * Do common casting to known fundamental type.
      * @return T the 'fundamental' cast
      */
      template<class T>
      T CastToFundamental() const
      {
        switch (Type())
        {
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return 0;

          // char
        case dynamic::Character_char:
        case dynamic::Character_unsigned_char:
        case dynamic::Character_signed_char:
        case dynamic::Character_wchar_t:
          if (dynamic::is_type_floating(NumberType()))
          {
            return static_cast<T>(_ldvalue);
          }
          return static_cast<T>(_llivalue);

          // Integer
        case dynamic::Integer_unsigned_int:
        case dynamic::Integer_int:
        case dynamic::Integer_short_int:
        case dynamic::Integer_unsigned_short_int:
        case dynamic::Integer_long_int:
        case dynamic::Integer_unsigned_long_int:
        case dynamic::Integer_long_long_int:
        case dynamic::Integer_unsigned_long_long_int:
          return static_cast<T>(_llivalue);

        case dynamic::Floating_point_double:
        case dynamic::Floating_point_float:
        case dynamic::Floating_point_long_double:
          return static_cast<T>(_ldvalue);

        case dynamic::Boolean_bool:
          return static_cast<T>(_ldvalue);

        default:
          break;
        }

        // we cannot cast this.
        throw std::bad_cast();
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          if (nullptr == _svalue)
          {
            const_cast<Any*>(this)->CreateString();
          }
          return (T*)_svalue->c_str();

        case dynamic::Character_char:
        case dynamic::Character_signed_char:
        case dynamic::Character_unsigned_char:
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
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          throw std::bad_cast();

        case dynamic::Misc_null:
          return '\0';

        case dynamic::Character_wchar_t:
          return static_cast<wchar_t*>( (void*)_cvalue);

        case dynamic::Character_char:
        case dynamic::Character_signed_char:
        case dynamic::Character_unsigned_char:
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

        if (dynamic::is_type_floating(NumberType()))
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

        if (dynamic::is_type_floating(NumberType()))
        {
          *_svalue = std::to_string(_ldvalue);
        }
        else
        {
          *_svalue = std::to_string(_llivalue);
        }
      }

      /**
       * Clean up the value(s)
       */
      void CleanValues()
      {
        // delete the trivial value
        delete _tvalue;

        // delete the char if need be
        delete _cvalue;

        // delete the cosmetic strings
        delete _svalue;
        delete _swvalue;

        // reset the values
        _llivalue = 0;
        _ldvalue = 0;
        _lcvalue = 0;
        _ltvalue = 0;
        _cvalue = nullptr;
        _svalue = nullptr;
        _swvalue = nullptr;
        _tvalue = nullptr;
        _ptvalue = nullptr;
      }

      /**
       * Create a value from a double/float/long double number..
       * @param const T* number the number we are creating from.
       */
      template<class T>
      void CreateFromDouble(const T& number)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;

        // set the values
        _ldvalue = static_cast<long double>(number);
        _llivalue = static_cast<long long int>(_ldvalue);
      }

      /**
       * Create a value from a int/long/long long...
       * @param const T* number the number we are creating from.
       */
      template<class T>
      void CreateFromInteger(const T& number)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::get_type<T>::value;

        // set the values.
        _llivalue = static_cast<long long int>(number);
        _ldvalue = static_cast<long double>(_llivalue);
      }

      /**
       * Create from a trivally copyable value.
       * Objects of trivially - copyable types are the only C++ objects that may be safely copied with std::memcpy
       * @param const T& trivial the structure/class we want to copy from.
       */
      template<class T>
      std::enable_if_t<!std::is_pointer<T>::value> CreateFromTrivial( T trivial)
      {
        // as it is not a pointer value, it has to be trivially copyable.
        if (!std::is_trivially_copyable<T>::value)
        {
          throw std::bad_cast();
        }

        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::Misc_trivial;

        // set the values.
        _llivalue = 0;
        _ldvalue = 0;

        // copy the trival value.
        _ltvalue = sizeof(T);
        _tvalue = new char[_ltvalue];
        std::memset(_tvalue, 0, _ltvalue);
        std::memcpy(_tvalue, &trivial, _ltvalue);
      }

      /**
       * Create from a trivally copyable value.
       * Objects of trivially - copyable types are the only C++ objects that may be safely copied with std::memcpy
       * @param const T& trivial the structure/class we want to copy from.
       */
      template<class T>
      std::enable_if_t<std::is_pointer<T>::value> CreateFromTrivial(const T& trivial)
      {
        // clear all the values.
        CleanValues();

        // set the type
        _type = dynamic::Misc_trivial_ptr;

        // set the values.
        _llivalue = 0;
        _ldvalue = 0;

        // copy the trival pointer value that was given to us.
        _ptvalue = (void*)trivial;
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
          // get the number of characters.
          _lcvalue = (std::strlen((const char*)value)+1) * sizeof(T);

          // create the character, we know it is at least one, even for an empty string.
          _cvalue = new char[_lcvalue];

          // memory clear
          std::memset(_cvalue, '\0', _lcvalue);
          std::memcpy(_cvalue, value, _lcvalue);

          if (_lcvalue > 1)
          {
            // it does not matter if this is signed or not signed
            // we are converting it to an unsigned long long and back to a long long
            // in reality they both take the same amount of space.
            _llivalue = static_cast<long long int>(std::strtoull((const char*)value, nullptr, 0));

            // try and get the value as a long double.
            // this is represented in a slightly different way in memory
            // hence the reason we cannot just cast our long long to long double.
            _ldvalue = std::strtold((const char*)value, nullptr);
          }
          else
          {
            // the size is only one, (for our trailling '\0')
            // so we know that the value has to be zero.
            _llivalue = 0;
            _ldvalue = 0;
          }
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

        // parse the string to set the string flag
        ParseStringStatus((const char*)value);
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

          // create the character, we know it is at least one, even for an empty string.
          _cvalue = new char[_lcvalue];

          // memory clear
          std::memset(_cvalue, '\0', _lcvalue);
          std::memcpy(_cvalue, value, _lcvalue);

          if (_lcvalue > 1)
          {
            // it does not matter if this is signed or not signed
            // we are converting it to an unsigned long long and back to a long long
            // in reality they both take the same amount of space.
            _llivalue = static_cast<long long int>(std::wcstoull((const wchar_t*)value, nullptr, 0));

            // try and get the value as a long double.
            // this is represented in a slightly different way in memory
            // hence the reason we cannot just cast our long long to long double.
            _ldvalue = std::wcstold((const wchar_t*)value, nullptr);
          }
          else
          {
            // the size is only one, (for our trailling '\0')
            // so we know that the value has to be zero.
            _llivalue = 0;
            _ldvalue = 0;
          }
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

        // parse the string to set the string flag
        ParseStringStatus((const wchar_t*)value);
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
        std::memset(_cvalue, '\0', _lcvalue);
        std::memcpy(_cvalue, &value, _lcvalue);

        if (value >= '0' && value <= '9')
        {
          auto number = value - '0';
          _llivalue = number;
          _ldvalue = number;

          //  if has to be a non floating point number.
          _stringStatus = StringStatus_Pos_Number;
        }
        else
        {
          _llivalue = 0;
          _ldvalue = 0;

          //  this is not a number.
          _stringStatus = StringStatus_Not_A_Number;
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
        std::memset(_cvalue, '\0', _lcvalue);
        std::memcpy(_cvalue, &value, _lcvalue);

        // copy it.
        if (value >= L'0' && value <= L'9')
        {
          auto number = value - L'0';
          _llivalue = number;
          _ldvalue = number;

          //  if has to be a non floating point number.
          _stringStatus = StringStatus_Pos_Number;
        }
        else
        {
          _llivalue = 0;
          _ldvalue = 0;

          //  this is not a number.
          _stringStatus = StringStatus_Not_A_Number;
        }
      }

      /**
       * depending on the type we return if we should use the unsigned integer in a formula
       * @return bool if we should use the long long int as an unsigned signed integer.
       */
      bool UseUnsignedInteger() const
      {
        // divide the values and set it.
        switch (NumberType())
        {
        case dynamic::Integer_unsigned_short_int:
        case dynamic::Integer_unsigned_int:
        case dynamic::Integer_unsigned_long_int:
        case dynamic::Integer_unsigned_long_long_int:
          return true;
          break;
        }
        return false;
      }

      /**
       * depending on the type we return if we should use the signed integer in a formula
       * @return bool if we should use the long long int as a signed integer.
       */
      bool UseSignedInteger() const
      {
        // divide the values and set it.
        switch (NumberType())
        {
        case dynamic::Integer_short_int:
        case dynamic::Integer_int:
        case dynamic::Integer_long_int:
        case dynamic::Integer_long_long_int:
          return true;
          break;
        }
        return false;
      }

      /**
       * Divide *this number with T number.
       * @param dynamic::Type type the type we want to set the value with.
       * @param T number the number we will be dividing with.
       * @return *this the divided number.
       */
      template<class T>
      Any& DivideNumber(dynamic::Type type, T number)
      {

        // check for division by zero.
        if (number == 0)
        {
          // *this / null = std::overflow_error
          throw std::overflow_error("Division by zero.");
        }

        // divide the values and set it.
        if (UseUnsignedInteger())
        {
          CastFrom((unsigned long long int)_llivalue / (long double)number);
        }
        else if (UseSignedInteger())
        {
          CastFrom(( long long int)_llivalue / (long double)number);
        }
        else
        {
          CastFrom( _ldvalue / (long double)number);
        }

        // update the type.
        _type = type;

        // return the value.
        return *this;
      }

      /**
       * Multiply T number and *this number.
       * @param dynamic::Type type the type we want to set the value with.
       * @param T number the number we will be adding.
       * @return *this the multiplied number.
       */
      template<class T>
      Any& MultiplyNumber(dynamic::Type type, T number)
      {
        // add the values.
        if (dynamic::is_type_floating(type))
        {
          CastFrom(_ldvalue * number);
        }
        else
        {
          CastFrom(_llivalue * number);
        }

        // update the type.
        _type = type;

        // done
        return *this;
      }

      /**
       * Add T number to *this number.
       * @param dynamic::Type type the type we want to set the value with.
       * @param T number the number we will be adding.
       * @return *this the added number.
       */
      template<class T>
      Any& AddNumber(dynamic::Type type, T number)
      {
        if (UseUnsignedInteger())
        {
          CastFrom((unsigned long long int)_llivalue + number);
        }
        else if (UseSignedInteger())
        {
          CastFrom((long long int)_llivalue + number);
        }
        else
        {
          CastFrom(_ldvalue + number);
        }

        // update the type.
        _type = type;

        // done
        return *this;
      }

      /**
       * Subtract T number from *this.
       * @param dynamic::Type type the type we want to set the value with.
       * @param T number the number we will be subtracting from this.
       * @return *this the subtracted number.
       */
      template<class T>
      Any& SubtractNumber(dynamic::Type type, T number)
      {
        if (UseUnsignedInteger())
        {
          CastFrom((unsigned long long int)_llivalue - number);
        }
        else if (UseSignedInteger())
        {
          CastFrom((long long int)_llivalue - number);
        }
        else
        {
          CastFrom(_ldvalue - number);
        }

        // update the type.
        _type = type;

        // done
        return *this;
      }

      // the string status, does it represent a number? a floating number?
      // is it a partial or non partial number?
      enum StringStatus{
        StringStatus_Not_A_Number,                    // 'blah' or 'blah123'
        StringStatus_Partial_Pos_Number,              // '+123blah'
        StringStatus_Partial_Neg_Number,              // '-123blah'

        StringStatus_Pos_Number,                      // '+123' or '123' or '0' or '-0'
        StringStatus_Neg_Number,                      // '-123'

        StringStatus_Floating_Partial_Pos_Number,    // '+123.00blah' or '123.00blah'
        StringStatus_Floating_Partial_Neg_Number,    // '-123.00blah'
        StringStatus_Floating_Pos_Number,            // '+123.1' or '123.1' or '0.1' or '-0.1'
        StringStatus_Floating_Neg_Number,            // '-123.1'
      };

      /**
       * Check if this is a trivial type or not.
       * @return boolean if *this is trivial or not.
       */
      bool IsTrivial() const
      {
        switch (Type())
        {
        case dynamic::Misc_trivial:
        case dynamic::Misc_trivial_ptr:
          // trivial
          return true;

        default:
          break;
        }

        // no trivial
        return false;
      }

      /** 
       * Check if our string is a number or not.
       * @param bool allowPartial if partial strings are allowed or not.
       * @return bool if this string represents a number or not.
       */
      bool IsStringNumber( bool allowPartial ) const
      {
        switch (_stringStatus)
        {
        case myodd::dynamic::Any::StringStatus_Partial_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Partial_Neg_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Partial_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Partial_Neg_Number:
          return allowPartial; // only true if we allow partials.

        case myodd::dynamic::Any::StringStatus_Floating_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Floating_Neg_Number:
        case myodd::dynamic::Any::StringStatus_Pos_Number:
        case myodd::dynamic::Any::StringStatus_Neg_Number:
          return true;

        case myodd::dynamic::Any::StringStatus_Not_A_Number:
        default:
          break;
        }

        // not a number
        return false;
      }

      /**
       * Parse a string to see if it is a number, partial or not.
       * @param const char *str the string we are parsing.
       */
      void ParseStringStatus(const char *str)
      {
        //  call the const char* equivalent.
        ParseStringStatus(str, '+', '-', '.', '\0');
      }

      /**
      * Parse a string to see if it is a number, partial or not.
      * @param const wchar_t *str the string we are parsing.
      */
      void ParseStringStatus(const wchar_t *str)
      {
        //  call the const wide char* equivalent.
        ParseStringStatus(str, L'+', L'-', L'.', L'\0');
      }

      /**
       * check if this is a digit char, (0-9)
       * @param const char c the character we are checking.
       * @return bool if the number is a digit or not.
       */
      inline bool _isdigit(const char c) const { return (isdigit(c) != 0);}

      /**
       * check if this is a space wide char
       * @param const wchar_t c the character we are checking.
       * @return bool if the char is a space or not.
       */
      inline bool _isspace(const char c) const { return (isspace(c) != 0); }

      /**
       * check if this is a digit wide char, (0-9)
       * @param const wchar_t c the character we are checking.
       * @return bool if the number is a digit or not.
       */
      inline bool _isdigit(const wchar_t c)const { return (iswdigit(c) != 0); }

      /**
       * check if this is a space wide char
       * @param const wchar_t c the character we are checking.
       * @return bool if the char is a space or not.
       */
      inline bool _isspace(const wchar_t c)const { return (iswspace(c) != 0); }

      /**
       * Parse a string to check if it is a number or not.
       * -0 and +0 keep their sign and 12.00 remains a floating point.
       * because this is how it was pased to us, it is up to the user to make sure they pass
       * a valid number that makes it posible to investigate.
       * @param const T* str the string we are checking.
       * @param const T str_plus the plus sign, ('+')
       * @param const T str_minus the minus sign, ('-')
       * @param const T str_decimal how a decimal is represented, , ('.')
       * @param const T str_eol the eol character, ('\0')
       */
      template<typename T>
      void ParseStringStatus( const T* str, const T str_plus, const T str_minus, const T str_decimal, const T str_eol )
      {
        // sanity check
        if (nullptr == str)
        {
          // null is not a number
          _stringStatus = StringStatus_Not_A_Number;
          return;
        }

        short sign = 0;       //  0=unknown, 1=positive, 2=negative.
        short found = 0;      // the number of ... numbers we found.
        bool partial = false; // if we found some non characters.
        bool decimal = false;

        // go around all the characters.
        for (const T *it = str; *it != str_eol; *it++)
        {
          //  is it a space?
          if ( _isspace(*it))
          {
            continue;
          }

          // minus sign? as long as we have not done it yet...
          if (*it == str_minus && sign == 0)
          {
            sign = 2;
            continue;
          }

          // + sign? as long as we have not done it yet...
          if (*it == str_plus && sign == 0)
          {
            sign = 1;
            continue;
          }
          
          // is it a decimal? as long as we have done done it.
          if (*it == str_decimal && false == decimal)
          {
            decimal = true;
            continue;
          }

          // is it a digit?
          if (!_isdigit(*it))
          {
            // not a number, (anymore)
            partial = true;
            break;
          }

          // this is a number
          ++found;
        }

        //
        //  using all the values together we need to check what king of string this is.
        //
        if (0 == found)
        {
          // we found no number at all, so it cannot be a string.
          // of by the time we found a non string, we had no number.
          _stringStatus = StringStatus_Not_A_Number;
        }
        else if( true == partial )
        {
          if (sign == 1 || sign == 0)
          {
            //  '+' sign or no sign - it is positive.
            _stringStatus = (decimal) ? StringStatus_Floating_Partial_Pos_Number : StringStatus_Partial_Pos_Number;
          }
          else if (sign == 2)
          {
            // -ve sign.
            _stringStatus = (decimal) ? StringStatus_Floating_Partial_Neg_Number : StringStatus_Partial_Neg_Number;
          }
        }
        else
        {
          if (sign == 1 || sign == 0)
          {
            //  '+' sign or no sign - it is positive.
            _stringStatus = (decimal) ? StringStatus_Floating_Pos_Number : StringStatus_Pos_Number;
          }
          else if (sign == 2)
          {
            // -ve sign.
            _stringStatus = (decimal) ? StringStatus_Floating_Neg_Number : StringStatus_Neg_Number;
          }
        }
      }

      // the biggest integer value.
      long long int _llivalue;

      // the biggest floating point value
      long double _ldvalue;

      // this is the given character value either char/signed char/unsigned char/wide
      char* _cvalue;
      size_t _lcvalue;

      void* _ptvalue;
      char* _tvalue;
      size_t _ltvalue;

      // the status of the string.
      StringStatus _stringStatus;

      // 'cosmetic' representations of the numbers, both wide and non wide strings.
      // the values are only created if/when the caller call a to string function.
      std::string* _svalue;
      std::wstring* _swvalue;

      // the variable type
      dynamic::Type _type;
    };
  }
}