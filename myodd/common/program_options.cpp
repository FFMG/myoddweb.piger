#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "program_options.h"
#include "string.h"

namespace myodd {
  variables::variables
  ( 
    int argc, 
    MYODD_CHAR* args[],
    const desc& d,
    bool allow_unregistered /*= true*/
  )
  {
# ifdef _UNICODE
    myodd::po::wparsed_options* parsed = NULL;
    if( allow_unregistered )
      parsed = new myodd::po::wparsed_options( myodd::po::wcommand_line_parser(argc, args).options(d).allow_unregistered().run() );
    else
      parsed = new myodd::po::wparsed_options( myodd::po::wcommand_line_parser(argc, args).options(d).run() );
# else
    myodd::po::parsed_options* parsed = NULL;
    if( allow_unregistered )
      parsed = new myodd::po::parsed_options( myodd::po::command_line_parser(argc, args).options(d).allow_unregistered().run() );
    else
      parsed = new myodd::po::parsed_options( myodd::po::command_line_parser(argc, args).options(d).run() );
# endif
    myodd::po::store( *parsed, *this);
    myodd::po::notify( *this );

    delete parsed;
  }
}