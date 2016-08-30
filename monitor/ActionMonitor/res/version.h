#include <tchar.h>

#define VERSION_MAJOR          0
#define VERSION_MINOR          8
#define VERSION_MAINTENANCE    1
#define VERSION_BUILD         62

#define VERSION_HOUR           6
#define VERSION_MINUTE        52

#define VERSION_YEAR        2016
#define VERSION_MONTH          8
#define VERSION_DAY           30

//
// No need to edit below...
//

#define _STR(x) #x
#define STR(x) _STR(x)

#define VERSION_BUILD_DATE  _T(STR(VERSION_YEAR)) \
                            _T("\\") \
                            _T(STR(VERSION_MONTH)) \
                            _T("\\") \
                            _T(STR(VERSION_DAY))

#define VERSION_BUILD_TIME  _T(STR(VERSION_HOUR)) \
                            _T(":") \
                            _T(STR(VERSION_MINUTE)) \
                            _T(":00UTC")

#define VERSION_NUMBER VERSION_MAJOR, VERSION_MINOR, VERSION_MAINTENANCE, VERSION_BUILD
#define VERSION_STRING _T( STR(VERSION_MAJOR)) \
                       _T(".") \
                       _T(STR(VERSION_MINOR)) \
                       _T(".") \
                       _T(STR(VERSION_MAINTENANCE)) \
                       _T(".") \
                       _T(STR(VERSION_BUILD))

#define VERSION_COMPANY _T("MyOddweb.com.  All rights reserved.")
#define VERSION_COPYRIGHT _T("Copyright (C) MyOddweb.com. 2008-") \
                          _T(STR(VERSION_YEAR))
#define VERSION_TRADEMARK _T("")
#define VERSION_BUILD_DATE_TIME VERSION_BUILD_DATE \
                                _T(" - ") \
                                VERSION_BUILD_TIME
#define VERSION_DESCRIPTION _T( "Piger keyboard Launcher." );

#define VERSION_PRODUCTNAME _T("ActionMonitor ")          \
                            _T( STR(VERSION_MAJOR))       \
                            _T(".")                       \
                            _T( STR(VERSION_MINOR))       \
                            _T(".")                       \
                            _T(STR(VERSION_MAINTENANCE))
                            
#define VERSION_SPECIALBUILD _T("Alpha Version - ")       \
                             _T( STR(VERSION_MAJOR))      \
                             _T(".")                      \
                             _T( STR(VERSION_MINOR))      \
                             _T(".")                      \
                             _T( STR(VERSION_MAINTENANCE))
