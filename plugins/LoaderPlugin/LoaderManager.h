#pragma once

class LoaderManager
{
public:
  LoaderManager( const std::wstring& pluginPath );
  ~LoaderManager(void);

  void Init( amplugin* p  );
  void Exit( );
  void Main( amplugin* p  );

protected:
  void Learn( amplugin* p  );
  void UnLearn( amplugin* p, LPCWSTR lpName );

  bool RemoveCommand( amplugin* p, LPCWSTR name );
  bool AddCommand( amplugin* p, LPCWSTR name, LPCWSTR path );

  typedef std::map< std::wstring, std::wstring > OPENAS_NAMES;
  OPENAS_NAMES  m_openAs;

  OPENAS_NAMES::const_iterator Find( LPCWSTR name );

protected:
  // the XML file.
  std::wstring m_lpFilePath;
  std::wstring m_pluginPath;
  std::wstring m_thisPath;

  const std::wstring& GetPluginPath() const{
    return m_pluginPath;
  }
  const std::wstring& GetThisPath() const{
    return m_thisPath;
  }

  bool LoadXML( amplugin* p  );
  bool SaveXML();

  bool SaveLUAFile( std::wstring& fileName, 
                    const std::wstring command,
                    const std::wstring appPath
                  );
};
