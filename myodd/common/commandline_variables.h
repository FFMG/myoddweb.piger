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
#include "includes.h"
#include <vector>
#include <map>

namespace myodd {
  class commandline_variables
  {
  private:
    /// <summary>
    /// The given path to the exe
    /// </summary>
    std::wstring PathToExe;

    /// <summary>
    /// All the raw values.
    /// </summary>
    std::vector<std::wstring> _values;

    /// <summary>
    /// They key value pairs
    /// A value of -1 refers to an option that does not require a value.
    /// like example.exe --foo --bar
    /// </summary>
    std::map<std::wstring, int> _key_values;
  public:
    commandline_variables(int argc, wchar_t* args[], bool allow_unregistered = true);

    /// <summary>
    /// Check if the arguments contain the given value.
    /// </summary>
    /// <param name="value"></param>
    /// <returns></returns>
    bool contains(const wchar_t* value) const;

    /// <summary>
    /// Get the value for a cerain argument
    /// </summary>
    /// <param name="value"></param>
    /// <param name="valueIfNotFound"></param>
    /// <returns></returns>
    const wchar_t* get(const wchar_t* value, const wchar_t* valueIfNotFound = nullptr ) const;

    /// <summary>
    /// Get a value at a given position
    /// </summary>
    /// <param name="position"></param>
    /// <returns></returns>
    const wchar_t* get_raw( int position ) const;

  private:
    /// <summary>
    /// Check if a value is a key or not.
    /// if it is then we will set the acual value without the prefix.
    /// </summary>
    /// <param name="value"></param>
    /// <param name="prefix"></param>
    /// <param name="actual"></param>
    /// <returns></returns>
    bool IsKey(const wchar_t* value, const wchar_t* prefix, std::wstring& actual) const;
  };
}
