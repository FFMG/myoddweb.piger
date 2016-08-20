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
      Integer_unsigned_long_long_int
    };
  }
}