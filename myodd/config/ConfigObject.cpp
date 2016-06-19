#include "stdafx.h"

#include "ConfigObject.h"

namespace myodd{ namespace config{

/**
 * Constructor.
 * @param none
 * @return none
 */ 
ConfigObject::ConfigObject()
{
  _Construct( );
}

/**
 * Copy Constructor.
 * @param none
 * @return none
 */ 
ConfigObject::ConfigObject( const ConfigObject& c)
{
  _Construct( );
  _Copy( c );
}

/**
 * Constructor.
 *
 * @param const MYODD_STRING& the name of the value we are setting.
 * @return none
 */ 
ConfigObject::ConfigObject( const MYODD_STRING& stdVarName )
{
  _Construct( );
  SetObjectName( stdVarName );
}

/**
 * Constructor with a value.
 *
 * @param const MYODD_STRING& the name of the value we are setting.
 * @param const Data& the value we are setting.
 * @return none
 */ 
ConfigObject::ConfigObject( const MYODD_STRING& stdVarName, const Data& cod )
{
  _Construct( );
  SetObjectName( stdVarName );
  m_codData = cod;
}

/**
 * copy constructor.
 * @param const Object& the value we are copying
 * @return const Object& this
 */ 
const ConfigObject& ConfigObject::operator=( const ConfigObject& c )
{
  _Copy( c );
  return *this;
}

void ConfigObject::_Copy( const ConfigObject&c )
{
  if( this == &c )
  {
    return; //  copy onto ourselves.
  }

  // the notifications.
  __super::_Copy( *this );

  // the local values.
  SetObjectName( c.GetObjectName() );
  m_codData = c.m_codData;
  m_bIsSet  = c.m_bIsSet;
}

/**
 * Set the values, used by the various constructors.
 *
 * @param none
 * @return none
 */ 
void ConfigObject::_Construct()
{
  m_unicodeName = _T("");
  m_bIsSet = false;
}

/**
 * The destructor.
 * @param none
 * @return none
 */ 
ConfigObject::~ConfigObject()
{
}

/**
 * Set the name of the Config Object
 * @param const MYODD_STRING& the new name of the object.
 * @return none
 */ 
void ConfigObject::SetObjectName( const MYODD_STRING& s )
{
  m_unicodeName = s;
}

/**
 * Add a function we want to call when something changes in this config object.
 * @param CONFIG_NOTIFY the function we want to call.
 * @param MYODD_LPARAM a param we are passing to this function.
 * @return bool if the monitor was added or not.
 */ 
bool ConfigObject::AddMonitor
( 
  const CONFIG_NOTIFY& notif, 
  MYODD_LPARAM lParam
)
{
  //
  // ok, we can add this item to the list.
  // if it already exists then we only add the MYODD_LPARAM
  return AddNotif( notif, lParam );
}

/**
 * Remove a monitored item given the notification and the lParam
 *
 * @param CONFIG_NOTIFY the function we want to call.
 * @param MYODD_LPARAM a param we are passing to this function.
 * @return bool if we removed something or not.
 */
bool ConfigObject::RemoveMonitor( const CONFIG_NOTIFY& notif, MYODD_LPARAM lParam )
{
  // the notification will do its own locking
  return RemoveNotif( notif, lParam );
}

/**
 * Remove all notification monitors.
 * @param none
 * @return none
 */
void ConfigObject::RemoveAllNotifs()
{
  __super::RemoveAllNotifs();
}

/**
 * Call all the functions that are monitoring changes in this config object.
 * And notify the type that was changed.
 * @param Data::config_type the type that was updated.
 * @return none
 */ 
void ConfigObject::Notify( Data::config_type type )
{
  const MYODD_STRING& name = GetObjectName();
  const MYODD_CHAR* lpName = name.c_str();

  MYODD_LPARAM lParam;
  CONFIG_NOTIFY fnNotif;
  unsigned int idx = 0;
  while( GetNotif( idx++, fnNotif, lParam ) )
  {
    (*fnNotif)( lpName, type, lParam );
  }
}

/**
 * Return if the data is set or not.
 * @param none
 * @return bool if the data is set or not.
 */
bool ConfigObject::IsSet() const
{
  return m_bIsSet;
}

/**
 * Set the 'isset' value.
 * A value is set by default, when we delete it, it is 'unset'
 * @param bool if the value is set or not.
 * @return bool if we changed the value or not.
 */
bool ConfigObject::SetIsSet( bool is )
{
  if( m_bIsSet == is )
  {
    return false;
  }

  m_bIsSet = is;
  return true;
}

} //  config
} //  myodd