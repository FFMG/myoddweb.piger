#include "stdafx.h"

#pragma comment(lib, "wininet.lib" )

#include "network.h"
#include "../string/string.h"

namespace myodd{ namespace net{
  // -------------------------------------------------------------------
  LONG GetRegKey(HKEY key, const MYODD_CHAR* subkey, MYODD_CHAR* retdata)
  {
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
      long datasize = MAX_PATH;
      MYODD_CHAR data[MAX_PATH];
      RegQueryValue(hkey, NULL, data, &datasize);
      lstrcpy(retdata,data);
      RegCloseKey(hkey);
    }

    return retval;
  }

  /**
  * Go to a given URL using the user default browser/options
  * @param const MYODD_CHAR* the path we want to go to.
  * @param int how we want to display the bowser. @see http://msdn.microsoft.com/en-us/library/bb762153(VS.85).aspx
  * @return HINSTANCE if < than 32 then there was a problem.
  */
  HINSTANCE GoToURL(const MYODD_CHAR* lpUrl, int showcmd)
  {
    MYODD_CHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), lpUrl, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
    {
      if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
      {
        lstrcat(key, _T("\\shell\\open\\command"));

        if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
        {
          MYODD_CHAR *pos;
          pos = _tcsstr(key, _T("\"%1\""));
          if (pos == NULL)                      // No quotes found
          {
            pos = _tcsstr(key, _T("%1"));       // Check for %1, without quotes 
            if (pos == NULL)                    // No parameter at all...
              pos = key+lstrlen(key)-1;
            else
              *pos = '\0';                      // Remove the parameter
          }
          else
          {
            *pos = '\0';                        // Remove the parameter
          }

          lstrcat(pos, _T(" "));
          lstrcat(pos, lpUrl);

          USES_CONVERSION;
          result = (HINSTANCE) WinExec(T2A(key),showcmd);
        }
      }
    }
    return result;
  }

  /**
  * Read an entire file off the interent
  * @param const MYODD_CHAR* the URL of the file we are getting.
  * @param void* the buffer that will contain the data we are getting.
  * @param unsigned long the max number of bytes we can read
  * @param unsigned long the actual number of bytes read. 
  * @param const MYODD_CHAR* the agent to send to the site.
  * @return bool if we managed to read the file or not.
  */
  bool InternetReadWholeFile
  ( 
    const MYODD_CHAR* lpUrl,
    void* lpBuffer,
    unsigned int dwNumberOfBytesToRead, 
    unsigned int* lpNumberOfBytesRead, 
    const MYODD_CHAR* lpszAgent /*= _T("myodd getfile")*/
  )
  {
    HINTERNET hInet = InternetOpen(lpszAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    try
    {
      HINTERNET hUrl = InternetOpenUrl(hInet, 
                                       lpUrl, 
                                       NULL, 
                                       (unsigned long)-1L,
                                       (unsigned long)(INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE |INTERNET_FLAG_NO_CACHE_WRITE |WININET_API_FLAG_ASYNC), 
                                       NULL);
      if( NULL == hUrl )
      {
        throw -1; //  ensure that we close everything.
      }
      try
      {
        unsigned long dwRead=0;
        BYTE szBuffer[512];
        memset(lpBuffer,0,dwNumberOfBytesToRead);
        *lpNumberOfBytesRead = 0;
        do
        {
          if (!InternetReadFile(hUrl,szBuffer,sizeof(szBuffer),&dwRead))
            dwRead = 0;
          if (dwRead!=0)
          {
            if (*lpNumberOfBytesRead + dwRead > dwNumberOfBytesToRead) 
            {
              InternetCloseHandle( hInet );
              InternetCloseHandle( hInet );
              assert( 0 );
              return false;//too much data for the buffer
            }
            memcpy(&((char *)lpBuffer)[*lpNumberOfBytesRead],szBuffer,dwRead);
            *lpNumberOfBytesRead+=dwRead;
          }
        }while (dwRead != 0 && strchr((char *)lpBuffer,EOF)==NULL);
      }
      catch( ... )
      {
      }
      InternetCloseHandle( hUrl );
    }
    catch( ... )
    {
#ifdef _DEBUG 
      unsigned long code = ::GetLastError();
      unsigned long size_needed = 0;
      InternetGetLastResponseInfo(&code,NULL,&size_needed);
      if( size_needed > 0 )
      {
        MYODD_CHAR *message = new TCHAR[size_needed + 1];
        InternetGetLastResponseInfo(&code,message,&size_needed);
        delete [] message;
      }
#endif
    }
    InternetCloseHandle( hInet );

    return strchr((char *)lpBuffer,EOF)==NULL;
  }

  /**
   * Get the network name of the machine and the IP addresses if available.
   * @param MYODD_STRING& the name of the machine on the network.
   * @param MYODD_STRING& the IP address of the machine.
   * @return bool if we managed to get the details or not.
   */
  bool GetLocalNetworkInfo( MYODD_STRING& networkName, MYODD_STRING& networkIP )
  {
    WSADATA wsaData;
    char name[255];
    PHOSTENT hostinfo;
    WORD wVersionRequested = MAKEWORD( 2, 2 ); // version 2.2

    if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
      return false;

    if( gethostname ( name, sizeof(name)) == 0)
    { 
      USES_CONVERSION;
      networkName = T_A2T( name );
      if((hostinfo = gethostbyname(name)) != NULL)
      {
        networkIP = T_A2T( inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list) );
        WSACleanup( );
        return true;
      }
    }
    WSACleanup( );
    return false;
  }

  /**
   * Ping a host name/IP
   * @param const MYODD_STRING the host name we are looking for.
   * @param int the max number of attempts to try.
   * @param std::vector<MYODD_STRING>* a container where the aliases will be loaded, (if there are any).
   * @return bool if we were able to get some data from the given address.
   */
  bool ping
  ( 
    const MYODD_STRING& host, 
    int maxAttempts /*= 3*/,
    std::vector<MYODD_STRING>* m_pAliases /*= NULL*/,
    std::vector<MYODD_STRING>* m_pAddress /*= NULL*/
  )
  {
    //  cleanup if needed.
    if( m_pAliases )
    {
      m_pAliases->erase( m_pAliases->begin(), m_pAliases->end() );
    }

    if( m_pAddress )
    {
      m_pAddress->erase( m_pAddress->begin(), m_pAddress->end() );
    }
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD( 2, 2 ); // version 2.2
    if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
      return false;

    USES_CONVERSION;
    LPCSTR lpaHost = T_T2A( host.c_str() );
    const hostent* host_info = 0 ;
    for( int attempt=0 ; (host_info==0) && (attempt<maxAttempts) ; ++attempt )
      host_info = gethostbyname(lpaHost) ;

    if(host_info)
    {
      //  std::cout << "host: " << host_info->h_name << '\n' ;
      if( m_pAliases )
      {
        for( int i=0 ; host_info->h_aliases[i] ; ++i )
        {
          m_pAliases->push_back( T_A2T( host_info->h_aliases[i] ));
        }
      }

      if( m_pAddress )
      {
        for( int i=0 ; host_info->h_addr_list[i] ; ++i )
        {
          const in_addr* address = (in_addr*)host_info->h_addr_list[i] ;
          m_pAddress->push_back( T_A2T(inet_ntoa( *address ) ));
        }
      }
    }
    else 
    {
      WSACleanup( );
      return false;
    }
    WSACleanup( );
    return true;
  }
} //  network
} //  myodd