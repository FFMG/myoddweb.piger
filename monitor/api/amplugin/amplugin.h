// amplugin.h : main header file for the amplugin DLL
//
#ifndef amplugin_h
#define amplugin_h

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "am_plugins.h"
#include <string>
#include <map>

// CampluginApp
// See amplugin.cpp for the implementation of this class
class amplugin
{
public:
  amplugin();
  ~amplugin();

public:
  // the various functions
  double version ();
  bool say( LPCWSTR msg, UINT nElapse, UINT nFadeOut);
  int getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );

  // get the full action as given by the user
  int getAction( DWORD nBufferLength, LPWSTR lpBuffer );

  // get the number of parameters passed.
  int getCommandCount();
  bool execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  int getString ( DWORD nBufferLength, LPWSTR lpBuffer );
  int getFile   ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  int getFolder ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  int getURL    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer);
  bool addAction( LPCWSTR szText, LPCTSTR szPath );
  bool removeAction( LPCWSTR szText, LPCTSTR szPath );
  bool findAction( UINT idx, LPCWSTR szText, DWORD nBufferLength, LPWSTR lpBuffer);

  //  add a definition
  void Add( LPCTSTR name, void* );

private:
  void* get( LPCTSTR name );
  // ---------------------------------------------------------------------------------------------
  typedef std::map< std::wstring, void *> FNC_CONTAINER;
  FNC_CONTAINER m_pFunction;
};
#endif // amplugin_h