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
#include "xml/tinyxml2.h"

// config files.
#include "config/config.h"

// offset files.
#include "offsets/offset.h"

// the registry files.
#include "registry/registry.h"

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
#ifdef _DEBUG
#  ifdef _WIN64
#    pragma comment(lib, "myodd6414d.lib" )
#  else
#    pragma comment(lib, "myodd14d.lib" )
#  endif
#else
#  ifdef _WIN64
#    pragma comment(lib, "myodd6414.lib" )
#  else
#    pragma comment(lib, "myodd14.lib" )
#  endif
#endif

#endif // __myoddinclude_h__