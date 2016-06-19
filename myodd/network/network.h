#ifndef __Network_h__
#define __Network_h__

/**
 * Do all the internet/network calls.
 * For example GoToURL and so on.
 */
#pragma once

#include "../common/includes.h"
#include "wininet.h"
#include <vector>

namespace myodd{ namespace net{
  // go to a site, launch the user browser.
  HINSTANCE GoToURL(LPCTSTR lpUrl, int showcmd);
  
  // read an internet file
  bool InternetReadWholeFile( LPCTSTR lpUrl, LPVOID lpBuffer, unsigned int dwNumberOfBytesToRead, unsigned int* lpNumberOfBytesRead, LPCTSTR lpszAgent );

  //  get information about ourselves.
  bool GetLocalNetworkInfo( MYODD_STRING& networkName, MYODD_STRING& networkIP );

  //  ping a HOST/IP address.
  bool ping( const MYODD_STRING& host, int maxAttempts = 3, std::vector<MYODD_STRING>* m_pAliases = NULL, std::vector<MYODD_STRING>* m_pAddress = NULL );
} //  net
} //  myodd
#endif // __Network_h__ 
