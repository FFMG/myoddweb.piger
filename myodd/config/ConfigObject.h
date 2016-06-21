#ifndef __ConfigObject_h
#define __ConfigObject_h

#pragma once

#include "../common/includes.h"
#include "../notif/notif.h"
#include "ConfigData.h"

#include <map>

// define the class for the notification.
typedef int ( *CONFIG_NOTIFY )(const MYODD_CHAR*, unsigned int, MYODD_LPARAM lParam);

namespace myodd{ namespace config{

class ConfigObject : protected myodd::notif::Notifications<CONFIG_NOTIFY>
{
public:
  /**
   * The constructors
   */
  ConfigObject();
  ConfigObject( const ConfigObject &c);
  ConfigObject( const MYODD_STRING& stdVarName );
  ConfigObject( const MYODD_STRING& stdVarName, const Data& cod );
  virtual ~ConfigObject();

protected:
  void _Construct();
  void _Copy( const ConfigObject&c );
  const ConfigObject& operator=( const ConfigObject&c );

public:
  /** 
   * Get the current data type
   * @param none
   * @return Data::config_type the data type
   */
  Data::config_type GetDataType() const
  { 
    return m_codData.type();
  }

  /** 
   * Get the current data Name
   * @return const MYODD_STRING& the name of this object.
   */
  const MYODD_STRING& GetObjectName() const 
  {
    return m_unicodeName;
  }

  bool IsSet() const;
  bool SetIsSet( bool is );

  // set the name of the object.
  void SetObjectName  (const MYODD_STRING& n);

public:
  Data& data()              { return m_codData; }
  const Data& data() const  { return m_codData; }

protected:
  //
  // The data
  Data m_codData;

  //
  // If the data has been unset or not.
  bool m_bIsSet;

  //
  // the name of the variable.
  MYODD_STRING m_unicodeName;

public:
  bool AddMonitor( const CONFIG_NOTIFY& notif, MYODD_LPARAM lParam );
  bool RemoveMonitor( const CONFIG_NOTIFY& notif, MYODD_LPARAM lParam );
  void RemoveAllNotifs();

public:
  void Notify( Data::config_type type );
};//

//  ---------------------------------------------------------------------------------------------------
typedef std::map<MYODD_STRING, ConfigObject* > MAP_CONFIGOBJECTS;
typedef MAP_CONFIGOBJECTS::const_iterator MAP_CONFIGOBJECTS_CONST_IT;

} //  config
} //  myodd

#endif // __ConfigObject_h