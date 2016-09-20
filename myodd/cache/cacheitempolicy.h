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

#include <time.h>

namespace myodd {
  namespace cache {
    /**
     * Implementation of CacheItemPolicy
     * https://msdn.microsoft.com/en-us/library/system.runtime.caching.cacheitem(v=vs.110).aspx
     */
    class CacheItemPolicy
    {
    public:
      CacheItemPolicy();
      CacheItemPolicy(time_t absoluteExpiration );

      // set the expiration time.
      void SetAbsoluteExpiration(time_t absoluteExpiration );

      // get the expiration time
      time_t GetAbsoluteExpiration() const;

      // check if the item has expired or not.
      bool HasExpired() const;

    private:
      // The period of time that must pass before a cache entry is evicted. The default value is InfiniteAbsoluteExpiration, meaning that the entry does not expire
      time_t _absoluteExpiration;
    };
  }
}
