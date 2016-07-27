#include "Regex2.h"

namespace myodd {
  namespace regex {

    Regex2::Regex2()
    {
    }

    Regex2::~Regex2()
    {
    }

    /**
    * Search for a pattern in a string and replace that string with given replacement string.
    * @param const wchar_t* rePattern the pattern we are looking for.
    * @param const wchar_t* replacement the string we want to replace with.
    * @param std::wstring& subjectResult, the subject/replaced string.
    * @param bool caseSensitive if the replacement is case sensitive or not.
    * @return int the number of items replaced.
    */
    int Regex2::Replace(const wchar_t* rePattern, const wchar_t* replacement, std::wstring& subjectResult, bool caseSensitive /*= false*/ )
    {
      return Replace(rePattern, replacement, subjectResult.c_str(), subjectResult, caseSensitive);
    }

    /**
     * Search for a pattern in a string and replace that string with given replacement string.
     * @param const wchar_t* rePattern the pattern we are looking for.
     * @param const wchar_t* replacement the string we want to replace with.
     * @param const wchar_t* subject the string that contains the string we want to replace.
     * @param std::wstring& subjectResult, the replaced string.
     * @param bool caseSensitive if the replacement is case sensitive or not.
     * @return int the number of items replaced.
     */
    int Regex2::Replace(const wchar_t* rePattern, const wchar_t* replacement, const wchar_t* subject, std::wstring& replaceResult, bool caseSensitive /*= false*/ )
    {
      if (nullptr == rePattern || nullptr == replacement || nullptr == subject)
      {
        return PCRE2_ERROR_BADDATA;
      }

      int rc;

      auto compiled_re = _Compile(rePattern, caseSensitive);
      if (nullptr == compiled_re)
      {
        return  PCRE2_ERROR_BADDATA;
      }

      PCRE2_SPTR pcre2_subject = (PCRE2_SPTR)subject;
      size_t subject_length = std::char_traits<wchar_t>::length(subject);

      PCRE2_SPTR pcre2_replacement = (PCRE2_SPTR)replacement;
      size_t replacement_length = std::char_traits<wchar_t>::length(replacement);

      // assume we are replacing every single characters.
      size_t output_length = ((replacement_length + 1)*subject_length) + 1;
      PCRE2_UCHAR* output = new PCRE2_UCHAR[output_length];
      memset(output, 0, output_length * sizeof(PCRE2_UCHAR));

      rc = pcre2_substitute(compiled_re,
        pcre2_subject,
        subject_length,
        0,
        PCRE2_SUBSTITUTE_GLOBAL,
        nullptr,
        nullptr,
        pcre2_replacement,
        replacement_length,
        output,
        &output_length
      );

      if (rc < 0)
      {
        switch (rc)
        {
        case PCRE2_ERROR_NOMEMORY:
          // "Output buffer not large enough\n"; 
        case PCRE2_ERROR_NOSUBSTRING:
          // "substring not found\n"; 
        case PCRE2_ERROR_BADREPLACEMENT:
          //  "Invalid replacement string %s\n", replacement; 
        default:
          //  printf("Unknown error %d \n", rc; 
          break;
        }
        replaceResult = L"";
      }
      else
      {
        // set the value
        replaceResult = std::wstring((wchar_t*)output);
      }

      // clean up
      pcre2_code_free(compiled_re);        /* data and the compiled pattern. */
      delete[] output;

      return rc;
    }

    /**
     * Compile the patern into usable code.
     * @param const wchar_t *rePattern the pattern we want to use.
     * @param bool caseSensitive if this is case sensitive or not.
     * @return pcre2_code the compiled patern.
     */
    pcre2_code* Regex2::_Compile(const wchar_t *rePattern, bool caseSensitive)
    {
      auto pcre2_pattern = (PCRE2_SPTR)rePattern;

      pcre2_code *re;
      int errornumber;
      PCRE2_SIZE erroroffset;

      re = pcre2_compile(
        pcre2_pattern,                        /* the pattern */
        PCRE2_ZERO_TERMINATED,                /* indicates pattern is zero-terminated */
        (caseSensitive ? 0 : PCRE2_CASELESS), /* default options */
        &errornumber,                         /* for error number */
        &erroroffset,                         /* for error offset */
        nullptr);                             /* use default compile context */

      pcre2_jit_compile(re, PCRE2_JIT_COMPLETE);

      if (re == nullptr) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        return nullptr;
      }

      return re;
    }

    /**
     * Check if we have one or more items match our pattern.
     * @param const wchar_t *rePattern the pattern we are looking for.
     * @param const wchar_t *wsubject the subject containing the data we want.
     * @param bool caseSensitive if this is case sensitive or not.
     * @return bool if we match one or more items.
     */
    bool Regex2::Search(const wchar_t *rePattern, const wchar_t *wsubject, bool caseSensitive /*= false*/)
    {
      return (_Match(rePattern, wsubject, caseSensitive, (Matches*)nullptr) > 0 );
    }

    /**
     * Check if we have one or more items match our pattern.
     * @param const wchar_t *rePattern the pattern we are looking for.
     * @param const wchar_t *wsubject the subject containing the data we want.
     * @param bool caseSensitive if this is case sensitive or not.
     * @return bool if we match one or more items.
     */
    bool Regex2::Search(const wchar_t *rePattern, const std::wstring& wsubject, bool caseSensitive /*= false*/)
    {
      return (_Match(rePattern, wsubject.c_str(), caseSensitive, (Matches*)nullptr) > 0);
    }

