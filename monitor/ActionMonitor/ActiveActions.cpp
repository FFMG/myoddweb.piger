// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActiveActions.h"

/**
 * todo
 * @param void
 * @return void
 */
ActiveActions::ActiveActions()
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActiveActions::~ActiveActions()
{
  // we have to wait for all the workers now
  // we must do it in this destructor
  // otherwise our own map will be destroyed.
  WaitForAllWorkers();

  // the map should now be empty
  // otherwise we will simply delete the items.
  {
    myodd::threads::Lock guard(_mutex);
    for (Runners::iterator it = _runners.begin(); it != _runners.end(); ++it)
    {
      delete it->second;
    }
    _runners.clear();
  }
}

/**
 * Add an item to our list of workers and queue.
 * @param ActiveAction* activeAction the action we want to queue, this can be NULL
 */
void ActiveActions::QueueAndExecute( ActiveAction* activeAction )
{
  //  it could be NULL.
  if (NULL == activeAction)
  {
    return;
  }

  // add this item to our list.
  myodd::threads::Lock guard(_mutex);

  // add it to the list
  _runners[activeAction] = activeAction;

  // start the thread.
  QueueWorker(&ActiveActions::Execute, activeAction, this );
}

/**
 * Remove an active action form the list of actions.
 * @param ActiveAction* runner the runner we would like to remove from the list.
 */
void ActiveActions::RemoveRunner( ActiveAction* runner )
{
  //  lock it.
  myodd::threads::Lock guard(_mutex);

  // find it
  Runners::iterator it = _runners.find(runner);
  if (it == _runners.end())
  {
    //  does not exist?
    return;
  }

  // de-initialize it
  runner->DeInitialize();

  // delete it
  delete it->second;

  // remove it
  _runners.erase(it);
}

/**
 * Execute an active action
 * @param ActiveAction* runner the action we will be running.
 * @param ActiveActions* parent the parent actions holder so we can clean up once complete.
 */
void ActiveActions::Execute( ActiveAction* runner, ActiveActions* parent )
{
  //  do the action.
  (*runner).ExecuteInThread();

  // and remove the runner.
  parent->RemoveRunner(runner);
}