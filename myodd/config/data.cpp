#pragma once

#include "../string/string.h"
#include "../files/files.h"
#include "../pcre2/regex2.h"
#include "data.h"

namespace myodd {
  namespace config {

    // the current version number
    const long _CurrentVersion = 1;

    /**
     * The constructor, with the root name we will use.
     * @param const std::wstring& rootName the rootname we want to use.
     */
    Data::Data( const std::wstring& rootName ) : 
      _path( "" ), 
      _dirty( false ),
      _values( L"Configuration" ),
      _rootName( rootName )
    {
      // validate the root name.
      ValidateElementName(_rootName);

      // and prevent '\\' as well.
      if (::myodd::regex::Regex2::Search(L"\\\\", _rootName, false))
      {
        throw std::invalid_argument("The root name cannot have 'back slash'");
      }
    }

    Data::~Data()
    {
    }

#ifdef UNICODE
    /**
     * Consvert a wide string to a string
     * @param const std::wstring& the value we are converting
     * @return std::string the converted string
     */
    std::string Data::ToChar(const std::wstring& s)
    {
      // convert wide string to string.
      return myodd::strings::WString2String(s);
    }
#else
     /**
      * Dummy function for string > string conversion
      * No work is actually done as the string is not wide.
      * @param const std::wstring& the value we are converting
      * @return std::string the converted string
      */
    std::string Data::ToChar(const std::string& s)
    {
      //  nothing to do, it is already acii
      return s;
    }
#endif
    /**
     * Convert the source value to a safe, clean value
     * in other words a trimmed, lower case path.
     * We will throw an exception if the value is empty or contains illigal characters.
     * @param const std::wstring& src the source value we are cleaning up.
     * @return std::wstring the cleaned-up name
     */
    std::wstring Data::SafeName(const std::wstring& src)
    {
      // lower case.
      auto result = myodd::strings::lower(src);

      // remove the blank characters.
      myodd::strings::Trim(result);

      // remove the '\' characters.
      myodd::strings::Trim(result, L"\\");

      // replace multiple escapes to just one.
      ::myodd::regex::Regex2::Replace(L"\\\\{2,}", L"\\", result, false);

      // validate the final value
      ValidateElementName(result);
      
      // return what we have
      return result;
    }

    /**
     * Check if the given element is valid or not.
     * @param const std::wstring& src the element we are checking.
     */
    void Data::ValidateElementName(const std::wstring& src)
    {
      // do we have anything left.
      if (src.length() == 0)
      {
        throw std::invalid_argument("The name/path cannot be empty.");
      }

      // final check if, look for any character that is not allowed.
      if (::myodd::regex::Regex2::Search(L"[^a-zA-Z0-9-\\.\\\\]", src, false))
      {
        throw std::invalid_argument("The name/path contains illigal characters.");
      }

      // look for elements that start with numbers.
      // something like "1234\value"
      if (::myodd::regex::Regex2::Search(L"^[0-9-\\.]", src, false))
      {
        throw std::invalid_argument("The name/path cannot be a number only.");
      }
      // and something like "value\1234\value"
      if (::myodd::regex::Regex2::Search(L"(\\\\[0-9-\\.])", src, false))
      {
        throw std::invalid_argument("The name/path cannot be a number only.");
      }
    }

    /**
     * Check if the value has been set or not.
     * @param const std::wstring& path the path we are looking for.
     * @return bool if the path exists or not.
     */
    bool Data::Contains(const std::wstring& path) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      //
      // does this value exist?
      return _values.Contains(lpath.c_str());
    }

    /**
     * Check if a given path is a temp value or not
     * @param const std::wstring& path the path we are looking for.
     * @return bool if the value is a path or not.
     */
    bool Data::IsTemp(const std::wstring& path) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      // do we even know about this value?
      if (!_values.Contains(lpath.c_str()))
      {
        return false;
      }

      // get the value
      auto data = (const DataStruct)_values.Get(lpath.c_str());

