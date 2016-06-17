#pragma once

class LoaderManager
{
public:
  LoaderManager( const std::string& pluginPath );
  ~LoaderManager(void);

  void Init( amplugin* p  );
  void Exit( );
  void Main( amplugin* p  );

protected:
  void Learn( amplugin* p, bool isPrivileged );
  void UnLearn( amplugin* p, LPCWSTR lpName );

  bool RemoveCommand( amplugin* p, LPCWSTR name );
  bool AddCommand( amplugin* p, LPCWSTR name, LPCWSTR path );

  typedef std::map< std::wstring, std::wstring > OPENAS_NAMES;
  OPENAS_NAMES  m_openAs;

  OPENAS_NAMES::const_iterator Find( LPCWSTR name );

protected:
  // the XML file.
  std::string m_lpFilePath;
  std::string m_pluginPath;
  std::string m_thisPath;

  const std::string& GetPluginPath() const{
    return m_pluginPath;
  }
  const std::string& GetThisPath() const{
    return m_thisPath;
  }

  bool LoadXML( amplugin* p  );
  bool SaveXML() const;

  bool SaveLUAFile( std::wstring& fileName, 
                    const std::wstring command,
                    const std::wstring appPath,
                    bool isPrivileged
                  );

  bool RemoveActionIfInList(amplugin* p, const std::wstring& lowerName, bool deleteFileIfExists );
  static std::wstring GetUnLearnCommand( const std::wstring& lowerName);
};
