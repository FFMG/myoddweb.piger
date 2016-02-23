#pragma once

#include "Actions.h"

class ActionsImmediate :
  public Actions
{
public:
  ActionsImmediate( LPCTSTR subDir );
  virtual ~ActionsImmediate(void);

public:
  virtual void Init();

protected:
  //  no reserved directories here.
  virtual bool IsReservedDir( LPCTSTR )const{ return false;}

protected:
  STD_TSTRING m_subDir;
  void DoThem(bool isPrivileged);
};
