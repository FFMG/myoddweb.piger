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

#include <string>
#include <sstream> 

//
// Classes taken in part from http://stackoverflow.com/questions/12261915/howto-throw-stdexceptions-with-variable-messages
//
namespace myodd {
  namespace strings {
    // wide type.
    class FormatterW
    {
    public:
      FormatterW() {}
      ~FormatterW() {}

      template <typename Type>
      FormatterW & operator << (const Type & value)
      {
        stream_ << value;
        return *this;
      }

      std::wstring str() const { return stream_.str(); }
      operator std::wstring() const { return stream_.str(); }

      enum ConvertToString
      {
        to_str
      };
      std::wstring operator >> (ConvertToString) { return stream_.str(); }

    private:
      std::wstringstream stream_;

      // prevent copies
      FormatterW(const FormatterW &);
      FormatterW & operator = (FormatterW &);
    };

    // ascii type
    class FormatterA
    {
    public:
      FormatterA() {}
      ~FormatterA() {}

      template <typename Type>
      FormatterA & operator << (const Type & value)
      {
        stream_ << value;
        return *this;
      }

      std::string str() const { return stream_.str(); }
      operator std::string() const { return stream_.str(); }

      enum ConvertToString
      {
        to_str
      };
      std::string operator >> (ConvertToString) { return stream_.str(); }

    private:
      std::stringstream stream_;

      // prevent copies
      FormatterA(const FormatterA &);
      FormatterA & operator = (FormatterA &);
    };

    //  for the lazy
#ifdef _UNICODE
    class Formatter : public FormatterW {};
#else
    class Formatter : public FormatterA {};
#endif
  }
}