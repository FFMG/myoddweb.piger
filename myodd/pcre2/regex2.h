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
#pragma warning(disable : 4305)
#pragma warning(disable : 4306)
#pragma warning(disable : 4702)
#pragma warning(disable : 4706)
#endif /* defined(_MSC_VER) */

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
      int u8match(const wchar_t *rePattern,
        const wchar_t *wsubject,
        bool caseSensitive = false
      )const;

      int u8match(const wchar_t *rePattern,
        const wchar_t *wsubject,
        matches& wmatches,
        bool caseSensitive = false
      )const;

      int u8replace(const wchar_t* rePattern,
        const wchar_t* replacement,
        const wchar_t* subject,
        std::wstring& replaceResult,
        bool caseSensitive = false
      ) const;

      int u8replace(const wchar_t* rePattern,
        const wchar_t* replacement,
        std::wstring& subjectResult,
        bool caseSensitive = false
      ) const;

    protected:
      pcre2_code *u8compile(const wchar_t *pattern, bool caseSensitive) const;

      int u8match(const wchar_t *rePattern,
        const wchar_t *subject,
        matches* pu8matches,
        bool caseSensitive = false
      )const;
    };
  }
};
