#include "stdafx.h"
#include "../files/files.h"
#include "Config.h"
#include <assert.h>

// --------------------------------------------------------------------------
#define IS_VALID_CONTAINER  if( NULL == m_staticContainer ) \
                            {                               \
                              ASSERT (0);                   \
                              throw -1;                     \
                            }

namespace myodd{ namespace config{

/**
 * The one and only data container.
 * @param ConfigBase* the static data.
 */
ConfigBase* m_staticContainer = NULL;

/**
 * Read/load the definition file.
 * Set all the values.
 *
 * @param const MYODD_STRING& the path of the configuration file.
 * @return bool success or not.
 */
bool init( const MYODD_STRING& sz )
{
  free();

  //  make sure that this file path does exist.
  if( !myodd::files::CreateFullDirectory( sz, true ) )
  {
    return false;
  }

  MYODD_STRING lpDest;
  if( !myodd::files::ExpandEnvironment( sz, lpDest ))
  {
    return false;
  }

  // configure the base now,
  m_staticContainer = new ConfigBase( lpDest.c_str() );
  return m_staticContainer->LoadValuesFromFile();
}

/**
 * Release the container and free all the memory.
 *
 * @param none
 * @return none
 */
void free()
{
  if( m_staticContainer )
  {
    m_staticContainer->Save();
    delete m_staticContainer;
    m_staticContainer = NULL;
  }
}

/**
 * Check if the configuration has been initialized
 * @return bool if the container has been initialized.
 */
bool HasBeenInitialized()
{
  return (m_staticContainer != NULL );
}

/**
 * Set a value for a configuration item.
 *
 * @param const MYODD_STRING& s the name of the config item we are changing
 * @param Data& the new value
 * @return bool true|false if the item was added/value changed
 */
bool set ( const MYODD_STRING& stdVarName, const Data &d )
{
  // remember that we are throwing if something is wrong!
  IS_VALID_CONTAINER
  return m_staticContainer->Add( stdVarName, d );
}

/**
 * Set one of more values at the same time and notify a common name, (probably part of)
 * @param const MYODD_STRING& the name that we send to notify this global update.
 * @param const DATA_CONTAINER& the name/data that we are updating.
 * @return bool if any data was changed at all.
 */
bool set ( const MYODD_STRING& stdNotifyName, const DATA_CONTAINER& data )
{
  // remember that we are throwing if something is wrong!
  IS_VALID_CONTAINER

  // do not lock the thread here
  // as we have notifications we need to be careful what we lock.
  return m_staticContainer->Add( stdNotifyName, data );
}

/**
 * Set a value for a configuration item.
 *
 * @param const MYODD_STRING& s the name of the config item we are changing
 * @param Data& the new value
 * @return bool true|false if the item was added/value changed
 */
bool setTemp ( const MYODD_STRING& stdVarName, const Data &d )
{
  IS_VALID_CONTAINER
  return m_staticContainer->Add( stdVarName, d, true );
}

/**
 * Add a function to monitor a variable change.
 * When the variable changes then we will call that particular function.
 * @see CONFIG_NOTIFY
 *
 * @param const MYODD_STRING& s the name of the object been monitored.
 * @param CONFIG_NOTIFY the config notifier, (function definition)
 * @param MYODD_LPARAM a single parameter that will be passed to the function.
 * @return none
 */
void AddConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, MYODD_LPARAM lParam )
{
  IS_VALID_CONTAINER
  std::vector<MYODD_STRING> v_s;
  size_t l = myodd::strings::explode( v_s, s, _T('\\') );
  
  if( v_s[l-1] == _T( "*" ) )
  {
    // we are monitoring any parts of an object.
    m_staticContainer->AddPartMonitor( s, notif, lParam );
  }
  else
  {
    //  we are monitoring an exact path.
    m_staticContainer->AddMonitor( s, notif, lParam );
  }
}

void RemoveConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, MYODD_LPARAM lParam )
{
  IS_VALID_CONTAINER
  std::vector<MYODD_STRING> v_s;
  size_t l = myodd::strings::explode( v_s, s, _T('\\') );
  if( v_s[l-1] == _T( "*" ) )
  {
    // we are monitoring any parts of an object.
    m_staticContainer->RemovePartMonitor( s, notif, lParam );
  }
  else
  {
    //  we are monitoring an exact path.
    m_staticContainer->RemoveMonitor( s, notif, lParam );
  }
}

/**
 * return true if a config item exists.
 * @param const MYODD_STRING& the name of the object we are checking.
 * @return none
 */
bool isset( const MYODD_STRING& stdVarName )
{
  IS_VALID_CONTAINER
  return m_staticContainer->Isset( stdVarName );
}

/**
 * Unset a config object from the list of items.
 * @param const MYODD_STRING& the name of the object been unset.
 * @return none
 */
void unset( const MYODD_STRING& stdVarName )
{
  IS_VALID_CONTAINER
  m_staticContainer->Unset( stdVarName );
}

/**
 * Get the current value of a config object.
 * The calling function will get the right data for that object.
 *
 * @param const MYODD_STRING& s the name of the config item we are getting
 * @param Data& the default value should the value not be found.
 * @return const Data& default value|actual value.
 */
const Data& getTemp( const MYODD_STRING& stdVarName,  const Data& defaultValue )
{
  IS_VALID_CONTAINER
  return m_staticContainer->Get( stdVarName, defaultValue, true );
}

/**
 * Get the current value of a config object.
 * The calling function will get the right data for that object.
 *
 * @param const MYODD_STRING& s the name of the config item we are getting
 * @param Data& the default value should the value not be found.
 * @return const Data& default value|actual value.
 */
const Data& get( const MYODD_STRING& stdVarName,  const Data& defaultValue )
{
  IS_VALID_CONTAINER
  return m_staticContainer->Get( stdVarName, defaultValue );
}

} //  config
} //  myodd
