## Description

#### Example

    // simple declaration
    auto any = myodd::dynamic::Any( 123 ) + myodd::dynamic::Any( 456 );

    // convert to a string
    std::string mystring = any;  // "579"

    // convert to a wide string
    std::wstring mystring = any;  // L"579"

## Arithmetic rules (+,-,*,/)

- can be done on integers.
- Can be done on floating points.
- Can be done on strings, (wide and non wide strings).
- Can be done on char types, (wide and non wide char).
- Can be done on booleans

### Division by zero

In the case of divisions by zero, a `std::overflow_error` is thrown.

- when dividing by a 'false' boolean
- when dividing by a zero char, ('0' and '\0')
- when dividing by a null pointer.
- when dividing by a string that cannot be converted to a non zero number.

#### Example

    auto bad = myodd::dynamic::Any( 123 ) / 0; // = std::overflow_error
    auto bad = 123 / myodd::dynamic::Any( 0 ); // = std::overflow_error
    auto bad = 1 / myodd::dynamic::Any( null_ptr );   // = std::overflow_error
    auto bad = 1 / myodd::dynamic::Any( "No a number" );   // = std::overflow_error

## Todo

- implement [std::is_trivially_copyable](http://en.cppreference.com/w/cpp/types/is_trivially_copyable) to allow structures to be held in memory.  
Those objects cannot have arithmetic done to them, (+,-,/,*)