// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ActiveActions_h__
#define __ActiveActions_h__
#pragma once

#include <map>
#include "activeaction.h"
#include "threads/workers.h"
#include "threads/lock.h"

class ActiveActions : protected myodd::threads::Workers
{
public:
  ActiveActions();
	virtual ~ActiveActions();

  void QueueAndExecute( ActiveAction* activeAction );

  static void Execute(ActiveAction* runner, ActiveActions* parent );

protected:
  DISALLOW_COPY_AND_ASSIGN(ActiveActions);

protected:
  std::mutex _mutex;
  typedef std::map<ActiveAction*, ActiveAction*> Runners;
  Runners _runners;

  void RemoveRunner(ActiveAction* runner);
};

#endif // __ActiveActions_h__
