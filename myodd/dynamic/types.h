#pragma once

/**
 * List all the dynamic types.
 */
namespace myodd {
  namespace dynamic {
    enum Type
    {
      type_unknown,
      type_null,

      // bool
      Boolean_bool,

      // character
      Character_signed_char,
      Character_unsigned_char,
      Character_char,
      Character_wchar_t,

      // short int
      Integer_short_int,

      // unsigned short int
      Integer_unsigned_short_int,

      // int
      Integer_int,

      // unsigned
      Integer_unsigned_int,

      // long int
      Integer_long_int,

      // unsigned
      Integer_unsigned_long_int,

      // long long int
      Integer_long_long_int,

      // unsigned
      Integer_unsigned_long_long_int,

      // float
      Floating_point_float,

      // double
      Floating_point_double,

      // long double.
      Floating_point_long_double
    };

    /**
     * Check if the given type is null or not.
     * @param const Type& type the type we are checking.
     * @return bool if it is or not.
     */
    inline bool is_type_character(Type type)
    {
      switch (type)
      {
      case Character_signed_char:
      case Character_unsigned_char:
      case Character_char:
      case Character_wchar_t:
        return true;
      }
      return false;
    }
    
    /**
     * Check if the given type is null or not.
     * @param const Type& type the type we are checking.
     * @return bool if it is or not.
     */
    inline bool is_type_null(Type type)
    {
      switch (type)
      {
      case type_null:
        return true;
      }
      return false;
    }

    /**
     * Check if the given type is a boolean or not.
     * @param const Type& type the type we are checking.
     * @return bool if it is or not.
     */
    inline bool is_type_boolean(Type type)
    {
      switch (type)
      {
      case Boolean_bool:
        return true;
      }
      return false;
    }

    /**
     * Check if the given type is a floating or not.
     * @param const Type& type the type we are checking.
     * @return bool if it is or not.
     */
    inline bool is_type_floating( Type type )
    {
      switch (type )
      {
      case Floating_point_float:
      case Floating_point_double:
      case Floating_point_long_double:
        return true;
      }
      return false;
    }

    /**
     * Check if the given type is an integer or not.
     * @param const Type& type the type we are checking.
     * @return bool if it is or not.
     */
    inline bool is_type_integer( const Type& type)
    {
      switch (type)
      {
      case Integer_short_int:
      case Integer_unsigned_short_int:
      case Integer_int:
      case Integer_unsigned_int:
      case Integer_long_int:
      case Integer_unsigned_long_int:
      case Integer_long_long_int:
      case Integer_unsigned_long_long_int:
        return true;
      }
      return false;
    }

    template<typename T>
    struct get_type
    {
      static constexpr dynamic::Type value = dynamic::type_unknown;
    };

    template<>
    struct get_type<short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_short_int;
    };

    template<>
    struct get_type<unsigned short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_short_int;
    };

    template<>
    struct get_type<int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_int;
    };

    template<>
    struct get_type<unsigned int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_int;
    };

    template<>
    struct get_type<long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_int;
    };

    template<>
    struct get_type<unsigned long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_int;
    };

    template<>
    struct get_type<long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_long_int;
    };

    template<>
    struct get_type<unsigned long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_long_int;
    };

    template<>
    struct get_type<double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_double;
    };

    template<>
    struct get_type<float>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_float;
    };

    template<>
    struct get_type<long double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_long_double;
    };

    template<>
    struct get_type<bool>
    {
      static constexpr dynamic::Type value = dynamic::Boolean_bool;
    };

    template<>
    struct get_type<unsigned char>
    {
      static constexpr dynamic::Type value = dynamic::Character_unsigned_char;
    };

    template<>
    struct get_type<signed char>
    {
      static constexpr dynamic::Type value = dynamic::Character_signed_char;
    };

    template<>
    struct get_type<char>
    {
      static constexpr dynamic::Type value = dynamic::Character_char;
    };

    template<>
    struct get_type<wchar_t>
    {
      static constexpr dynamic::Type value = dynamic::Character_wchar_t;
    };
  }
}