#pragma once
#ifndef __Config_h
#define __Config_h

#include "ConfigBase.h"

namespace myodd{ namespace config{

bool init( const STD_TSTRING& sz );
void free();
bool HasBeenInitialized();

bool set ( const STD_TSTRING& stdNotifyName, const DATA_CONTAINER& data );

// set a group of values.
bool set ( const STD_TSTRING& stdVarName, const Data &d );
bool setTemp ( const STD_TSTRING& stdVarName, const Data &d );

// get a value, return a default if it does not exits.
const Data& get( const STD_TSTRING& stdVarName,  const Data& defaultValue = Data() );
const Data& getTemp( const STD_TSTRING& stdVarName,  const Data& defaultValue = Data() );

// return true if a value is set
bool isset( const STD_TSTRING& stdVarName );

// unset a value.
void unset( const STD_TSTRING& stdVarName );

// add an item to monitor
// call the function if/when the item change
void AddConfigurationMonitor( const STD_TSTRING& s, CONFIG_NOTIFY notif, LPARAM lParam );

template <class T>
void AddConfigurationMonitor( const STD_TSTRING& s, CONFIG_NOTIFY notif, T* lParam ){
  AddConfigurationMonitor( s, notif, (LPARAM)lParam );
}

// add an item to monitor
// call the function if/when the item change
void RemoveConfigurationMonitor( const STD_TSTRING& s, CONFIG_NOTIFY notif, LPARAM lParam );

template <class T>
void RemoveConfigurationMonitor( const STD_TSTRING& s, CONFIG_NOTIFY notif, T* lParam ){
  RemoveConfigurationMonitor( s, notif, (LPARAM)lParam );
}

} //  config
} //  myodd

#endif  //  __Config_h