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
    int Regex2::Replace(const wchar_t* rePattern,
      const wchar_t* replacement,
      std::wstring& subjectResult,
      bool caseSensitive /*= false*/
    ) const
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
    int Regex2::Replace(const wchar_t* rePattern,
      const wchar_t* replacement,
      const wchar_t* subject,
      std::wstring& replaceResult,
      bool caseSensitive /*= false*/
    ) const
    {
      if (nullptr == rePattern || nullptr == replacement || nullptr == subject)
      {
        return PCRE2_ERROR_BADDATA;
      }

      int rc;

      pcre2_code *compiled_re = u8compile(rePattern, caseSensitive);
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
    pcre2_code *Regex2::u8compile(const wchar_t *rePattern, bool caseSensitive) const
    {
      PCRE2_SPTR pcre2_pattern = (PCRE2_SPTR)rePattern;

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
    * Get the number of matches to a certain pattern in a subject.
    * @param const wchar_t *rePattern the pattern we are looking for.
    * @param const wchar_t *wsubject the subject containing the data we want.
    * @param bool caseSensitive if this is case sensitive or not.
    * @return int the number of items matched.
    */
    int Regex2::Match(const wchar_t *rePattern, const wchar_t *wsubject, bool caseSensitive /*= false*/) const
    {
      return Match(rePattern, wsubject, (matches*)nullptr, caseSensitive);
    }

    /**
    * Get the number of matches to a certain pattern in a subject.
    * @param const wchar_t *rePattern the pattern we are looking for.
    * @param const wchar_t *wsubject the subject containing the data we want.
    * @param matches& matches the matches we are after.
    * @param bool caseSensitive if this is case sensitive or not.
    * @return int the number of items matched.
    */
    int Regex2::Match(const wchar_t *rePattern, const wchar_t *wsubject, matches& matches, bool caseSensitive /*= false*/) const
    {
      return Match(rePattern, wsubject, &matches, caseSensitive);
    }

    /**
    * Private function to find matches.
    * Get the number of matches to a certain pattern in a subject.
    * @param const wchar_t *rePattern the pattern we are looking for.
    * @param const wchar_t *wsubject the subject containing the data we want.
    * @param matches* matches the matches we are after, if nullptr we will not be adding anything to that list.
    * @param bool caseSensitive if this is case sensitive or not.
    * @return int the number of items matched.
    */
    int Regex2::Match
    (
      const wchar_t *rePattern,
      const wchar_t *wsubject,
      matches* pmatches,
      bool caseSensitive /*= false*/
    ) const
    {
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
      pcre2_code *compiled_re = u8compile(rePattern, caseSensitive);
      if (nullptr == compiled_re)
      {
        return  PCRE2_ERROR_BADDATA;
      }

      PCRE2_SIZE *ovector;
      PCRE2_SPTR pcre2_subject = (PCRE2_SPTR)wsubject;

      int totalCount = 0;
      pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(compiled_re, nullptr);
      size_t offset = 0;
      for (;;)
      {
        int rc = pcre2_match(
          compiled_re,
          pcre2_subject,
          subject_length,
          offset,
          0,
          match_data,
          nullptr);

        if (rc < 1)
        {
          break;
        }

        ovector = pcre2_get_ovector_pointer(match_data);

        for (int i = 0; i < rc; i++)
        {
          size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];

          if (pmatches != nullptr)
          {
            PCRE2_SPTR substring_start = pcre2_subject + ovector[2 * i];
            pmatches->push_back(
              std::wstring((wchar_t *)substring_start, (wchar_t *)substring_start + substring_length)
            );
          }
          offset = ovector[2 * i] + substring_length;

          // we found one more.
          ++totalCount;
        }

      }

      pcre2_match_data_free(match_data);   /* Release memory used for the match */
      pcre2_code_free(compiled_re);        /* data and the compiled pattern. */

      return totalCount;
    }
  }
};