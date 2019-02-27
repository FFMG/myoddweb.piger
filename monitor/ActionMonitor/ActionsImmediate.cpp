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
#include "ActionsImmediate.h"
#include "ActionMonitor.h"
#include "ActionMonitorDlg.h"

/**
 * \brief Constructor
 * \param directoryToParse the directory with the actions we want to do immidiately.
 * \param parentActions the parent actions.
 */
ActionsImmediate::ActionsImmediate(const std::wstring& directoryToParse, IActions& parentActions) :
  Actions(),
  _directoryToParse( directoryToParse ),
  _parentActions( parentActions ),
  _mutex( L"Actions - Immediate")
{

}

/**
 * \brief Initialise all the actions so we can start doing them.
 *        Go around the directory and look for possible actions.
 */
void ActionsImmediate::Initialize()
{
  std::wstring sPath = ::myodd::config::Get( L"paths\\commands", L"");
  if( myodd::files::ExpandEnvironment( sPath, sPath ) )
  {
    ParseDirectory( sPath.c_str(), _directoryToParse.c_str() );
  }

  //  we must now do all the actions
  DoThem( );
}

/**
 * \brief Do all the action right away.
 */
void ActionsImmediate::DoThem(  )
{
  myodd::threads::Lock guard(_mutex);
  const auto isPrivileged = true;
  for( auto it = _actions.begin(); it != _actions.end(); ++it )
  {
    // we must be carefull that we replace all the values properly
    // recover properly even in a crash so we can reset the posible Action
    try
    {
      // The action.
      const auto& action = *it;

      // We must tell the APIs who the current action will be
      // this one is because some APIs need to know the name of the current command
      // in order to get the path of the command.
      //
      // we cannot replace 'possibleActions' with 'this' as the APIs might
      // actually add or remove commands.
      _parentActions.SetAction( action );

      // do the action, we don't have any arguments to pass to the action
      // so we bypass the 'CreateActiveAction(...)' function
      // and go directly to 'Launch(...)'
      QueueAndExecute( action->CreateActiveActionDirect( nullptr, L"" , isPrivileged ) );
    }
    catch( ... )
    {
      _ASSERT(0);
    }// catch
  }// for loop

  // reset the current action to the default.
  _parentActions.SetAction( nullptr );
}

/**
 * \brief wait for all currently running actions to complete.
 */
void ActionsImmediate::WaitForAll()
{
  // wait for all the workers to finish.
  WaitForAllWorkers( &CActionMonitorApp::MessagePump );
}

