#pragma once

#ifndef __Program_Options_h__
#define __Program_Options_h__

#include "includes.h"

#define BOOST_ALL_NO_LIB 1
#include <boost/program_options.hpp>

namespace myodd {
  // ----
  namespace po = boost::program_options;

  // ----
  class desc : public boost::program_options::options_description
  {
  public:
    desc(){}
  };

  // ----
  class variables : public boost::program_options::variables_map
  {
  public:
    variables( int argc, MYODD_CHAR* args[], const desc& d, bool allow_unregistered = true );
  };
} //  myodd

#endif // __Program_Options_h__