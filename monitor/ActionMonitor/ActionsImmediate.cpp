#include "stdafx.h"
#include "ActionsImmediate.h"
#include "ActionMonitor.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

/**
 * todo
 * @param void
 * @return void
 */
ActionsImmediate::ActionsImmediate(LPCTSTR subDir ) : 
  Actions(),
  m_subDir( subDir )
{

}

/**
 * todo
 * @param void
 * @return void
 */
ActionsImmediate::~ActionsImmediate(void)
{
}

/**
 * todo
 * @param void
 * @return void
 */
void ActionsImmediate::Init()
{
  STD_TSTRING sPath = myodd::config::get( _T("paths\\commands") );
  if( myodd::files::ExpandEnvironment( sPath, sPath ) )
  {
    ParseDirectory( sPath.c_str(), m_subDir.c_str() );
  }

  //  we must now do all the actions
  DoThem( false );
}

/**
 * todo
 * @param bool isPrivileged if we need administrator privilege to run this.
 * @return void
 */
void ActionsImmediate::DoThem(  bool isPrivileged )
{
  for( array_of_actions::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it )
  {
    // we must be carefull that we replace all the values properly
    // recover properly even in a crash so we can reset the posible Action
    try
    {
      // The action.
      Action& action = *it;

      // We must tell the APIs who the current action will be
      // this one is because some APIs need to know the name of the current command
      // in order to get the path of the command.
      //
      // we cannot replace 'posibleActions' with 'this' as the APIs might
      // actually add or remove commands.
      posibleActions->SetAction( &action );

      // do the action, we don't have any arguments to pass to the action
      // so we bypass the 'DoIt(...)' function
      // and go directly to 'Launch(...)'
      action.DoItDirect( _T( "" ), isPrivileged );
    }
    catch( ... )
    {
      _ASSERT(0);
    }// catch
  }// for loop

  // reset the current action to the default.
  posibleActions->SetAction( NULL );
}