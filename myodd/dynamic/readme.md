## Introduction
This class tries to introduce dynamic typing to c++, basically we try to remove the 'type' from the variable.
Rather the type is decided at runtime.

### Home project

The home project is [myoddweb.any](https://github.com/FFMG/myoddweb.any), this is where the latest code is located and kept up to date.

## Installation

`::myodd::dynamic::Any` is a standalone class that consist of 2 files, `types.h` and `any.h` so just copy those 2 files where ever you need them, (in a subfolder might be a good idea).

Include them in your project as you would include any other classes.

The code was written and tested on `c++11`, I *suspect* it will work on older compiler with a couple of macros, but I will cross that bridge if/when someone ask for it.

## Usage

Just include the file where ever you placed it and then use the class.

    #include "location/any.h"

    int main() {

      // declare it
      ::myodd::dynamic::Any life(42);

      // use it...
     
      return 0;
    }

or you can call `using` if you prefer, (of course).

    #include "location/any.h"

    using ::myodd::dynamic::Any;

    int main() {

      // declare it
      Any life(42);

      // use it...
     
      return 0;
    }

### Rules of the game

Like any dynamic types, myodd::dynamic::Any() has to have some sort of [type juggling](http://php.net/manual/en/language.types.type-juggling.php) rules to follow.

    auto foo = myodd::dynamic::Any(); // foo is 'null', the default
    foo = "1";  // foo is string... for now
    foo *= 2;   // foo is now an integer with a value of 2
    foo = foo * 1.3;  // foo is now a double with a value of 2.6
    foo = 3 * "Hello"; // foo is now an integer with a value of zero, ('cause 5*"blah" == 0)
    foo = 5 * "10";    // foo is now an integer with a value of 50, ('cause 5*"10" == 50, we clever like that)

You can also cast to many fundamental types.

    auto foo = myodd::dynamic::Any("Hello"); // foo is 'Hello'
    auto bar = (int)foo; // bar is an integer with a value of zero, ('cause 'blah' == 0 until math changes its rules).

Equality

    auto foo = myodd::dynamic::Any("10"); // foo is a string "10"
    auto bar = myodd::dynamic::Any(10); // foo is an integer 10
    ...
    foo == bar; // true because 10 is 10.
    true == 1234; // anything not zero is true

[Arithmetic Conversions](doc/arithmeticconversions.md)

Arithmetic Conversion is what happens when you add/subtract/divide/etc 2 numbers together.

See the [Arithmetic Conversions](doc/arithmeticconversions.md) document for more details.

#### Pointer rules.
- you can pass a null pointer, (Any(nullptr) )
- you can pass a null pointer to a variable, (Any( (int*)nullptr))

#### String rules
- If it is a number then it is an `unsigned long long int` or a `long long int` or an `long double`
- If it is a partial number, and the number is first, then it is also an `unsigned long long int` or a `long long int` or an `long double`
	- For example `-12asee` will be used as `-12` in an equation.
	- Or `-0-----` will be used as `0`
- In the case of comparison, only exact numbers are compared.
	- `"-9" == "-0009.0000"`
	- but `"-9" != "-9.a"`
- Decimal strings are respected, `"-121.000"` will become a `long double`, in real terms this does not really make a difference with most compilers/interpretations, (but it is something to bear in mind).

#### Arithmetic rules.

- Non number strings are equal to zero, (so "Hello"=0 and "Hello" + "World"=0)
- true=1 and false=0
	- 2 * false = 0;  // 2*0=0
	- 3 * true = 3; // 3*1=3
- Number strings equal the number, (so "1" + "41" = 42)
	- '0' -> '9' for `char`/`wchar_t`
	- any valid looking number, "1234.456" = (double)1234.456
- By default the floating point number is a `double`, if more space is needed is it elevated to a `long double`.

### Who else does dynamic typing?

Many languages use dynamic typing, but some of the popular ones are...

- php
- javascript
- Lua
- Perl
- Python
- Ruby
- C#, (well, to an extend it does)

Similar apps that use 

### Use cases
#### Map and/or Vectors
In a vector you could totally remove the data type

    #include "dynamic/any.h"
    #include <vector>
    ...
    std::vector<myodd::dynamic::Any> sampledata{ 10, "Hello", L"World", 20.5 };
    ...
    if( sampledata[2] == L"World" )
    {
      // do something cool
    }
    else
    if( sampledata[3] == 42 )
    {
      // universe == solved!
    }

#### Structure/classes.
You can pass so called, trivial structures and classes.

    struct TrivialStruct
    {
      int a;
      int b;
    };
    
    auto ts = { 10, 20 };
    
    ::myodd::dynamic::Any any = ts;
    
    TrivialStruct ts2 = any; // ts2.a = 10 && ts2.b = 20;

#### Unknown type
Unknown use case at run time, you could for example read some data from the database, and the data could be either a number, a wide string, a double... anything.

#### Supported types
You can query the current data type of the variable as we understand it. Call `Any::Type()` and the return value is the data type.  

##### Fundamental types
###### Special types
- null (nullptr_t)
- unknown (*should* never happen!)

###### Boolean
- bool

###### Character
- signed char
- unsigned char
- char
- wchar_t

###### Integer
- short int
- unsigned short int
- int
- unsigned int
- long int
- unsigned long int
- long long int
- unsigned long long int

###### Floating point
- float
- double
- long double

#### Example

    // simple declaration
    auto any = myodd::dynamic::Any( 123 ) + myodd::dynamic::Any( 456 );

    // convert to a string
    std::string mystring = any;  // "579"

    // convert to a wide string
    std::wstring mystring = any;  // L"579"

## Arithmetic rules (+,-,*,/)

- can be done on integers (1+1=2).
- Can be done on floating points (1.1+1.1=2.2).
- Can be done on strings, (wide and non wide strings), "10"+"1"=11".
- Can be done on char types, (wide and non wide char), '1'+'1'=2.
- Can be done on booleans, (true+false=1)

See the special cases in the rules of the game.

### Examples

nullptr_t is treated as 0

    auto any = myodd::dynamic::Any( nullptr_t ) * myodd::dynamic::Any( nullptr_t ); // = 0; nullptr_t*nullptr_t==0

false is treated as 0

    auto any1 = myodd::dynamic::Any( false ) * 17; // = 0; false*17==0
    auto any2 = myodd::dynamic::Any( false ) + 17; // = 17; false+17==17

You can add wide and non wide strings

    auto string1 = myodd::dynamic::Any("12");
    auto string2 = myodd::dynamic::Any(L"12");	//	wide
    auto add = string1 + string2; // = 24

### Division by zero

In the case of divisions by zero, a `std::overflow_error` is thrown.

- when dividing by a 'false' boolean
- when dividing by a zero char or a zero string, ('0', '\0' or "0")
- when dividing by a null pointer.
- when dividing by a string that cannot be converted to a non zero number.

#### Examples

    auto bad = myodd::dynamic::Any( 123 ) / 0; // = std::overflow_error
    auto bad = 123 / myodd::dynamic::Any( 0 ); // = std::overflow_error
    auto bad = 1 / myodd::dynamic::Any( nullptr_t );   // = std::overflow_error
    auto bad = 1 / myodd::dynamic::Any( "No a number" );   // = std::overflow_error

## Performance comparison.

While this level of flexibility will never result in an equal performance with the fundamental types, it is always good to see how well this will perform. 

### Version 0.1
#### [Addition loop](doc/perfaddition.md)
- std : `0.003s` | `0.003s`
- any : `0.108s` | `0.103s`

#### [Subtraction loop](doc/perfsubtraction.md)
- std : `0.003s` | `0.003s`
- any : `0.107s` | `0.108s`

#### [Multiplication loop](doc/perfmultiplication.md)

- std : `0.003s` | `0.003s`
- any : `0.170s` | `0.141s`

#### [Division loop](doc/perfdivision.md)

- std : `0.003s` | `0.003s`
- any : `0.072s` | `0.079s`

## Todo

- <strike>implement [std::is_trivially_copyable](http://en.cppreference.com/w/cpp/types/is_trivially_copyable) to allow structures to be held in memory.</strike> *(done 30/08/2016)*  
- implement [std::is_copy_constructible](http://en.cppreference.com/w/cpp/types/is_copy_constructible) to allow classes to be added.  
Those objects cannot have arithmetic done to them, (+,-,/,*)
- <strike>Removed new/delete of `long long int` and `long double` as it is slow(er) and been on the stack is often better.</strike> *(done 24/08/2016)*
- <strike>Assign `std::string` and `std::wstring`  
myodd::dynamic::And( std::string("Hello") );</strike> *(done 25/08/2016)*
- Add the ! operator, (the logical negation operator).
- Manage [arithmetic conversions](http://stackoverflow.com/a/4814901/4584436) properly [[1]](http://en.cppreference.com/w/cpp/language/operator_arithmetic#Conversions)[[2]](https://docs.oracle.com/cd/E19205-01/819-5265/bjabw/index.html).
- Create sample projects
	- vs2015
	- gcc
- <strike>Implement pointer option so the user can do :   
`x = new MyClass();`   
`auto y = ::myodd::dynamic::Any( x );` </strike> *(done 30/08/2016)*
