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
  bool say(const wchar_t* msg, unsigned int nElapse, unsigned int nFadeOut);
  size_t getCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer );

  // get the full action as given by the user
  size_t getAction(unsigned int nBufferLength, wchar_t* lpBuffer );

  // get the number of parameters passed.
  size_t getCommandCount() const;
  bool execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged);
  int getString (unsigned int nBufferLength, wchar_t* lpBuffer );
  size_t getFile   (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  size_t getFolder (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  size_t getURL    (unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  bool addAction(const wchar_t* szText, const wchar_t* szPath );
  bool removeAction(const wchar_t* szText, const wchar_t* szPath );
  bool findAction(unsigned int idx, const wchar_t* szText, unsigned int nBufferLength, wchar_t* lpBuffer);

  //  add a definition
  void Add(const wchar_t* name, void* );

private:
  void* get(const wchar_t* name ) const;
  // ---------------------------------------------------------------------------------------------
  typedef std::map< std::wstring, void *> FNC_CONTAINER;
  FNC_CONTAINER m_pFunction;
};
#endif // amplugin_h