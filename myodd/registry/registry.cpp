#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "registry.h"

namespace myodd{ namespace reg{
  bool	DeleteFullPath
  ( 
    LPCTSTR lpSubKey, 
    LPCTSTR section, 
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
    LPCTSTR lpSubKey, 
    LPCTSTR section, 
    LPCTSTR newVal,
    HKEY hkey, 
    DWORD *dwRet
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
				               (DWORD)_tcslen(newVal)     // length of value data 
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
    LPCTSTR lpSubKey, 
	  LPCTSTR section, 
	  DWORD newVal, 
	  HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, 
	  DWORD *dwRet
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
    LPCTSTR lpSubKey, 
    LPCTSTR section, 
    STD_TSTRING &oldVal, 
    HKEY hkey /*= HKEY_LOCAL_MACHINE*/, 
    DWORD *dwRet 
  ) 
  {
	  bool bRet = FALSE;
	  try{
		  //
		  //	success or not
		  LONG ret = ERROR_SUCCESS;
		  DWORD dwDataSize = sizeof( STD_TSTRING );
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
			  TCHAR*  pszBuffer = NULL;
  			
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
					  pszBuffer = new TCHAR[ dwDataSize ];

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
					  oldVal	=	_T("");
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
    LPCTSTR lpSubKey, 
    LPCTSTR section, 
    DWORD &oldVal, 
    HKEY hRootKey /*= HKEY_LOCAL_MACHINE*/, 
    DWORD *dwRet
  )
  {
	  bool bRet = FALSE;
	  try
    {
		  //
		  //	success or not
		  LONG ret = ERROR_SUCCESS;
		  DWORD dwDataSize = sizeof( DWORD );
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
			  DWORD	dwValue;
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