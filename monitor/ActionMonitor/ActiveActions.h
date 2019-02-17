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

#include <map>
#include "activeaction.h"
#include "threads/workers.h"

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
