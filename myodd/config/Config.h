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
#include "../dynamic/any.h"

namespace myodd{ namespace config{
  void set(const std::wstring& path, const myodd::dynamic::Any& any);
  
  /**
   * Try and get a value, if the value is not found, we throw.
   * @param const std::wstring& path the name of the value we are looking for.
   * @return ::myodd::dynamic::Any the value, if it exists.
   */
  ::myodd::dynamic::Any get(const std::wstring& path);

  /**
   * Try and get a value, if the value is not found, we throw.
   * @param const std::wstring& path the name of the value we are looking for.
   * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
   * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
   */
  ::myodd::dynamic::Any get(const std::wstring& path, const myodd::dynamic::Any& defaultValue );

  /**
   * Check if the value has been set or not.
   * @param const std::wstring& path the path we are looking for.
   * @return bool if the path exists or not.
   */
  bool isset(const std::wstring& path);

  void free();

  /**
   * Create the data holder from and XML
   * @path const std::wstring& path the path of the xml we want to load.
   * @return bool success or not.
   */
  bool FromXMLFile( const std::wstring& path );
} //  config
} //  myodd