      // return if it is temp or not.
      return data._temp;
    }
        
    /**
    * Try and get a value, if the value is not found, we throw.
    * @param const std::wstring& path the name of the value we are looking for.
    * @return ::myodd::dynamic::Any the value, if it exists.
    */
    const ::myodd::dynamic::Any Data::Get(const std::wstring& path) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      //
      // does this value exist?
      if (!_values.Contains(lpath.c_str()))
      {
        throw std::exception("Unknown path");
      }

      // get the value.
      auto data = (const DataStruct)_values.Get(lpath.c_str());
      return data._value;
    }

    /**
    * Try and get a value, if the value is not found, we throw.
    * @param const std::wstring& path the name of the value we are looking for.
    * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
    * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
    */
    const ::myodd::dynamic::Any Data::Get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      //
      // does this value exist?
      if (!_values.Contains(lpath.c_str()))
      {
        return defaultValue;
      }

      // get the value.
      auto data = (const DataStruct)_values.Get(lpath.c_str());
      return data._value;
    }

    /**
     * Add a value to the map of values.
     * @param const std::wstring& name the name of the value we are setting.
     * @param const ::myodd::dynamic::Any& value the value been added.
     * @param bool isTemp
     */
    void Data::Set(const std::wstring& name, const ::myodd::dynamic::Any& value, bool isTemp)
    {
      //
      // lock the values
      myodd::threads::AutoLock lock(*this);

      // cleanup the name
      auto lname = SafeName(name);

      //
      // does this value exist?
      if (!_values.Contains(lname.c_str()) )
      {
        // this value does not exist, so it is new.
        // if the value is not temp, then the entire array is 'dirty'
        if (!isTemp)
        {
          _dirty = true;
        }
      }
      else
      {
        // the value aready exists.
        auto data = (DataStruct)_values.Get( lname.c_str() );
        if (data._temp && !isTemp)
        {
          // it was temp, but it no longer is temp
          // so it is now dirty.
          _dirty = true;
        }

        // if all the values are the same then there is nothing 
        // more for us to do here, we might as well move on.
        if (data._temp == isTemp && data._value == value )
        {
          return;
        }
      }

      // set the value, if we made it here, it is 'dirty'
      DataStruct ds = { value, isTemp, true };
      _values.Set( lname.c_str(), ds, ::myodd::cache::CacheItemPolicy() );
    }

    /**
     * In that node look for a 'type' attribute, if it exists then we will look for a value.
     * @param const long versionNumber the data version number.
     * @param tinyxml2::XMLElement& the current node
     * @param std::vector<std::wstring>& the current list of parent names.
     */
    void Data::WalkElement(const long versionNumber, const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents)
    {
      //  get the type
      const std::wstring sep = _T("\\");

      const tinyxml2::XMLAttribute* attributeType = root.FindAttribute("type");
      if (attributeType == NULL)
      {
        return;
      }

      // get the value
#ifdef UNICODE
      auto stdType = myodd::strings::String2WString(attributeType->Value());
      auto stdRoot = myodd::strings::String2WString(root.Value());
      auto stdText = myodd::strings::String2WString(root.GetText());
#else
      auto stdType = attributeType->Value;
      auto stdRoot = root.Value();
      auto stdText = root.GetText();
#endif // UNICODE    

      //  create the name of the object that we will be adding.
      std::wstring stdName = myodd::strings::implode(parents, sep);

      // and get the data type of the object
      long ltype = _tstol(stdType.c_str());

      // depending on the type.
      switch (ltype)
      {
      case ::myodd::dynamic::Type::Character_wchar_t:
        //  Set as an string
        Set(stdName, stdText, false);
        break;

      case ::myodd::dynamic::Type::Integer_long_int:
        //  Set as an long
        Set(stdName, _tstol(stdText.c_str()), false );
        break;

      case ::myodd::dynamic::Type::Floating_point_double:
        //  Set as an double
        Set(stdName, _tstof(stdText.c_str()), false);
        break;

      case ::myodd::dynamic::Type::Integer_int:
        //  Set as an int
        Set(stdName, _tstoi(stdText.c_str()), false);
        break;

      case ::myodd::dynamic::Type::Integer_long_long_int:
        //  Set as an int64
        Set(stdName, _tstoi64(stdText.c_str()), false);
        break;

      default:
        Set(stdName, stdText, false);
        break;
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
    * @param tinyxml2::XMLElement& the current node we are traversing.
    * @param std::vector<std::wstring>& the current names of the parents, (so we can rebuild them in a single string).
    * @return none.
    */
    void Data::WalkElements(const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents)
    {
      long version = 0;
      const tinyxml2::XMLAttribute* attributeVersion = root.FindAttribute("version");
      if (attributeVersion != NULL)
      {
        // get the value
        auto stdVersion = attributeVersion->Value();
        version = std::atol(stdVersion);
      }

      // is it a valid version number?
      if (version < 0 || version > _CurrentVersion)
      {
        throw std::exception("The given version number is unknown.");
      }

      // now that we have a version number we can parse the 
      WalkElements(version, root, parents);
    }

    /**
     * Parse the data from the root.
     * @param const long versionNumber the data version number.
     * @param tinyxml2::XMLElement& the current node we are traversing.
     * @param std::vector<std::wstring>& the current names of the parents, (so we can rebuild them in a single string).
     */
    void Data::WalkElements(const long versionNumber, const tinyxml2::XMLElement& root, std::vector<std::wstring>& parents)
    {
      // Look for all the child nodes, they can all be valid item.
      const tinyxml2::XMLElement* pValues = root.FirstChildElement();
      while (pValues)
      {
        // we must add that new name to the list.
#ifdef UNICODE
        parents.push_back(myodd::strings::String2WString(pValues->Value()));
#else
        parents.push_back(pValues->Value());
#endif // UNICODE

        // add that element if there is anything to actually add.
        // @see WalkElement( ... )
        WalkElement( versionNumber, *pValues, parents);

        // or/and add the sibling elements.
        WalkElements(*pValues, parents);

        // remove the item we just added, no longer needed, (we just parsed it).
        // we are going back down one level.
        parents.erase(parents.end() - 1);

        // and look for the next one.
        pValues = pValues->NextSiblingElement();
      }
    }

    /**
     * Create the data holder from an XML
     * @path const std::wstring& source the xml source
     * @return bool success or not.
     */
    bool Data::FromXML(const std::wstring& source)
    {
      tinyxml2::XMLDocument doc;
      auto asciiSource = ToChar(source);
      if (tinyxml2::XMLError::XML_SUCCESS != doc.Parse(asciiSource.c_str(), asciiSource.length() ))
      {
        // something didn't work
        return false;
      }

      // then load it as per normal.
      return FromXMLDocument( doc );
    }

    /**
     * Create the data holder from and XML
     * @path const std::wstring& path the path of the xml we want to load.
     * @return bool success or not.
     */
    bool Data::FromXMLFile(const std::wstring& path)
    {
      //  make sure that this file path does exist.
      if (!myodd::files::CreateFullDirectory(path, true))
      {
        return false;
      }

      // expand the path
      std::wstring expandedPath;
      if (!myodd::files::ExpandEnvironment(path, expandedPath))
      {
        return false;
      }

      // seems good
      _path = ToChar(expandedPath);

      // try and read the doc
      tinyxml2::XMLDocument doc;
      if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile(_path.c_str()))
      {
        // something didn't work
        return false;
      }

      // we can now load it from the xml document.
      return FromXMLDocument( doc );
    }

    /**
    * Create the data holder from an XML
    * @param const tinyxml2::XMLDocument& doc the document we are working from.
    * @return bool success or not.
    */
    bool Data::FromXMLDocument(const tinyxml2::XMLDocument& doc )
    {
      // look for the root attribute, 'Config'.
      auto rootName = ToChar(_rootName);
      const tinyxml2::XMLElement* pElemConfig = doc.FirstChildElement( rootName.c_str() );
      if (nullptr == pElemConfig)
      {
        // something didn't work
        return false;
      }

      // starting from the beginning we look for sub nodes
      // once there are no more nodes we will add the value.
      // @see _addLoadElements( ... )
      std::vector<std::wstring> parents;
      WalkElements(*pElemConfig, parents);

      // the values look valid to us, bbut maybe no data was gathered.      
      return true;
    }
  } //  config
} //  myodd
