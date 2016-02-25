// amplugin.h : main header file for the amplugin DLL
//
#ifndef amplugin_h
#define amplugin_h

#pragma once

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
  size_t getCommand( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer );

  // get the full action as given by the user
  size_t getAction( DWORD nBufferLength, LPWSTR lpBuffer );

  // get the number of parameters passed.
  size_t getCommandCount() const;
  bool execute( LPCWSTR module, LPCWSTR cmdLine, bool isPrivileged);
  int getString ( DWORD nBufferLength, LPWSTR lpBuffer );
  size_t getFile   ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer) const;
  size_t getFolder ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer) const;
  size_t getURL    ( UINT idx, DWORD nBufferLength, LPWSTR lpBuffer) const;
  bool addAction( LPCWSTR szText, LPCTSTR szPath );
  bool removeAction( LPCWSTR szText, LPCTSTR szPath );
  bool findAction( UINT idx, LPCWSTR szText, DWORD nBufferLength, LPWSTR lpBuffer);

  //  add a definition
  void Add( LPCTSTR name, void* );

private:
  void* get( LPCTSTR name ) const;
  // ---------------------------------------------------------------------------------------------
  typedef std::map< std::wstring, void *> FNC_CONTAINER;
  FNC_CONTAINER m_pFunction;
};
#endif // amplugin_h