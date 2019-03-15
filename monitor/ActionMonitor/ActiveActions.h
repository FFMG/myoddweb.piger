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
#pragma once

#include <vector>
#include <threads/lock.h>
#include "IActiveaction.h"
#include "threads/workers.h"

class ActiveActions : protected myodd::threads::Workers
{
public:
  ActiveActions();
	virtual ~ActiveActions();

  ActiveActions(const ActiveActions&) = delete;
  void operator=(const ActiveActions&) = delete;

  void QueueAndExecute(IActiveAction* activeAction );

private:
  static void Execute(IActiveAction* runner, ActiveActions* parent);

  /**
   * \brief the mutex that manages the runners
   */
  myodd::threads::Key _mutexRunner;

  /**
   * \brief the collection of runners.
   */
  typedef std::vector<IActiveAction*> Runners;
  Runners _runners;

  /**
   * \brief look for and remove a runner.
   */
  void RemoveRunner(IActiveAction* runner);
};
