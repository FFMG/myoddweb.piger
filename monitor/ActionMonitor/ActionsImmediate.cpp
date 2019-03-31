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

/**
 * \brief Constructor
 * \param application the application manager
 * \param directoryToParse the directory with the actions we want to do immidiately.
 * \param parentActions the parent actions.
 */
ActionsImmediate::ActionsImmediate(IApplication& application, const std::wstring& directoryToParse, IActions& parentActions, IVirtualMachines& virtualMachines) :
  Actions(application),
  _directoryToParse( directoryToParse ),
  _parentActions( parentActions ),
  _mutex( L"Actions - Immediate"),
  _virtualMachines( virtualMachines )
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
      const auto activeAction = action->CreateActiveActionDirect(_virtualMachines, nullptr, L"", isPrivileged);
      if( _application.ExecuteActiveAction( activeAction ) )
      {
        _runningActions.push_back(activeAction);
      }
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
  const auto sleepForMilliseconds = std::chrono::milliseconds(10);
  for(;;)
  {
    // lock us in while we are getting info on the running action
    myodd::threads::Lock guard(_mutex);

    // get the first running action
    const auto it = _runningActions.begin();
    if( it == _runningActions.end() )
    {
      // we are done.
      break;
    }

    // is it still running?
    if( !_application.IsActiveActionRunning( *it ) )
    {
      // no, it is not, so we can move on.
      _runningActions.erase(it);
      continue;
    }

    // we are done with the action
    guard.Release();

    // so wait a little for messages to progress.
    myodd::wnd::MessagePump(nullptr);
    std::this_thread::yield();
    std::this_thread::sleep_for(sleepForMilliseconds);
  }
}

