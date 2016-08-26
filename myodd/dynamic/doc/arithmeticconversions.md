# Arithmetic conversions

Arithmetic Conversion is what happens when you add/subtract/divide/etc 2 numbers together.

## General

The first true condition will set the data type.

- `long double` if either lhs/rhs is long double.
- `double` if either lhs/rhs is double.
- `float` if either lhs/rhs is float.
- `unsigned long long` if either is unsigned long long
- `long long` if either is long long
- `unsigned long` if either is unsigned long
- `unsigned long` if one is long and the other is unsigned int.
- `long` if either is long
- `int` is nothing else matches...

This is also true for strings and chars.
 
## Division

- `double` by default
- `long double` if either lhs/rhs is:
	- long double
	- long long int - signed/unsigned
	- long int - signed/unsigned  

## Overflow

The Arithmetic Conversion rules mean that you could have an overflow.
For example, `(int)2147483647 + (int)2147483647 = (int)4294967294` on a x86 application this would overflow, but with `Any` you can cast it to a long.

    // set the value
    int number = 2147483647;
    ::myodd::dynamic::Any bar(number);

    // add them together.
    bar += bar; // or multiply if you want to.
    bar.Type(); // = myodd::dynamic::Integer_int

    // but you can cast it if you want to.
    long long foo = (long long)bar; // = 4294967294


## Apendix
- Oracle : [Usual Arithmetic Conversions](https://docs.oracle.com/cd/E19205-01/819-5265/bjabw/index.html)
- Cpp reference - [Conversions](http://en.cppreference.com/w/cpp/language/operator_arithmetic#Conversions)