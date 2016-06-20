#pragma once
#include "amplugin.h"
#include <map>

class AmPluginPrivate :
  public AmPlugin
{
public:
  AmPluginPrivate();
  virtual ~AmPluginPrivate();

public:
  // the various functions
  virtual double Version();
  virtual bool Say(const wchar_t* msg, unsigned int nElapse, unsigned int nFadeOut);
  virtual size_t GetCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer);

  // get the full action as given by the user
  virtual size_t GetAction(unsigned int nBufferLength, wchar_t* lpBuffer);

  // get the number of parameters passed.
  virtual size_t GetCommandCount() const;
  virtual bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged);
  virtual int GetString(unsigned int nBufferLength, wchar_t* lpBuffer);
  virtual size_t GetFile(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  virtual size_t GetFolder(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  virtual size_t GetURL(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const;
  virtual bool AddAction(const wchar_t* szText, const wchar_t* szPath);
  virtual bool RemoveAction(const wchar_t* szText, const wchar_t* szPath);
  virtual bool FindAction(unsigned int idx, const wchar_t* szText, unsigned int nBufferLength, wchar_t* lpBuffer);
  virtual void* GetForegroundWindow() const;
  virtual void Log(unsigned int logType, const wchar_t* szText);

  //  add a definition
  virtual void Add(const wchar_t* name, void*);

private:

  void* Get(const wchar_t* name) const;
  // ---------------------------------------------------------------------------------------------
  typedef std::map< std::wstring, void *> FNC_CONTAINER;
  FNC_CONTAINER m_pFunction;
};

