#pragma once

/**
* Remove warnings that are thrown by pcre2.h
*/
#if defined(_MSC_VER)
#pragma warning(disable : 4054)
#pragma warning(disable : 4055)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4130)
#pragma warning(disable : 4152)
#pragma warning(disable : 4189)
#pragma warning(disable : 4206)
#pragma warning(disable : 4210)
#pragma warning(disable : 4232)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4305)
#pragma warning(disable : 4306)
#pragma warning(disable : 4702)
#pragma warning(disable : 4706)
#pragma warning(disable : 4996)
#endif /* defined(_MSC_VER) */

#ifndef PCRE2_CODE_UNIT_WIDTH
  #error You have to include the pre-processor for PCRE2
  #error   Use: 'PCRE2_CODE_UNIT_WIDTH = 16; HAVE_CONFIG_H; PCRE2_STATIC'
#endif // !PCRE2_CODE_UNIT_WIDTH

#include "..\pcre2\src\pcre2.h"
#include <vector>

namespace myodd {
  namespace regex {
    class Regex2
    {
    public:
      Regex2();
      virtual ~Regex2();

    public:
      typedef std::vector<std::wstring> matches;

    public:
      int Match(const wchar_t *rePattern,
        const wchar_t *wsubject,
        bool caseSensitive = false
      )const;

      int Match(const wchar_t *rePattern,
        const wchar_t *wsubject,
        matches& wmatches,
        bool caseSensitive = false
      )const;

      int Replace(const wchar_t* rePattern,
        const wchar_t* replacement,
        const wchar_t* subject,
        std::wstring& replaceResult,
        bool caseSensitive = false
      ) const;

      int Replace(const wchar_t* rePattern,
        const wchar_t* replacement,
        std::wstring& subjectResult,
        bool caseSensitive = false
      ) const;

    protected:
      pcre2_code *u8compile(const wchar_t *pattern, bool caseSensitive) const;

      int Match(const wchar_t *rePattern,
        const wchar_t *subject,
        matches* pmatches,
        bool caseSensitive = false
      )const;
    };
  }
};
