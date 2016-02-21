#include "stdafx.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif // _DEBUG
#endif // __AFXWIN_H__

#include "ConfigBase.h"

#include "../files/files.h"
#include "../xml/tinyxml.h"

namespace myodd{ namespace config{
/**
 * The constructor.
 * @param LPCTSTR the path of the config file.
 * @return none.
 */
ConfigBase::ConfigBase( LPCTSTR szPath ) : 
  m_szRegPath( szPath ),
  m_bIsDirty( false )
{
}

/**
 * The destructor. Clear all the values.
 *
 * @param none
 * @return none.
 */
ConfigBase::~ConfigBase()
{
  //  remove all the part notifications
  DeleteNotifyParts();

  erase();
}

/**
 * In that node look for a 'type' attribute, if it exists then we will look for a value.
 * @param TiXmlElement& the current node
 * @param std::vector<STD_TSTRING>& the current list of parent names.
 * @return none.
 */
void ConfigBase::_addLoadElement
( 
  TiXmlElement& root, 
  std::vector<STD_TSTRING>& parents 
)
{
  //  get the type
  STD_TSTRING stdType;
  static const STD_TSTRING sep = _T("\\");
  if( TIXML_SUCCESS == root.QueryValueAttribute( _T("type"), &stdType ) )
  {
    //  create the name of the object that we will be adding.
    STD_TSTRING stdName = myodd::strings::implode( parents, sep );

    // get the value that we saved.
    LPCTSTR szValue = root.GetText();

    // and get the type of the object
    long ltype = _tstol( stdType.c_str() );

    // depending on the type.
    switch( ltype )
    {
    case Data::type_string:
      //  Set as an string
      Add( stdName, szValue ? szValue : _T("") );
      break;

    case Data::type_long:
      //  Set as an long
      Add( stdName, szValue ? _tstol( szValue ) : 0 );
      break;

    case Data::type_double:
      //  Set as an double
      Add( stdName, szValue ? _tstof( szValue ) : 0.f );
      break;

    case Data::type_int:
      //  Set as an int
      Add( stdName, szValue ? _tstoi( szValue ) : 0 );
      break;

    case Data::type_int64:
      //  Set as an int64
      Add( stdName, szValue ? _tstoi64( szValue ) : 0 );
      break;

    default:
      ASSERT( 0 );  //  what type is it?
      break;
    }
  }
}

/**
 * Read the root element and look for variables to add.
 * If we find sub nodes we will continue to look for more values.
 * <Config>
 *   <commands>
 *     <current.bold type="8">0</current.bold>
 *     ...
 *   </commands>
 *   ...
 * <Config>
 *
 * @param TiXmlElement& the current node we are traversing.
 * @param std::vector<STD_TSTRING>& the current names of the parents, (so we can rebuild them in a single string).
 * @return none.
 */
void ConfigBase::_addLoadElements
( 
  TiXmlElement& root, 
  std::vector<STD_TSTRING>& parents 
)
{
  // Look for all the child nodes, they can all be valid item.
  TiXmlElement* pValues = root.FirstChildElement( );
  while( pValues )
  {
    // we must add that new name to the list.
    parents.push_back( pValues->Value() );

    // add that element if there is anything to actually add.
    // @see _addLoadElement( ... )
    _addLoadElement( *pValues, parents );

    // or/and add the sibling elements.
    _addLoadElements( *pValues, parents );

    // remove the item we just added, no longer needed, (we just parsed it).
    // we are going back down one level.
    parents.erase( parents.end() -1 );

    // and look for the next one.
    pValues = pValues->NextSiblingElement();
  }
}

/**
 * Load all the config values from the XML
 *
 * @param none
 * @return none.
 */
void ConfigBase::load_values()
{
  // Open this file and look for values.
  TiXmlDocument doc( m_szRegPath.c_str() );
  if (!doc.LoadFile())
  {
    return;
  }

  // get the config document.
  TiXmlElement* pElemConfig = doc.FirstChildElement( _T("Config"));
  if( !pElemConfig )
  {
    return;
  }

  // starting from the beginning we look for sub nodes
  // once there are no more nodes we will add the value.
  // @see _addLoadElements( ... )
  std::vector<STD_TSTRING> parents;
  _addLoadElements( *pElemConfig, parents ); 

  // loading the values will, by default set the values to dirty
  // so we need to force the value to false.
  SetDirty( false );
}

/**
 * Given an array of variables we create/look for various node.
 * So if the user creates a node "something/else" we will create the nodes "something->else"
 *
 * @param TiXmlElement& the current node we are adding elements to.
 * @param const std::vector<STD_TSTRING>& the array of elements we are traversing.
 * @return TiXmlElement* the last element that we will be adding data to.
 */
TiXmlElement* ConfigBase::_getSaveElement
( 
  TiXmlElement& root, 
  const std::vector<STD_TSTRING>& parents 
)
{
  //  sanity check
  if( parents.size() == 0 )
  {
    return NULL;
  }

  // get the first parent in the list as a starting point.
  LPCTSTR parent = parents[ 0 ].c_str();

  // Look for that element
  // if it does not exist then we must add it to the parent/root items
  TiXmlElement* pElement = root.FirstChildElement( parent );
  if( NULL == pElement )
  {
    // we could not find it, so we must add it to the list.
    pElement = new TiXmlElement( parent );
    
    // add this node to the parent
    root.LinkEndChild( pElement );
  }

  // If this was the last item then 
  // either the created item or the item that was found needs to be added.
  if( parents.size() == 1 )
  {
    return pElement;
  }

  // this was not the last item, so we need to remove the string from the list
  // and continue going down the list.
  std::vector<STD_TSTRING> tmpParents;
  for( std::vector<STD_TSTRING>::const_iterator it = parents.begin()+1; it != parents.end(); it++ )
  {
    tmpParents.push_back( *it );
  }

  // using this temp value we now look for the next item.
  return _getSaveElement( *pElement, tmpParents );
}

/**
 * Tell the class to save all the values to the given directory/file.
 *
 * @param none
 * @return none
 */
void ConfigBase::save_values()
{
  //  do we have anything to save?
  if( !IsDirty() )
  {
    return;
  }

  // create a new plan
  TiXmlDocument doc;

  // save the basic items
  // the declaration
  TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
  doc.LinkEndChild( decl );

  TiXmlElement* pElemConfig = new TiXmlElement( _T("Config"));
  doc.LinkEndChild( pElemConfig );

  //  and then we go around adding values to the values node.
  static TCHAR sep = _T('\\');
  BOOST_FOREACH( MAP_CONFIGOBJECTS::value_type& it, m_pData )
  {
    ConfigObject* c = it.second;

    // first check if the data is temp.
    // if it is temp then the user does not want to save it.
    if( !c->IsSet() )
    {
      continue;
    }

    if( !c->IsSet() )
    {
      continue;
    }

    // the data
    Data& data = c->data();

    // the data was never set or is invalid
    // either way we do not want to save it.
    if( !data.IsValid() )
    {
      continue;
    }

    std::vector<STD_TSTRING> parents;
    myodd::strings::explode( parents, c->GetObjectName(), sep );
    TiXmlElement* pValue = _getSaveElement( *pElemConfig, parents );
    
    TiXmlText* text = NULL;
    switch( data.type() )
    {
    case Data::type_string:
      pValue->SetAttribute( _T("type"), Data::type_string );
      text = new TiXmlText( (LPCTSTR)(data) );
      break;

    case Data::type_long:
      pValue->SetAttribute( _T("type"), Data::type_long );
      text = new TiXmlText( myodd::strings::ToString( (long)data ));
      break;

    case Data::type_double:
      pValue->SetAttribute( _T("type"), Data::type_double );
      text = new TiXmlText( myodd::strings::ToString( (double)data ));
      break;

    case Data::type_int:
      {
      pValue->SetAttribute( _T("type"), Data::type_int );
      text = new TiXmlText( myodd::strings::ToString( (int)data ));
      }
      break;

    case Data::type_int64:
      {
        pValue->SetAttribute( _T("type"), Data::type_int64 );
        text = new TiXmlText( myodd::strings::ToString( (__int64)data ));
      }
      break;

    default:
      // What is it?
      // because of the IsValid() function above we know that this
      // is a new data type that needs to be saved.
      assert( 0 );
      continue;
      break;
    }
    pValue->LinkEndChild( text );
  }

  doc.SaveFileUTF8( m_szRegPath.c_str() );
}

/**
 * Empty all the data from the map.
 * And deletes all the values properly
 *
 * @return none
 */
void ConfigBase::erase()
{
  //
  // lock the values, because we are erasing everything we cannot allow anyone to use it.
  myodd::threads::AutoLock lock( *this );

  BOOST_FOREACH( MAP_CONFIGOBJECTS::value_type& it, m_pData )
  {
    ConfigObject* p = it.second;
    delete p;
  }
  m_pData.erase( m_pData.begin(), m_pData.end() );
}

/**
 * Check if a variable is set, (if it was given a value ).
 * @param string the variable we are looking for.
 * @return boolean true if the value does exists or false if it does not.
 */
bool ConfigBase::Isset( const STD_TSTRING& stdVarName ) const
{
  //
  // we must prevent other threads from deleting anything while we are looking for it.
  myodd::threads::AutoLock lock( *this );

  // simply look for this object
  ConfigObject* object = NULL; 
  if( Find( stdVarName, object ) )
  {
    return object->IsSet();
  }

  // if we are here it does not exist.
  return false;
}

/**
 * Unset a variable if it exists.
 * @param const STD_TSTRING& the name of the variable we are un-setting.
 * @return bool if the item was removed or not.
 */
bool ConfigBase::Unset( const STD_TSTRING& stdVarName )
{
  ConfigObject* object = NULL;
  if( !Find( stdVarName, object ))
  {
    // the value was never set in the first place.
    return false;
  }

  // set it as UnSet, if the return value is false
  // the it means that the value was already unset.
  bool result = object->SetIsSet( false );

  // set the data to invalid.
  object->data() = Data();

  // and remove all the notifs
  object->RemoveAllNotifs();

  return result;
}

/**
 * Find of an object
 * @param const STD_TSTRING& the name of the variable we are looking for.
 * @param Object*& container that will contain the return value, (if found).
 * @return bool if the value was found or not.
 */
bool ConfigBase::Find( const STD_TSTRING &szVarName, ConfigObject*& obj ) const
{
  obj = NULL;
  MAP_CONFIGOBJECTS_CONST_IT it = m_pData.find( szVarName );
  if( m_pData.end() == it )
  {
    return false;
  }
  obj = it->second;
  return true;
}

/**
 * Add a variable/value to the array of object
 * @param const STD_TSTRING& the name of the variable we are watching.
 * @param const Data& the data we are adding.
 * @return bool true|false if the item was added/value changed
 */
bool ConfigBase::Add
( 
  const STD_TSTRING& stdVarName, 
  const Data &d,
  bool isTemp /*= false*/
)
{
  ConfigObject* object = NULL;
  
  bool isNew = false;
  bool isDirty = false;
  bool wasChanged = false;

  {
    //
    // lock the values
    myodd::threads::AutoLock lock( *this );

    // get the value if it already exist
    if( !Find( stdVarName, object ) )
    {
      // it is new
      isNew = true;
      
      // so we changed it,
      wasChanged = true;

      // if the value is new then it is dirty
      // but we only set is dirty if we are not a temp value.
      isDirty = !isTemp;

      // the value does not exits 
      // so we need to create a new object for it.
      object = new ConfigObject( stdVarName, d );

      // set if the value is set or not.
      object->SetIsSet( !isTemp );

      // And add it to the mad that was passed to us.
      m_pData[ object->GetObjectName() ] = ( object );
    }

    // if we are not temp, then the value is 'set'
    if( object->SetIsSet( !isTemp ) )
    {
      // the temp value has changed, either were temp and we are not anymore
      // or we were not and we are now.
      // in both cases we are dirty.
      isDirty = true;
      wasChanged = true;
    }

    // set the value for that object
    if( object->data().SetValue( d ) )
    {
      // the value has changed
      // if we are not temp then the value is dirty, no other check is needed.
      // but if we are temp then we are only dirty if we already set the value to true, (by changing the temp value itself).
      isDirty = (isTemp && isDirty) || (!isTemp);
      wasChanged = true;
    }
  }
  
  // if the item is not new then it is possible that we have notifications.
  if( isDirty )
  {
    SetDirty( true );
    if( !isNew )
    {
      // we know something changed so we need to notify.
      //
      // we are out of lock, it is possible for object to be deleted...
      object->Notify( d.type() );
    }
  }

  // this value was updated.
  return wasChanged;
}

/**
 * Set one of more values at the same time and notify a common name, (probably part of)
 * @param const STD_TSTRING& the name that we send to notify this global update.
 * @param const DATA_CONTAINER& the name/data that we are updating.
 * @return bool if any data was changed at all.
 */
bool ConfigBase::Add( const STD_TSTRING& stdNotifyName, const DATA_CONTAINER& data )
{
  bool bNotif = false;
  m_pDataNotify.BlockNotifications();
  try
  {
    for( DATA_CONTAINER::const_iterator it = data.begin();
         it != data.end();
         ++it )
    {
      if( Add( it->first, it->second ) )
      {
        bNotif = true;
      }
    }
  }
  catch(...)
  {
  }
  m_pDataNotify.UnblockNotifications();

  if( bNotif )
  {
    NotifyParts( Data::type_unknown, stdNotifyName );
  }
  return bNotif;
}

/**
* Remove a function that was called when parts of a variable name is changed.
* @param const STD_TSTRING& the name of the variable we were watching.
* @param CONFIG_NOTIFY the function that was called when the function changed.
* @param LPARAM the param that was passed when the function is called.
* @return none
*/
void ConfigBase::RemovePartMonitor
( 
  const STD_TSTRING& stdVarName, 
  CONFIG_NOTIFY notif, 
  LPARAM lParam 
)
{
  // lock the thread
  myodd::threads::AutoLock lock( *this );

  LPARAM lParamCur;
  CONFIG_NOTIFY fnNotif;
  unsigned int idx = 0;
  while( m_pDataNotify.GetNotif( idx++, fnNotif, lParamCur ) )
  {
    CONFIG_NOTIFY_PARTS* part = ((CONFIG_NOTIFY_PARTS*)lParamCur);
    if( part->lParam == lParam && part->stdVarName == stdVarName )
    {
      m_pDataNotify.RemoveNotif( notif, part );
      
      //  delete the part to clean the memory
      delete part;
    }
  }
}

/**
 * Add a function that will be called when parts of a variable name is changed.
 * @param const STD_TSTRING& the name of the variable we are watching.
 * @param CONFIG_NOTIFY the function that will be called when the function changes.
 * @param LPARAM the param that will be passed when the function is called.
 * @return none
 */
void ConfigBase::AddPartMonitor( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam )
{
  CONFIG_NOTIFY_PARTS* part = new CONFIG_NOTIFY_PARTS;

  std::vector<STD_TSTRING> v_s;
  size_t l = myodd::strings::explode( v_s, stdVarName, _T( '\\') );
  
  part->stParts     = l;
  part->lParam      = lParam;
  part->stdVarName  = stdVarName;

  m_pDataNotify.AddNotif( notif, part );
}

/**
 * Delete all the parts we are currently watching.
 * @param none
 * @return none
 */
void ConfigBase::DeleteNotifyParts( )
{
  //
  // lock the values, because we are erasing everything we cannot allow anyone to use it.
  myodd::threads::AutoLock lock( *this );

  LPARAM lParam;
  CONFIG_NOTIFY fnNotif;
  unsigned int idx = 0;
  while( m_pDataNotify.GetNotif( idx++, fnNotif, lParam ) )
  {
    delete ((CONFIG_NOTIFY_PARTS*)lParam);
  }
  m_pDataNotify.RemoveAllNotifs();
}

/**
 * Notify all the items that are watching parts.
 * @param Data::config_type the data type been notified.
 * @param const STD_TSTRING& the var name that we are notifying.
 * @return 
 */
void ConfigBase::NotifyParts( Data::config_type type, const STD_TSTRING& stdVarName )
{
  if( 0 == m_pDataNotify.GetNumberOfNotifs() )
  {
    //  nothing to do.
    return;
  }

  std::vector<STD_TSTRING> v_s;
  size_t l = myodd::strings::explode( v_s, stdVarName, _T('\\') );
  
  LPARAM lParam;
  CONFIG_NOTIFY fnNotif;
  unsigned int idx = 0;
  while( m_pDataNotify.GetNotif( idx++, fnNotif, lParam ) )
  {
    //  get the parts.
    CONFIG_NOTIFY_PARTS* part = (CONFIG_NOTIFY_PARTS*)lParam;
    if( l < part->stParts-1/*the last one is the '*' */ )
    {
      //  it is to small.
      continue;
    }

    // how many parts are we looking for?
    // join it together.
    STD_TSTRING s = myodd::strings::implode( v_s, _T("\\"), 0, part->stParts-1 );
    s += _T("\\*");

    if( s != part->stdVarName )
      continue; //  not the same

    //  it is the same, so we can call the function.
    // note that we are calling the full name of the item that was changed.
    (*fnNotif)( stdVarName.c_str(), type, part->lParam );
  }
}

/**
 * Add a function that will be called when a variable name is changed.
 * @param const STD_TSTRING& the name of the variable we are watching.
 * @param CONFIG_NOTIFY the function that will be called when the function changes.
 * @param LPARAM the param that will be passed when the function is called.
 * @return bool success or not.
 */
bool ConfigBase::AddMonitor ( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam )
{
  //  look for that item
  ConfigObject* object = NULL;
  if( Find( stdVarName, object ) )
  {
    object->AddMonitor( notif, lParam );
    return true;
  }

  // just add a new empty item
  // when/if that item is loaded created we will call the function, (unless the value is NULL).
  if( !Add( stdVarName, Data() ) )
  {
    return false;
  }

  //  and add the notif
  return AddMonitor ( stdVarName, notif, lParam );
}

const Data& ConfigBase::Get
( 
  const STD_TSTRING& stdVarName, 
  const Data& default,
  bool isTemp /*= false*/
)
{
  //
  // lock the values
  myodd::threads::AutoLock lock( *this );

  ConfigObject* object = NULL;
  if( Find( stdVarName, object ) )
  {
    Data& data = object->data();

    // get the data, if it is not really set
    // then return the default value.
    if( !data.IsValid() )
    {
      // we don't call set(...) as this will call all the monitors when in fact
      // the value is not really changing, (or even been set for that matter).
      // we are just setting a value in memory so we can return it.
      data = default;
    }

    // if the value was set, and it no longer is then we need to mark ourselves as dirty.
    bool wasTemp = !object->IsSet();
    if( object->SetIsSet( !isTemp ) )
    {
      if( !wasTemp )
      {
        SetDirty( true );
      }
    }

    // this is the data we have
    // either the default given or the value it was.
    return data;
  }

  //
  // see warning above before the catch
  //
  if( !Add( stdVarName, default, isTemp ) )
  {
    ASSERT(0);  //  why?
    throw -1;
  }

  // we will throw if the value somehow does not exist
  // this is to make sure we don't end up in never ending loop.
  if( !Find( stdVarName, object ) )
  {
    ASSERT(0);  //  we just created it???
    throw -1;
  }

  // the data is 'fake' we did not really set the value.
  // it is a temp value that we created so we can return it next.
  object->SetIsSet( false );
  return object->data();
}

/**
 * Remove a function that will be called when a variable name is changed.
 * @param const STD_TSTRING& the name of the variable we are no longer watching.
 * @param CONFIG_NOTIFY the function that was called when the function changed.
 * @param LPARAM the param that was passed when the function was called.
 * @return bool success or not.
 */
bool ConfigBase::RemoveMonitor( const STD_TSTRING& stdVarName, CONFIG_NOTIFY notif, LPARAM lParam )
{
  ConfigObject* object = NULL;
  if( Find( stdVarName, object ) )
  {
    return object->RemoveMonitor( notif, lParam );
  }

  // does not exit!
  return false;
}

} //  config
} //  myodd
