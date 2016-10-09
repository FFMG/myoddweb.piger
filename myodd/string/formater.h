#pragma once

#include <string>
#include <sstream> 

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