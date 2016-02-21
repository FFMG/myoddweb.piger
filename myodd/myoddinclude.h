#ifndef __myoddinclude_h__
#define __myoddinclude_h__

#pragma once

//
// add all the needed files.
//
// common files.

// strings
#include "string/string.h"

// files
#include "files/files.h"

// math
#include "math/math.h"

// wnd
#include "wnd/wnd.h"

// geo
#include "geo/geo.h"

// network
#include "network/network.h"

// log
#include "log/log.h"

// XML files.
#include "xml/tinyxml.h"

// config files.
#include "config/config.h"

// offset files.
#include "offsets/offset.h"

// the registry files.
#include "registry/registry.h"

// containers
#include "containers/idx_map.h"
#include "containers/Matrix2D.h"

// sqlite
#include "sqlite/sqlite.h"

// html
#include "html/html.h"

// program options.
#include "common/program_options.h"

#include "stree/smap.h"
#include "stree/sset.h"
#include "stree/stree.h"

// threads
#include "threads/threads.h"

// and add the needed lib
#if !defined(_MT)
# error "You must have 'Multithreaded DLL' or 'Multithreaded' defined"
#endif 

#if _MSC_VER == 1900
#  if !defined(_DLL)
#    ifdef _DEBUG
#      pragma comment(lib, "myoddMT14d.lib" )
#    else
#      pragma comment(lib, "myoddMT14.lib" )
#    endif
#  else
#    ifdef _DEBUG
#       pragma comment(lib, "myodd14d.lib" )
#     else
#       pragma comment(lib, "myodd14.lib" )
#     endif
#   endif
#else
#   error "Unknown compilier version defined!"
#endif

#endif // __myoddinclude_h__