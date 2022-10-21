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
    Data::Data(const std::wstring& rootName) :
      _path(""),
      _dirty(false),
      _rootName(rootName)
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
        if (IsValidDataType(static_cast<::myodd::dynamic::Type>(givenType)))
        {
          // no conversion needed
          return static_cast<::myodd::dynamic::Type>(givenType);
        }

        // not valid...
        throw std::runtime_error(myodd::strings::FormatterA() << "Unknown data type :" << givenType);
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
    * Check if the data set is dirty or not.
    * @param const std::wstring& path the path we are looking for.
    * @return bool if the value is a path or not.
    */
    bool Data::IsDirty() const
    {
      return _dirty;
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
      if (nullptr == data)
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
      // cleanup the name
      auto lname = SafeName(name);

      //  make sure that it is a valid type 
      if (!IsValidDataType(value.Type()))
      {
        throw std::runtime_error("The given data type cannot be saved in the config.");
      }

      //
      // lock the values
      myodd::threads::AutoLock lock(*this);

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
        ::myodd::dynamic::Type actualType = ::myodd::dynamic::Integer_long_long_int;
        if (::myodd::dynamic::is_type_character(value.Type()))
        {
          actualType = ::myodd::dynamic::Character_wchar_t;
        }
        if (::myodd::dynamic::is_type_floating(value.Type()))
        {
          actualType = ::myodd::dynamic::Floating_point_long_double;
        }
        if (::myodd::dynamic::is_type_integer(value.Type()))
        {
          actualType = ::myodd::dynamic::Integer_long_long_int;
        }
        if (::myodd::dynamic::is_type_boolean( value.Type() ))
        {
          actualType = ::myodd::dynamic::Boolean_bool;
        }

        // if all the values are the same then there is nothing 
        // more for us to do here, we might as well move on.
        // the type is saved in the XML, so we need to make sure that the type
        // is the same as well.
        if (data->_temp == isTemp && data->_value == value && data->_value.Type() == actualType)
        {
          return;
        }

        //  something has changed.
        _dirty = true;
      }

      // set the value, if we made it here, it is 'dirty'
      _values[lname.c_str()] = { value, isTemp, true };
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
      const auto sep = L"\\";

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
      auto ltype = ConvertToDataType(versionNumber, std::atol(stdType));

      // depending on the type.
      if (::myodd::dynamic::is_type_character(ltype))
      {
        //  Set as an string
        Set(stdName, stdText, false);
        return;
      }

      if (::myodd::dynamic::is_type_floating(ltype))
      {
        //  Set as an string
        Set(stdName, std::stold(stdText), false);
        return;
      }

      if (::myodd::dynamic::is_type_integer(ltype))
      {
        Set(stdName, std::stoll(stdText), false);
        return;
      }

      if (::myodd::dynamic::is_type_boolean(ltype))
      {
        Set(stdName, (std::stoll(stdText) != 0), false);
        return;
      }

      //  this should never be reached
      // @see ConvertToDataType() we should have thrown here.
      throw std::runtime_error(::myodd::strings::FormatterA() << "Unknown data type : " << ltype);
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
        WalkElement(versionNumber, *pValues, parents);

        // or/and add the sibling elements.
        WalkElements(versionNumber, *pValues, parents);

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
    bool Data::LoadXml(const std::wstring& source)
    {
      tinyxml2::XMLDocument doc;
      auto asciiSource = ToChar(source);
      if (tinyxml2::XMLError::XML_SUCCESS != doc.Parse(asciiSource.c_str(), asciiSource.length()))
      {
        // something didn't work
        return false;
      }

      // then load it as per normal.
      return LoadXmlDocument(doc);
    }

    /**
     * Create the data holder from and XML
     * @path const std::wstring& path the path of the xml we want to load.
     * @return bool success or not.
     */
    bool Data::LoadXmlFile(const std::wstring& path)
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
      return LoadXmlDocument(doc);
    }

    /**
    * Create the data holder from an XML
    * @param const tinyxml2::XMLDocument& doc the document we are working from.
    * @return bool success or not.
    */
    bool Data::LoadXmlDocument(const tinyxml2::XMLDocument& doc)
    {
      // look for the root attribute, 'Config'.
      auto rootName = ToChar(_rootName);
      const tinyxml2::XMLElement* pElemConfig = doc.FirstChildElement(rootName.c_str());
      if (nullptr == pElemConfig)
      {
        // something didn't work
        return false;
      }

      // remove all the elements.
      // that might have been added/found already
      _values.clear();

      // starting from the beginning we look for sub nodes
      // once there are no more nodes we will add the value.
      // @see _addLoadElements( ... )
      std::vector<std::wstring> parents;
      WalkElements(*pElemConfig, parents);

      // we cannot say that the data is dirty.
      _dirty = false;

      // the values look valid to us, but maybe no data was gathered.      
      return true;
    }

    /**
    * Save the XML to file if we have a path.
    * @return success or not.
    */
    bool Data::SaveFileXml() const
    {
      if ( ::myodd::strings::IsEmptyString(_path))
      {
        return false;
      }

      // try and save it with the path we have.
      return SaveFileXml(_path);
    }

    /**
     * Given an array of variables we create/look for various node.
     * So if the user creates a node "something/else" we will create the nodes "something->else"
     * @param tinyxml2::XMLDocument& doc the document we are working in.
     * @param tinyxml2::XMLElement& the current node we are adding elements to.
     * @param const std::vector<std::wstring>& the array of elements we are traversing.
     * @return tinyxml2::XMLElement* the last element that we will be adding data to.
     */
    tinyxml2::XMLElement* Data::GetSaveElement (tinyxml2::XMLDocument& doc, tinyxml2::XMLElement& root, const std::vector<std::wstring>& parents ) const
    {
      //  sanity check
      if (parents.size() == 0)
      {
        return nullptr;
      }

      // get the first parent in the list as a starting point.
      auto parent = ToChar(parents[0]);

      // Look for that element
      // if it does not exist then we must add it to the parent/root items
      tinyxml2::XMLElement* pElement = root.FirstChildElement(parent.c_str());
      if (nullptr == pElement)
      {
        // we could not find it, so we must add it to the list.
        pElement = doc.NewElement(parent.c_str());

        // add this node to the parent
        root.InsertEndChild(pElement);
      }

      // If this was the last item then 
      // either the created item or the item that was found needs to be added.
      if (parents.size() == 1)
      {
        return pElement;
      }

      // this was not the last item, so we need to remove the string from the list
      // and continue going down the list.
      std::vector<std::wstring> tmpParents;
      for ( auto it = parents.begin() + 1; it != parents.end(); it++)
      {
        tmpParents.push_back(*it);
      }

      // using this temp value we now look for the next item.
      return GetSaveElement(doc, *pElement, tmpParents);
    }

    /**
    * Save the XML to file using the given path.
    * const std::wstring& path the path we are saving to.
    * @return success or not.
    */
    bool Data::SaveFileXml(const std::string& path) const
    {
      // create a new document
      tinyxml2::XMLDocument doc;

      // save to it.
      SaveDocument(doc);

      // set the BOM
      doc.SetBOM(true);

      // save the file
      doc.SaveFile(path.c_str());

      // all done
      return true;
    }

    /**
    * Save the data to an xml tring
    * @return std::wstring the formated xml data.
    */
    std::wstring Data::SaveXml() const
    {
      // create a new document
      tinyxml2::XMLDocument doc;

      // save to it.
      SaveDocument(doc);

      tinyxml2::XMLPrinter printer;
      doc.Print(&printer);
      auto src = printer.CStr();

      // convert it to a string
      // and return it.
      return ::myodd::strings::String2WString(src);
    }
    
    /**
     * Save the XML to file using the given path.
     * @preturn const tinyxml2::XMLDocument& doc the document.
     */
    void Data::SaveDocument(tinyxml2::XMLDocument& doc) const
    {
      // reset whatever was set.
      doc.Clear();

      // create the basic header definitons
      tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
      decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\"");
      doc.InsertEndChild(decl);

      // create the root version.
      auto rootName = ToChar(_rootName);
      auto pElemConfig = doc.NewElement( rootName.c_str() );
      pElemConfig->SetAttribute("version", _CurrentVersion );

      doc.LinkEndChild(pElemConfig);

      //  and then we go around adding values to the values node.
      const wchar_t sep = L'\\';
      for( auto it = _values.begin(); it != _values.end(); ++it )
      {
        const auto& data = it->second;

        // first check if the data is temp.
        // if it is temp then the user does not want to save it.
        if( data._temp )
        {
          continue;
        }

        // the full name
        const auto& name = it->first;

        // get the parentss.
        std::vector<std::wstring> parents;
        strings::Explode(parents, name, sep);

        // and get the element we will save it to.
        auto pValue = GetSaveElement( doc, *pElemConfig, parents);

        // then the value we will save.
        auto any = data._value;
        pValue->SetAttribute("type", any.Type());

        if (::myodd::dynamic::is_type_character(any.Type()))
        {
          pValue->SetText( (const char*)any );
        }
        if (::myodd::dynamic::is_type_floating(any.Type()))
        {
          pValue->SetText(myodd::strings::WString2String(myodd::strings::ToString((long double)any)).c_str());
        }
        if (::myodd::dynamic::is_type_integer(any.Type()))
        {
          pValue->SetText(myodd::strings::WString2String(myodd::strings::ToString((long long int )any)).c_str());
        }
        if (::myodd::dynamic::is_type_boolean(any.Type()))
        {
          pValue->SetText((bool)any ? "1" : "0");
        }
      }
    }

    /**
     * Check if the given type is one that can be used to save the data.
     * @param ::myodd::dynamic::Type type the type we want to set/load
     * @return boolean if the type can be used here.
     */
    bool Data::IsValidDataType(const ::myodd::dynamic::Type& type)
    {
      if (::myodd::dynamic::is_type_character(type))
      {
        return true;
      }
      if (::myodd::dynamic::is_type_floating(type))
      {
        return true;
      }
      if (::myodd::dynamic::is_type_integer(type))
      {
        return true;
      }
      if (::myodd::dynamic::is_type_boolean(type))
      {
        return true;
      }

      // not a valid type
      return false;
    }
 } //  config
} //  myodd