    /**
     * Get the number of matches to a certain pattern in a subject.
     * @param const wchar_t *rePattern the pattern we are looking for.
     * @param const wchar_t *wsubject the subject containing the data we want.
     * @param Matches& matches the matches we are after.
     * @param bool caseSensitive if this is case sensitive or not.
     * @return int the number of items matched.
     */
    int Regex2::Match(const wchar_t *rePattern, const wchar_t *wsubject, Matches& matches, bool caseSensitive /*= false*/)
    {
      return _Match(rePattern, wsubject, caseSensitive, &matches );
    }

    /**
     * Get the number of matches to a certain pattern in a subject.
     * @param const wchar_t *rePattern the pattern we are looking for.
     * @param const std::wstring& wsubject the subject containing the data we want.
     * @param Matches& matches the matches we are after.
     * @param bool caseSensitive if this is case sensitive or not.
     * @return int the number of items matched.
     */
    int Regex2::Match(const wchar_t *rePattern, const std::wstring& wsubject, Matches& matches, bool caseSensitive /*= false*/)
    {
      return _Match(rePattern, wsubject.c_str(), caseSensitive, &matches );
    }

    /**
     * Private function to find matches.
     * Get the number of matches to a certain pattern in a subject.
     * @param const wchar_t *rePattern the pattern we are looking for.
     * @param const wchar_t *wsubject the subject containing the data we want.
     * @param bool caseSensitive if this is case sensitive or not.
     * @param Matches* matches the matches we are after, if nullptr we will not be adding anything to that list.
     * @return int the number of items matched.
     */
    int Regex2::_Match( const wchar_t *rePattern, const wchar_t *wsubject, bool caseSensitive, Matches* pmatches )
    {
      // clear the matches
      if (pmatches)
      {
        pmatches->clear();
      }

      // do we have a pattern or subject?
      if (nullptr == rePattern || nullptr == wsubject)
      {
        return PCRE2_ERROR_BADDATA;
      }

      // make sure that the pattern makes sense.
      size_t pattern_length = std::char_traits<wchar_t>::length(rePattern);
      if (0 == pattern_length)
      {
        // nothing matches
        return PCRE2_ERROR_NOMATCH;
      }

      // get the size of the subject
      size_t subject_length = std::char_traits<wchar_t>::length(wsubject);
      if (0 == subject_length)
      {
        // shortcut.
        return 0;
      }

      //  if we cannot compile then this is not valid.
      auto compiled_re = _Compile(rePattern, caseSensitive);
      if (nullptr == compiled_re)
      {
        return  PCRE2_ERROR_BADDATA;
      }

      const auto pcre2_subject = (PCRE2_SPTR)wsubject;

      int totalCount = 0;
      auto match_data = pcre2_match_data_create_from_pattern(compiled_re, nullptr);

      PCRE2_SIZE offset = 0;
      for (;;)
      {
        int rc = pcre2_match(
          compiled_re,
          pcre2_subject,
          subject_length,
          offset,
          PCRE2_NO_UTF_CHECK | PCRE2_NOTEMPTY_ATSTART,
          match_data,
          nullptr);

        // do we have anything?
        if (rc < 1)
        {
          break;
        }

        // if we are just looking for a count
        // then we can bail out now.
        if (nullptr == pmatches)
        {
          // get a pattern count
          pcre2_pattern_info(compiled_re, PCRE2_INFO_CAPTURECOUNT, &totalCount);

          // and add the actual subject itslef.
          ++totalCount;
          break;
        }

        auto ovector = pcre2_get_ovector_pointer(match_data);
        for (int i = 0; i < rc; i++)
        {
          auto substring_length = ovector[2 * i + 1] - ovector[2 * i];
          if ( nullptr !=  pmatches)
          {
            PCRE2_SPTR substring_start = pcre2_subject + ovector[2 * i];
            pmatches->push_back(
              std::wstring((wchar_t *)substring_start, (wchar_t *)substring_start + substring_length)
            );
          }
          offset = ovector[2 * i] + substring_length;
        }

        // we found one more.
        totalCount += rc;
      }
      pcre2_match_data_free(match_data);   /* Release memory used for the match */
      pcre2_code_free(compiled_re);        /* data and the compiled pattern. */
      return totalCount;
    }

    /**
     * Escape a string that might contain reserved regex characters.
     * @param const std::wstring& src the string we want to escape.
     * @return string the escaped string.
     */
    std::wstring Regex2::Escape(const std::wstring& src)
    {
      // the pattern
      //            . \ + * ? [ ^ ] $ ( ) { } = ! > < | : -': 
      // @see https://msdn.microsoft.com/en-us/library/4edbef7e(v=vs.110).aspx
      static const wchar_t* pattern = L"([\\.\\\\\\+\\*\\?\\[\\^\\]\\$\\(\\)\\{\\}\\=\\!\\>\\<\\|\\:\\-\\'\\:])";

      // the replacement.
      static const wchar_t* replacement = L"\\$1";
      auto result = src;
      myodd::regex::Regex2::Replace(pattern, replacement, src.c_str(), result, true );

      // return the result.
      return result;
    }
  }
};