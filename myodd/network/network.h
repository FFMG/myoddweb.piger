#ifndef __Network_h__
#define __Network_h__

/**
 * Do all the internet/network calls.
 * For example GoToURL and so on.
 */
#pragma once

#include "../common/includes.h"
#include <windows.h>
#include "wininet.h"
#include <vector>

namespace myodd{ namespace net{
  // go to a site, launch the user browser.
  HINSTANCE GoToURL(const wchar_t* lpUrl, int showcmd);
  
  // read an internet file
  bool InternetReadWholeFile(const wchar_t* lpUrl, void* lpBuffer, unsigned int dwNumberOfBytesToRead, unsigned int* lpNumberOfBytesRead, LPCTSTR lpszAgent );

  //  get information about ourselves.
  bool GetLocalNetworkInfo( std::wstring& networkName, std::wstring& networkIP );

  //  ping a HOST/IP address.
  bool ping( const std::wstring& host, int maxAttempts = 3, std::vector<std::wstring>* m_pAliases = NULL, std::vector<std::wstring>* m_pAddress = NULL );
} //  net
} //  myodd
#endif // __Network_h__ 
