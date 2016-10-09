#pragma once

#include "../string/string.h"
#include "../string/formatter.h"
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
     * Look for a certain data structure, return nullptr if we do not find it.
     * @param const std::wstring& name the value we are looking for.
     * @return DataStruct* the data struct we found.
     */
    const Data::DataStruct* Data::GetRaw(const std::wstring& name) const
    {
      //
      // lock the values
      // NB: we are returning a pointer, so locking
      //     should be also done by the parent function calling this.
      //     otherwise you could very well loose the data as soon as we 
      //     releast the lock.
      myodd::threads::AutoLock lock(*this);

      // look got it.
      auto it = _values.find(name);
      if (it == _values.end())
      {
        return nullptr;
      }
      return &(it->second);
    }

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
     * Get the data type given the version number of the config
     * Older xmls have different data types.
     * We will throw if the data type is unknown.
     * @param const long version the xml file version number
     * @param const long givenType the type we want to convert.
     * @return ::myodd::dynamic::Type the converted data type.
     */
    ::myodd::dynamic::Type Data::ConvertToDataType(const long version, const long givenType)
    {
      //  current version.
      if (version == _CurrentVersion)
      {
        switch ( givenType)
        {
        case ::myodd::dynamic::Type::Character_wchar_t:
        case ::myodd::dynamic::Type::Integer_long_int:
        case ::myodd::dynamic::Type::Floating_point_double:
        case ::myodd::dynamic::Type::Integer_int:
        case ::myodd::dynamic::Type::Integer_long_long_int:
          // no conversion needed
          return static_cast<::myodd::dynamic::Type>(givenType);

        default:
          throw std::runtime_error( myodd::strings::FormatterA() << "Unknown data type :" << givenType );
        }
      }

      // the old way
      switch (givenType)
      {
      case 0x1: // type_string = 0x01
        return ::myodd::dynamic::Type::Character_wchar_t;

      case 0x2: //  type_long = 0x02
        return ::myodd::dynamic::Type::Integer_long_int;

      case 0x4: //  type_double = 0x04
        return ::myodd::dynamic::Type::Floating_point_double;

      case 0x8: //  type_int = 0x08
        return ::myodd::dynamic::Type::Integer_int;

      case 0x10: // type_int64  = 0x10
        return ::myodd::dynamic::Type::Integer_long_long_int;

      default:
        throw std::runtime_error(myodd::strings::FormatterA() << "Unknown data type :" << givenType);
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
      return (GetRaw(lpath.c_str()) != nullptr);
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
      auto data = GetRaw(lpath.c_str());
      if (nullptr == data )
      {
        return false;
      }

      // return if it is temp or not.
      return data->_temp;
    }
        
    /**
    * Try and get a value, if the value is not found, we throw.
    * @param const std::wstring& path the name of the value we are looking for.
    * @return ::myodd::dynamic::Any the value, if it exists.
    */
    const ::myodd::dynamic::Any& Data::Get(const std::wstring& path) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      //
      // does this value exist?
      auto data = GetRaw(lpath.c_str());
      if (nullptr == data)
      {
        throw std::runtime_error("Unknown path");
      }

      // get the value.
      return data->_value;
    }

    /**
    * Try and get a value, if the value is not found, we throw.
    * @param const std::wstring& path the name of the value we are looking for.
    * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
    * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
    */
    const ::myodd::dynamic::Any& Data::Get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) const
    {
      // cleanup the path
      auto lpath = SafeName(path);

      //
      // does this value exist?
      auto data = GetRaw(lpath.c_str());
      if (nullptr == data)
      {
        return defaultValue;
      }

      // get the value.
      return data->_value;
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
      auto data = GetRaw(lname.c_str());
      if (nullptr == data)
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
        if (data->_temp && !isTemp)
        {
          // it was temp, but it no longer is temp
          // so it is now dirty.
          _dirty = true;
        }

        // if all the values are the same then there is nothing 
        // more for us to do here, we might as well move on.
        if (data->_temp == isTemp && data->_value == value )
        {
          return;
        }
      }

      // set the value, if we made it here, it is 'dirty'
      _values[lname.c_str()] =  { value, isTemp, true };
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
      auto stdType = attributeType->Value();
#ifdef UNICODE
      auto stdRoot = myodd::strings::String2WString(root.Value());
      auto stdText = myodd::strings::String2WString(root.GetText());
#else
      auto stdRoot = root.Value();
      auto stdText = root.GetText();
#endif // UNICODE    

      //  create the name of the object that we will be adding.
      std::wstring stdName = myodd::strings::implode(parents, sep);

      // convert the type to a valid data type
      // depending on the version number, this could be differemt.
      auto ltype = ConvertToDataType( versionNumber, std::atol(stdType) );

      // depending on the type.
      switch (ltype)
      {
      case ::myodd::dynamic::Type::Character_wchar_t:
        //  Set as an string
        Set(stdName, stdText, false);
        break;

      case ::myodd::dynamic::Type::Integer_long_int:
        //  Set as an long
        Set(stdName, std::stol(stdText), false );
        break;

      case ::myodd::dynamic::Type::Floating_point_double:
        //  Set as an double
        Set(stdName, std::stod(stdText), false);
        break;

      case ::myodd::dynamic::Type::Integer_int:
        //  Set as an int
        Set(stdName, std::stoi(stdText), false);
        break;

      case ::myodd::dynamic::Type::Integer_long_long_int:
        //  Set as an long long
        Set(stdName, std::stoll(stdText), false);
        break;

      default:
        //  this should never be reached
        // @see ConvertToDataType() we should have thrown here.
        throw std::runtime_error( ::myodd::strings::FormatterA() << "Unknown data type : " << ltype );
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

        // convert it to a long
        version = std::atol(stdVersion);
      }

      // is it a valid version number?
      if (version < 0 || version > _CurrentVersion)
      {
        throw std::runtime_error("The given version number is unknown.");
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
