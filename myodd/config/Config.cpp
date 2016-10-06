#include "Config.h"
#include "data.h"

namespace myodd{ namespace config{

  /**
   * The one and only data container.
   * @param ConfigBase* the static data.
   */
  ::myodd::config::Data* _data = NULL;

  void set(const std::wstring& path, const myodd::dynamic::Any& any) { throw std::exception(); };

  /**
   * Try and get a value, if the value is not found, we throw.
   * @param const std::wstring& path the name of the value we are looking for.
   * @param ::myodd::dynamic::Any& defaultValue the default value to use in case it does not exist.
   * @return ::myodd::dynamic::Any the value, if it exists, the default otherwise.
   */
  ::myodd::dynamic::Any Get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) 
  { 
    if (nullptr == _data)
    {
      throw std::exception( "The configuration has not been initialised.");
    }
    return _data->Get( path, defaultValue ); 
  }

  /**
   * Check if the value has been set or not.
   * @param const std::wstring& path the path we are looking for.
   * @return bool if the path exists or not.
   */
  bool Contains(const std::wstring& path)
  {
    if (nullptr == _data)
    {
      throw std::exception("The configuration has not been initialised.");
    }
    return _data->Contains(path);
  }

  void free() { throw std::exception(); }

  /**
   * Create the data holder from and XML
   * @path const std::wstring& path the path of the xml we want to load.
   * @return bool success or not.
   */
  bool FromXMLFile(const std::wstring& path) 
  { 
    // get rid of the old one
    delete _data;

    // create a new one
    _data = new ::myodd::config::Data();

    // and load it.
    return _data->FromXMLFile(path);
  }
} //  config
} //  myodd
