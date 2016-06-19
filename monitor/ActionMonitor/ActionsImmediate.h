#pragma once

#include "Actions.h"
#include "ActiveActions.h"

class ActionsImmediate :
  public Actions, ActiveActions
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
  MYODD_STRING m_subDir;
  void DoThem();
};
