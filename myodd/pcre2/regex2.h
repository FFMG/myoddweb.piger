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
#define PCRE2_CODE_UNIT_WIDTH 16
#endif // !PCRE2_CODE_UNIT_WIDTH 

#ifndef HAVE_CONFIG_H 
#define HAVE_CONFIG_H
#endif // !HAVE_CONFIG_H 

#include <..\pcre2\src\pcre2.h>
#include <vector>
#include <string>

namespace myodd {
  namespace regex {
    class Regex2
    {
    private:
      Regex2();
      virtual ~Regex2();

    public:
      typedef std::vector<std::wstring> Matches;

    public:
      static std::wstring Escape(const std::wstring& src );

      static bool Search(const wchar_t* rePattern, const std::wstring& wsubject, bool caseSensitive = false );
      static bool Search(const wchar_t* rePattern, const wchar_t* wsubject, bool caseSensitive = false);

      static int Match(const wchar_t *rePattern, const std::wstring& wsubject, Matches& wmatches, bool caseSensitive = false );
      static int Match(const wchar_t *rePattern, const wchar_t *wsubject, Matches& wmatches, bool caseSensitive = false);

      static int Replace(const wchar_t* rePattern, const wchar_t* replacement, const wchar_t* subject, std::wstring& replaceResult, bool caseSensitive = false );

      static int Replace(const wchar_t* rePattern, const wchar_t* replacement, std::wstring& subjectResult, bool caseSensitive = false);

    protected:
      static pcre2_code* _Compile(const wchar_t *pattern, bool caseSensitive);
      static int _Match(const wchar_t *rePattern, const wchar_t *subject, bool caseSensitive, Matches* pmatches );
    };
  }
};
