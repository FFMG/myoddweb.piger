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
  bool GetLocalNetworkInfo( STD_TSTRING& networkName, STD_TSTRING& networkIP );

  //  ping a HOST/IP address.
  bool ping( const STD_TSTRING& host, int maxAttempts = 3, std::vector<STD_TSTRING>* m_pAliases = NULL, std::vector<STD_TSTRING>* m_pAddress = NULL );
} //  net
} //  myodd
#endif // __Network_h__ 
