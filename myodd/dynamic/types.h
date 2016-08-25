#pragma once

/**
 * List all the dynamic types.
 */
namespace myodd {
  namespace dynamic {
    enum Type
    {
      Misc_unknown,
      Misc_null,

      // bool
      Boolean_bool,

      // character
      Character_signed_char,
      Character_unsigned_char,
      Character_char,
      Character_wchar_t,

      // Integer
      Integer_short_int,
      Integer_unsigned_short_int,
      Integer_int,
      Integer_unsigned_int,
      Integer_long_int,
      Integer_unsigned_long_int,
      Integer_long_long_int,
      Integer_unsigned_long_long_int,

      // Floating point
      Floating_point_float,
      Floating_point_double,
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
    inline bool is_Misc_null(Type type)
    {
      switch (type)
      {
      case Misc_null:
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
      static constexpr dynamic::Type value = dynamic::Misc_unknown;
    };

    template<>
    struct get_type<short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_short_int;
    };

    template<>
    struct get_type<const short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_short_int;
    };

    template<>
    struct get_type<unsigned short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_short_int;
    };

    template<>
    struct get_type<const unsigned short int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_short_int;
    };

    template<>
    struct get_type<int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_int;
    };

    template<>
    struct get_type<const int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_int;
    };

    template<>
    struct get_type<unsigned int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_int;
    };

    template<>
    struct get_type<const unsigned int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_int;
    };

    template<>
    struct get_type<long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_int;
    };

    template<>
    struct get_type<const long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_int;
    };

    template<>
    struct get_type<unsigned long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_int;
    };

    template<>
    struct get_type<const unsigned long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_int;
    };

    template<>
    struct get_type<long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_long_int;
    };

    template<>
    struct get_type<const long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_long_long_int;
    };

    template<>
    struct get_type<unsigned long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_long_int;
    };

    template<>
    struct get_type<const unsigned long long int>
    {
      static constexpr dynamic::Type value = dynamic::Integer_unsigned_long_long_int;
    };

    template<>
    struct get_type<double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_double;
    };

    template<>
    struct get_type<const double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_double;
    };

    template<>
    struct get_type<float>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_float;
    };

    template<>
    struct get_type<const float>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_float;
    };

    template<>
    struct get_type<long double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_long_double;
    };

    template<>
    struct get_type<const long double>
    {
      static constexpr dynamic::Type value = dynamic::Floating_point_long_double;
    };

    template<>
    struct get_type<bool>
    {
      static constexpr dynamic::Type value = dynamic::Boolean_bool;
    };

    template<>
    struct get_type<const bool>
    {
      static constexpr dynamic::Type value = dynamic::Boolean_bool;
    };

    template<>
    struct get_type<unsigned char>
    {
      static constexpr dynamic::Type value = dynamic::Character_unsigned_char;
    };

    template<>
    struct get_type<const unsigned char>
    {
      static constexpr dynamic::Type value = dynamic::Character_unsigned_char;
    };

    template<>
    struct get_type<signed char>
    {
      static constexpr dynamic::Type value = dynamic::Character_signed_char;
    };

    template<>
    struct get_type<const signed char>
    {
      static constexpr dynamic::Type value = dynamic::Character_signed_char;
    };

    template<>
    struct get_type<char>
    {
      static constexpr dynamic::Type value = dynamic::Character_char;
    };

    template<>
    struct get_type<const char>
    {
      static constexpr dynamic::Type value = dynamic::Character_char;
    };

    template<>
    struct get_type<wchar_t>
    {
      static constexpr dynamic::Type value = dynamic::Character_wchar_t;
    };

    template<>
    struct get_type<const wchar_t>
    {
      static constexpr dynamic::Type value = dynamic::Character_wchar_t;
    };
  }
}