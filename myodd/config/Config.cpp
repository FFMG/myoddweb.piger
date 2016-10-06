#include "Config.h"
#include "data.h"

namespace myodd{ namespace config{

  /**
   * The one and only data container.
   * @param ConfigBase* the static data.
   */
  ::myodd::config::Data* _data = NULL;

  void set(const std::wstring& path, const myodd::dynamic::Any& any) { throw std::exception(); };
  ::myodd::dynamic::Any get(const std::wstring& path) { throw std::exception(); return 0; }
  ::myodd::dynamic::Any get(const std::wstring& path, const myodd::dynamic::Any& defaultValue) { throw std::exception(); return 0; }

  bool isset(const std::wstring& path) { throw std::exception(); return false; }
  bool isset(const wchar_t* path) { throw std::exception(); return false; }

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
