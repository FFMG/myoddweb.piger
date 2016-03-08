#include <tchar.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 5
#define VERSION_MAINTENANCE 5
#define VERSION_BUILD 2
#define VERSION_BUILD_DATE  _T("08/03/2016")
#define VERSION_BUILD_TIME  _T("19:10:26UTC")

//
// No need to edit below... (or maybe the year...)
//

#define _STR(x) #x
#define STR(x) _STR(x)

#define VERSION_NUMBER VERSION_MAJOR, VERSION_MINOR, VERSION_MAINTENANCE, VERSION_BUILD
#define VERSION_STRING _T( STR(VERSION_MAJOR)) \
                       _T(".") \
                       _T(STR(VERSION_MINOR)) \
                       _T(".") \
                       _T(STR(VERSION_MAINTENANCE)) \
                       _T(".") \
                       _T(STR(VERSION_BUILD))

#define VERSION_COMPANY _T("MyOddweb.com.  All rights reserved.")
#define VERSION_COPYRIGHT _T("Copyright (C) MyOddweb.com. 2008-2016" )
#define VERSION_TRADEMARK _T("")
#define VERSION_BUILD_DATE_TIME VERSION_BUILD_DATE _T( " - " ) \
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
