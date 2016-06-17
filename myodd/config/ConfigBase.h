#pragma once
#ifndef __ConfigBase_h
#define __ConfigBase_h

#include "../string/string.h"
#include "../notif/notif.h"

#include "ConfigData.h"
#include "ConfigObject.h"

#include "../xml/tinyxml2.h"

namespace myodd{ namespace config{

typedef std::map<STD_TSTRING, Data> DATA_CONTAINER;
//  ---------------------------------------------------------------------------------------------------
class ConfigBase : 
  public myodd::threads::CritSection
{
public:
  ConfigBase(const STD_TSTRING& szPath );
  virtual ~ConfigBase();

public:
  bool LoadValuesFromFile();

  /**
  * Save all the values if the class is 'dirty'
  * @return none
  */
  void Save();
protected:
  tinyxml2::XMLElement* _getSaveElement(tinyxml2::XMLElement& root, const std::vector<STD_TSTRING>& parents );
  void _addLoadElements( const tinyxml2::XMLElement& root, std::vector<STD_TSTRING>& parents );
  void _addLoadElement( const tinyxml2::XMLElement& root, std::vector<STD_TSTRING>& parents );

protected:
  //  prevent copies
  ConfigBase( const ConfigBase& );
  const ConfigBase& operator=( const ConfigBase& );

protected:
  /**
  * Erase all the data from the map.
  * And deletes all the values properly
  * @return none
  */
  void erase();

public:
  /**
  * All the get/set functions
  * You can use get_x when you want a specific type to be returned
  */
  bool AddMonitor ( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam );
  bool RemoveMonitor ( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam );

  void AddPartMonitor( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam );
  void RemovePartMonitor( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam );

  /**
  * Add an object to the array of ConfigBase objects
  */
  bool Add( const STD_TSTRING& stdVarName, const Data& d, bool isTemp = false );
  bool Add( const STD_TSTRING& stdNotifyName, const DATA_CONTAINER& data );

  const Data& Get( const STD_TSTRING& stdVarName, const Data& default, bool isTemp = false );
private:

  struct  CONFIG_NOTIFY_PARTS
  {
    size_t stParts;
    LPARAM lParam;
    STD_TSTRING stdVarName;
  };
  myodd::notif::Notifications<CONFIG_NOTIFY> m_pDataNotify;
  void NotifyParts( Data::config_type type, const STD_TSTRING& stdVarName );
  void DeleteNotifyParts( );

protected:
  //
  // Find a variable.
  bool Find( const STD_TSTRING &szVarName, ConfigObject*& obj ) const;

public:
  //
  // Check if a value is set or not.
  bool Isset( const STD_TSTRING& stdVarName ) const;

  //
  // Unset the object value.
  bool Unset( const STD_TSTRING& stdVarName );

public:
  bool IsDirty() const{ return m_bIsDirty; }

protected:
  void SetDirty( bool bIs ){  
    m_bIsDirty = bIs;  
  }

protected:
  //
  // All the objects, a map by name/object*
  MAP_CONFIGOBJECTS m_pData;

  //
  // The full path of the XML
  std::string m_szRegPath;

  //
  // If the data is dirty, (and must be saved).
  bool m_bIsDirty;

  // the current document
  tinyxml2::XMLDocument* _doc;
  void CloseDoc();
  //  ---------------------------------------------------------------------------------------------------
};

} //  config
} //  myodd

#endif  //  __ConfigBase_h