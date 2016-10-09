// ***********************************************************************
// Copyright (c) 2016 Florent Guelfucci
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// @see https://opensource.org/licenses/MIT
// ***********************************************************************
#pragma once

#include <string>
#include <vector>
#include <map>
#include "../dynamic/any.h"
#include "../xml/tinyxml2.h"
#include "../threads/threads.h"

namespace myodd{ namespace config{

  class Data : public myodd::threads::CritSection
  {
  public:
    /**
     * The constructor, with the root name we will use.
     * @param const std::wstring& rootName the rootname we want to use.
     */
    Data(const std::wstring& rootName = L"Config" );

    virtual ~Data();

    /**
     * Check if the value has been set or not.
     * @param const std::wstring& path the path we are looking for.
     * @return bool if the path exists or not.
     */
    bool Contains(const std::wstring& path) const;

    /**
     * Add a value to the map of values.
     * @param const std::wstring& name the name of the value we are setting.
     * @param long type the data type
     * @param const ::myodd::dynamic::Any& value the value been added.
     * @param bool isTemp
     */
    void Set(const std::wstring& name, const ::myodd::dynamic::Any& value, bool isTemp );

    /**
     * Try and get a value, if the value is not found, we throw.
     * @param const std::wstring& path the name of the value we are looking for.
     * @return ::myodd::dynamic::Any the value, if it exists.
     */
    const ::myodd::dynamic::Any& Get(const std::wstring& path) const;

    /**
     * Try and get a value, if the value is not found, we throw.
     * @param const std::wstring& path the name of the value we are looking for.
     * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
     * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
     */
    const ::myodd::dynamic::Any& Get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) const;

    /**
     * Check if a given path is a temp value or not
     * @param const std::wstring& path the path we are looking for.
     * @return bool if the value is a path or not.
     */
    bool IsTemp(const std::wstring& path) const;

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
     * Convert the source value to a safe, clean value
     * in other words a trimmed, lower case path.
     * We will throw an exception if the value is empty or contains illigal characters.
     * @param const std::wstring& src the source value we are cleaning up.
     * @return std::wstring the cleaned-up name
     */
    static std::wstring SafeName(const std::wstring& src);

    /**
     * Check if the given element is valid or not.
     * @param const std::wstring& src the element we are checking.
     */
    static void ValidateElementName(const std::wstring& src);

    /** 
     * Get the data type given the version number of the config
     * Older xmls have different data types.
     * We will throw if the data type is unknown.
     * @param const long version the xml file version number
     * @param const long givenType the type we want to convert.
     * @return ::myodd::dynamic::Type the converted data type.
     */
    static ::myodd::dynamic::Type ConvertToDataType(const long version, const long givenType);

    /**
     * Create the data holder from an XML
     * @return bool success or not.
     */
    bool FromXMLDocument( const tinyxml2::XMLDocument& doc );

#ifdef UNICODE
    static std::string ToChar( const std::wstring& s );
#else
    static std::string ToChar( const std::string& s );
#endif

    /**
     * Parse the data from the root.
     * @param const long versionNumber the data version number.
     * @param tinyxml2::XMLElement& the current node we are traversing.
     * @param std::vector<std::wstring>& the current names of the parents, (so we can rebuild them in a single string).
     */
    void WalkElements(const long versionNumber, const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents);
  
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
     * @param tinyxml2::XMLElement& the current node we are traversing.
     * @param std::vector<std::wstring>& the current names of the parents, (so we can rebuild them in a single string).
     */
    void WalkElements( const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents);

    /**
     * In that node look for a 'type' attribute, if it exists then we will look for a value.
     * @param const long versionNumber the data version number.
     * @param tinyxml2::XMLElement& the current node
     * @param std::vector<std::wstring>& the current list of parent names.
     */
    void WalkElement(const long versionNumber, const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents);

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
      bool _temp;
      bool _dirty;
    };

    // all the values
    typedef std::map<std::wstring, DataStruct> DataStructs;
    DataStructs _values;

    /**
     * Look for a certain data structure, return nullptr if we do not find it.
     * @param const std::wstring& name the value we are looking for.
     * @return DataStruct* the data struct we found.
     */
    const DataStruct* GetRaw(const std::wstring& name) const;

    // the config name
    const std::wstring  _rootName;
  };
} //  config
} //  myodd
