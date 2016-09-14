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
#include "cacheitempolicy.h"
#include <stdexcept>
#include <limits.h>

namespace myodd {
  namespace cache {
    CacheItemPolicy::CacheItemPolicy() : 
      _absoluteExpiration(std::numeric_limits<time_t>::max() )
    {
    }

    CacheItemPolicy::CacheItemPolicy(time_t absoluteExpiration) :
      _absoluteExpiration(std::numeric_limits<time_t>::max())
    {
      // set the expoiration
      SetAbsoluteExpiration(absoluteExpiration);
    }

    /**
     * Set the absolute expiration time.
     * @param _int64 absoluteExpiration when this will be expiring.
     */
    void CacheItemPolicy::SetAbsoluteExpiration(time_t absoluteExpiration)
    {
      if (absoluteExpiration < 0)
      {
        throw std::runtime_error("The absolute time cannot be negative.");
      }
      _absoluteExpiration = absoluteExpiration;
    }

    /**
     * Get the absolute expiration time.
     * @return time_t the absolute expiration time.
     */
    time_t CacheItemPolicy::GetAbsoluteExpiration() const
    {
      return _absoluteExpiration;
    }
  }
}