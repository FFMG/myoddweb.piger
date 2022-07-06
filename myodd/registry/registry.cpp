#include "stdafx.h"

#include "../string/string.h"
#include "registry.h"

namespace myodd{ namespace reg{
  bool	DeleteFullPath
  ( 
    const MYODD_CHAR* lpSubKey,
    const MYODD_CHAR* section,
    HKEY hkey 
  )
  {
	  //
	  //	success or not
	  LONG ret = ERROR_SUCCESS;

	  try
	  {
		  //
		  //	Open the registery
		  HKEY hKey;
		  ret = 
		  RegCreateKey(	hkey,
						  lpSubKey, 
						  &hKey);
  	
		  //
		  //	default values, save the screen size & position
		  if (ret == ERROR_SUCCESS)
		  {
			  ret = RegDeleteValue( hKey, section );
			  //
			  //	clean
			  RegCloseKey(hKey );
		  }
	  }
	  catch(...)
    {
		  return false;
	  }
	  return (ret == ERROR_SUCCESS);
  }

  bool SaveStringFullPath
  ( 
    const MYODD_CHAR* lpSubKey,
    const MYODD_CHAR* section,
    const MYODD_CHAR* newVal,
    HKEY hkey, 
    unsigned long *dwRet
  )
  {
	  if( newVal == NULL )
	  {
		  return DeleteFullPath( lpSubKey, section, hkey );
	  }
	  //
	  //	success or not
	  LONG ret = ERROR_SUCCESS;

	  try
	  {
		  //
		  //	Open the registry
		  HKEY hKey;
		  ret = 
		  RegCreateKey(	hkey,
						  lpSubKey, 
						  &hKey);
		  //
		  //	default values, save the screen size & position
		  if (ret == ERROR_SUCCESS)
		  {
			  //	save the value
        //
			  ret = 
			  RegSetValueEx( hKey,				      // subkey handle 
				               section,						// value name 
				               0,									// must be zero 
				               REG_SZ,						// value type 
				               (LPBYTE)newVal,		// pointer to value data 
				               (unsigned long)myodd::strings::Length(newVal)     // length of value data 
                      );
			  //
			  //	clean
			  RegCloseKey(hKey );
		  }
		  if( dwRet )
      {
        *dwRet = ret;
      }
	  }
	  catch(...)
    {
		  if( dwRet ) *dwRet = ERROR_INVALID_DATA;
		  return false;
	  }
	  return (ret == ERROR_SUCCESS);
  }

  bool SaveDWORDFullPath
  ( 
    const MYODD_CHAR* lpSubKey,
    const MYODD_CHAR* section,
    unsigned long newVal,
	  HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, 
    unsigned long *dwRet
  )
  {
	  try
    {
		  //
		  //	success or not
		  LONG ret = ERROR_SUCCESS;
		  //
		  //	Open the registery
		  HKEY hKey;
		  ret = 
		  RegCreateKey(	hRootKey,
						  lpSubKey, 
						  &hKey);
		  //
		  //	default values, save the screen size & position
		  if (ret == ERROR_SUCCESS)
		  {
			  //	save the value
			  RegSetValueEx( hKey,					// subkey handle 
				  section,							// value name 
				  0,									// must be zero 
				  REG_DWORD,							// value type 
				  (LPBYTE)&newVal,					// pointer to value data 
				  4);									// length of value data 
			  //
			  //	clean
			  RegCloseKey(hKey );
		  }
      if( dwRet )
      {
        *dwRet = ret;
      }
	  }
	  catch(...)
    {
		  if( dwRet )
      {
        *dwRet = ERROR_INVALID_DATA;
      }
		  return false;
	  }
	  return true;
  }

  //
  //	load a String value with Full path
  bool	LoadStringFullPath
  ( 
    const MYODD_CHAR* lpSubKey,
    const MYODD_CHAR* section,
    std::wstring &oldVal, 
    HKEY hkey /*= HKEY_LOCAL_MACHINE*/, 
    unsigned long *dwRet
  ) 
  {
	  bool bRet = FALSE;
	  try{
		  //
		  //	success or not
		  LONG ret = ERROR_SUCCESS;
      unsigned long dwDataSize = sizeof( std::wstring );
		  //
		  //	Open the registery
		  HKEY hOpenKey;
		  ret	=	RegOpenKeyEx(hkey,
						  lpSubKey,
						  0,
						  KEY_QUERY_VALUE,
						  &hOpenKey);
		  //
		  //	try to load what was saved
		  if (ret == ERROR_SUCCESS)
		  {
        MYODD_CHAR* pszBuffer = NULL;
  			
			  // Get the size of the counter.
			  ret = RegQueryValueEx( hOpenKey,
					  section,
					  NULL, NULL, NULL,
					  &dwDataSize );

			  if( ERROR_SUCCESS == ret )
			  {
				  //
				  //	the value could be 0
				  if( dwDataSize > 0 )
				  {
					  // Allocate memory for the buffer.
					  pszBuffer = new MYODD_CHAR[ dwDataSize ];

					  ret = RegQueryValueEx( hOpenKey, 
										     section, 
										     NULL, 
										     NULL, 
										     (LPBYTE)pszBuffer,
										     &dwDataSize);
					  //
					  //	save the data
					  if ( ret == ERROR_SUCCESS )
					  {
						  bRet = true;
						  oldVal	= pszBuffer;
					  }

					  // Deallocate the memory.
					  delete [] pszBuffer;	
            pszBuffer	=	NULL;
				  }
				  else
				  {
					  // the data size is 0 so just return 0
					  bRet	  =	true;
#ifdef UNICODE
            oldVal = L"";
#else
            oldVal = "";
#endif // UNICODE
				  }
			  }
			  //
			  //	clean
			  RegCloseKey(hOpenKey );
		  }
		  if( dwRet )
      {
        *dwRet = ret;
      }
	  }
	  catch(...)
    {
		  if( dwRet )
      {
        *dwRet = ERROR_INVALID_DATA;
      }
		  return false;
	  }
	  return bRet;
  }

  bool LoadDWORDFullPath
  ( 
    const MYODD_CHAR* lpSubKey,
    const MYODD_CHAR* section,
    unsigned long &oldVal,
    HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, 
    unsigned long *dwRet
  )
  {
	  bool bRet = FALSE;
	  try
    {
		  //
		  //	success or not
		  LONG ret = ERROR_SUCCESS;
      unsigned long dwDataSize = sizeof(unsigned long);
		  //
		  //	Open the registery
		  HKEY hKey;
		  ret	=	RegOpenKeyEx( hRootKey,
						  lpSubKey,
						  0,
						  KEY_QUERY_VALUE,
						  &hKey);
		  //
		  //	try to load what was saved
		  if (ret == ERROR_SUCCESS)
		  {
        unsigned long	dwValue;
			  ret = RegQueryValueEx( hKey, 
								     section, 
								     NULL, 
								     NULL, 
								     (LPBYTE)&dwValue,
								     &dwDataSize);
			  if ( ret == ERROR_SUCCESS )
			  {
				  bRet = true;
				  oldVal	=	dwValue;
			  }
			  //
			  //	clean
			  RegCloseKey(hKey );
		  }
		  if( dwRet )
      {
        *dwRet = ret;
      }
	  }
	  catch(...)
    {
		  if( dwRet )
      {
        *dwRet = ERROR_INVALID_DATA;
      }
		  return false;
	  }
	  return bRet;
  }
} //  reg
} //  myodd