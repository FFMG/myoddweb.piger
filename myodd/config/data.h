#pragma once

#include <string>
#include <vector>
#include <map>
#include "../dynamic/any.h"
#include "../cache/memorycache.h"
#include "../xml/tinyxml2.h"
#include "../threads/threads.h"

namespace myodd{ namespace config{

  class Data : public myodd::threads::CritSection
  {
  public:
    /**
    * The various types we are saving the config to/from
    */
    enum config_type
    {
      type_unknown = 0x00,
      type_string = 0x01,
      type_long = 0x02,
      type_double = 0x04,
      type_int = 0x08,
      type_int64 = 0x10,
      type_all = (type_string | type_string | type_long | type_double | type_int | type_int64),
    };

  public:
    Data();

    virtual ~Data();

    /**
     * Add a value to the map of values.
     * @param const std::wstring& name the name of the value we are setting.
     * @param long type the data type
     * @param const ::myodd::dynamic::Any& value the value been added.
     * @param bool isTemp
     */
    void Set(const std::wstring& name, long type, const ::myodd::dynamic::Any& value, bool isTemp );

    /**
     * Try and get a value, if the value is not found, we throw.
     * @param const std::wstring& path the name of the value we are looking for.
     * @return ::myodd::dynamic::Any the value, if it exists.
     */
    ::myodd::dynamic::Any Get(const std::wstring& path) const;

    /**
     * Try and get a value, if the value is not found, we throw.
     * @param const std::wstring& path the name of the value we are looking for.
     * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
     * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
     */
    ::myodd::dynamic::Any Get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) const;

    /**
     * Create the data holder from an XML file.
     * @path const std::wstring& path the path of the xml we want to load.
     * @return bool success or not.
     */
    bool FromXMLFile(const std::wstring& path);

    /**
     * Create the data holder from an XML
     * @path const std::wstring& source the xml source
     * @return bool success or not.
     */
    bool FromXML(const std::wstring& source );

  protected:
    /**
     * Create the data holder from an XML
     * @return bool success or not.
     */
    bool FromXMLDocument(tinyxml2::XMLDocument& doc );

#ifdef UNICODE
    static std::string ToChar( const std::wstring& s );
#else
    static std::string ToChar( const std::string& s );
#endif

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
     * @param tinyxml2::XMLElement& the current node we are traversing.
     * @param std::vector<MYODD_STRING>& the current names of the parents, (so we can rebuild them in a single string).
     * @return none.
     */
    void WalkElements(const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents);

    /**
     * In that node look for a 'type' attribute, if it exists then we will look for a value.
     * @param tinyxml2::XMLElement& the current node
     * @param std::vector<MYODD_STRING>& the current list of parent names.
     * @return none.
     */
    void WalkElement(const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents);

    /**
     * The path to our xml file.
     */
    std::string _path;

    /**
     * If any of the data was changed or not.
     */
    bool _dirty;

    struct DataStruct
    {
      ::myodd::dynamic::Any _value;
      long _type;
      bool _temp;
      bool _dirty;
    };

    // all the values
    ::myodd::cache::MemoryCache _values;
  };
} //  config
} //  myodd
