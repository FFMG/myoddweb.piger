// ***********************************************************************
// Copyright (c) 2022 Florent Guelfucci
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
#include "commandline_variables.h"

namespace myodd {
  commandline_variables::commandline_variables(int argc, wchar_t* args[], bool allow_unregistered)
  {
    const wchar_t* prefix = L"-";

    //  by definition the fist one is the exe path
    PathToExe = argc > 0 ? args[0] : L"";

    // just save the values in the vector of values.
    for (int i = 0; i < argc; ++i)
    {
      _values.push_back(args[i]);
    }

    // now parse all the arguments
    for (int i = 1; i < argc; ++i)
    {
      // is it a key?
      std::wstring actual;
      if (IsKey(args[i], prefix, actual ))
      {
        // if the next one is not a value then we are done
        if (i + 1 == argc )
        {
          _key_values[actual] = -1;
          break;
        }

        std::wstring next_actual;
        if (IsKey(args[i + 1], prefix, next_actual))
        {
          _key_values[actual] = -1;
          continue;
        }

        // the next value is a given value.
        // move forward to the next item and set that value as the actual
        _key_values[actual] = ++i;
      }
      else
      {
        // this is not very good, it mean that the user passed a value like
        // --key foo bar
        // in the conditional above we have set [key] = foo
        // but now we do not knwo what to do with 'bar'
        // so we are just going to leave the value as a key.
        _key_values[args[i]] = -1;
      }
    }
  }

  bool commandline_variables::contains(const wchar_t* value) const
  {
    const auto& it = _key_values.find(value);
    return (it != _key_values.end());
  }

  /// <summary>
  /// Get the value for a cerain argument
  /// </summary>
  /// <param name="value"></param>
  /// <param name="valueIfNotFound"></param>
  /// <returns></returns>
  const wchar_t* commandline_variables::get(const wchar_t* value, const wchar_t* valueIfNotFound ) const
  {
    const auto& it = _key_values.find(value);
    if (it == _key_values.end())
    {
      return valueIfNotFound;
    }

    //  there is no value
    if ((*it).second == -1)
    {
      return nullptr;
    }
    return _values[(*it).second].c_str();
  }

  bool commandline_variables::IsKey(const wchar_t* value, const wchar_t* prefix, std::wstring& actual) const
  {
    const auto& prefix_len = wcslen(prefix);
    const auto& value_len = wcslen(value);
    if (value_len <= prefix_len)
    {
      //  if the value len is less than the prefix len then we cannot be a key
      // if it is the same, it does not make sense
      // it means we would have something like "example.exe -- foo
      return false;
    }

    // possible value is the value without what we thing might be the prefix
    std::wstring possible_prefix = std::wstring(value).substr(0, prefix_len);
    if (possible_prefix != prefix)
    {
      // the characters do not look like a prefix.
      return false;
    }

    // the characters look like a prefix.
    // so we will strip them and set the value,
    actual = std::wstring(value).substr(prefix_len);

    return true;
  }
}