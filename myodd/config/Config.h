#pragma once
#ifndef __Config_h
#define __Config_h

#include "ConfigBase.h"

namespace myodd{ namespace config{

bool init( const MYODD_STRING& sz );
void free();
bool HasBeenInitialized();

bool set ( const MYODD_STRING& stdNotifyName, const DATA_CONTAINER& data );

// set a group of values.
bool set ( const MYODD_STRING& stdVarName, const Data &d );
bool setTemp ( const MYODD_STRING& stdVarName, const Data &d );

// get a value, return a default if it does not exits.
const Data& get( const MYODD_STRING& stdVarName,  const Data& defaultValue = Data() );
const Data& getTemp( const MYODD_STRING& stdVarName,  const Data& defaultValue = Data() );

// return true if a value is set
bool isset( const MYODD_STRING& stdVarName );

// unset a value.
void unset( const MYODD_STRING& stdVarName );

// add an item to monitor
// call the function if/when the item change
void AddConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, LPARAM lParam );

template <class T>
void AddConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, T* lParam ){
  AddConfigurationMonitor( s, notif, (LPARAM)lParam );
}

// add an item to monitor
// call the function if/when the item change
void RemoveConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, LPARAM lParam );

template <class T>
void RemoveConfigurationMonitor( const MYODD_STRING& s, CONFIG_NOTIFY notif, T* lParam ){
  RemoveConfigurationMonitor( s, notif, (LPARAM)lParam );
}

} //  config
} //  myodd

#endif  //  __Config_h