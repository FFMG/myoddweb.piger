//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#include "stdafx.h"
#include "ActiveActions.h"

/**
 * \brief Constructor.
 */
ActiveActions::ActiveActions() : _mutexRunner( L"Active Action - Runner")
{
}

/**
 * \brief Destructor
 */
ActiveActions::~ActiveActions()
{
  // we have to wait for all the workers now
  // we must do it in this destructor
  // otherwise our own map will be destroyed.
  WaitForAllWorkers();

  // the map should now be empty
  // otherwise we will simply delete the items.
  myodd::threads::Lock guard(_mutexRunner);
  for ( auto it = _runners.begin(); it != _runners.end(); ++it)
  {
    delete *it;
  }
  _runners.clear();
}

/**
 * \brief Add an item to our list of workers and queue.
 * \param activeAction the action we want to queue, this can be NULL
 */
void ActiveActions::QueueAndExecute( ActiveAction* activeAction )
{
  //  it could be NULL.
  if (nullptr == activeAction)
  {
    return;
  }

  // add this item to our list.
  // make sure that we keep the lock for as little as posible..
  myodd::threads::Lock guard(_mutexRunner );

  const auto it = std::find(_runners.begin(), _runners.end(), activeAction);
  if (it == _runners.end())
  {
    // add it to the list
    _runners.push_back(activeAction);
  }

  //  release the lock
  guard.Release();

  // start the thread.
  QueueWorker(&ActiveActions::Execute, activeAction, this );
}

/**
 * \brief Remove an active action form the list of actions.
 * \param runner the runner we would like to remove from the list.
 */
void ActiveActions::RemoveRunner( ActiveAction* runner )
{
  //  lock it.
  myodd::threads::Lock guard(_mutexRunner);

  // find it
  const auto it = std::find( _runners.begin(), _runners.end(), runner);
  if (it == _runners.end())
  {
    //  does not exist?
    return;
  }

  // de-initialize it
  runner->DeInitialize();

  // delete it
  delete *it;

  // remove it
  _runners.erase(it);
}

/**
 * \brief Execute an active action
 * \param runner the action we will be running.
 * \param parent the parent actions holder so we can clean up once complete.
 */
void ActiveActions::Execute( ActiveAction* runner, ActiveActions* parent )
{
  //  do the action.
  (*runner).ExecuteInThread();

  // and remove the runner.
  parent->RemoveRunner(runner);

  // give other threads a chance to run.
  std::this_thread::yield();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}