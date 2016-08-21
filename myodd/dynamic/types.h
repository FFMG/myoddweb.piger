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
  }
}