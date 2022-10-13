#include "StdAfx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "LoaderManager.h"

#include <myoddinclude.h>

static const wchar_t* LOADER_LEARN = L"learn";
static const wchar_t* LOADER_LEARN_PRIVILEGED = L"learn (privileged)";
static const wchar_t* LOADER_UNLEARN = L"unlearn";

// -------------------------------------------------------------
LoaderManager::LoaderManager( const std::wstring& pluginPath ) :
  m_pluginPath( pluginPath )
{
}

// -------------------------------------------------------------
LoaderManager::~LoaderManager(void)
{
}

/**
 * Initialise the loader with the plugin.
 * @param AmPlugin* p the plugin itself.
 */
void LoaderManager::Init(AmPlugin* p  )
{
  // assume that we have no path.
  m_lpFilePath = m_thisPath = L"";

  // Add our own commands.
  wchar_t thisPath[ MAX_PATH ];
  int l = p->GetCommand( 0, _countof(thisPath), thisPath );
  if( l > 0 )
  {
    // convert this path to std::wstring as we use it 
    m_thisPath = thisPath;
    p->AddAction( LOADER_LEARN, thisPath );
    p->AddAction( LOADER_LEARN_PRIVILEGED, thisPath );
  }
  else
  {
    p->Log( AM_LOG_ERROR, L"I was unable to get the path of this command.");
    return;
  }

  // we now need to load all the items we already have in our XML
  auto stdXml = myodd::files::Join( GetPluginPath(), L"loader.xml");

  // expand the path.
  std::wstring wFilePath;
  if( myodd::files::ExpandEnvironment( stdXml, wFilePath) )
  {
    //  make sure that this file path does exist.
    if( myodd::files::CreateFullDirectory(wFilePath, true ) )
    {
      m_lpFilePath = wFilePath;
      LoadXML( p );
    }
    else
    {
      auto log = myodd::strings::Format(L"Could create the directry '%s'.", wFilePath);
      p->Log(AM_LOG_ERROR, log.c_str() );
      return;
    }
  }
  else
  {
    auto log = myodd::strings::Format(L"Could not expand the environment '%s'.", stdXml);
    p->Log(AM_LOG_ERROR, log.c_str() );
    return;
  }
}

// -------------------------------------------------------------
void LoaderManager::Exit( )
{
}

// -------------------------------------------------------------
void LoaderManager::Main(AmPlugin* p  )
{
  int l = 0;
  //  get the name of the action
  wchar_t asAction[ 1024 ];
  l = p->GetAction( _countof(asAction), asAction );
  if( l == 0 )
  {
    p->Say( L"Error : Could not get action name.", 100, 5 );
    return;
  }

  if( _wcsicmp( LOADER_LEARN, asAction ) == 0 )
  {
    Learn( p, false );
  }
  else if (_wcsicmp(LOADER_LEARN_PRIVILEGED, asAction) == 0)
  {
    Learn(p, true);
  }
  else if( _wcsnicmp( LOADER_UNLEARN, asAction, wcslen(LOADER_UNLEARN) ) == 0 )
  {
    if( wcslen(asAction) >= wcslen(LOADER_UNLEARN)+1 )
    {
      // get the command name
      // and be sure to add a char for the space
      std::wstring lpName =  asAction+wcslen(LOADER_UNLEARN) + 1;
      UnLearn( p, lpName );
    }
  }
  else
  {
    std::wstring s = _T("Error : Unknown action : ");
    s += asAction;
    p->Say( s.c_str(), 100, 5 );
    return;
  }
}

/**
 * remove one of OUR commands from the list.
 * @param AmPlugin* the plugin manager
 * @param const std::wstring& the name of the command we are removing.
 * @return none
 */
void LoaderManager::UnLearn(AmPlugin* p, const std::wstring& lpName  )
{
  if( myodd::strings::IsEmptyString(lpName) )
  {
    p->Say( L"Error : Remove as <i>... what</i>?", 100, 5 );
    return;
  }

  // Then add the command to the list.
  if( RemoveCommand( p, lpName ) )
  {
    // save it.
    SaveXML();

    std::wstring s;
    s = L"The command : <i>";
    s+= lpName;
    s+= L"</i> has been removed.";
    p->Say( s.c_str(), 100, 5 );
  }
  else
  {
    std::wstring s;
    s = L"The command : <i>";
    s+= lpName;
    s+= L"</i> could not be removed.";
    p->Say( s.c_str(), 100, 5 );
  }
}

// -------------------------------------------------------------
//  add an item to the list to open
void LoaderManager::Learn(AmPlugin* p, bool isPrivileged)
{
  // the user could have entered a multi word command.
  int count = p->GetCommandCount();
  if( 0 == count)
  {
    p->Say( L"Error : Open as <i>... what</i>?", 100, 5 );
    return;
  }

  std::wstring command;
  for( int i = 1; i <= count; ++i )
  {
    //  get the command we are adding.
    wchar_t asName[ MAX_PATH ];
    const auto l = p->GetCommand( i, _countof(asName), asName );
    if( l == 0 )
    {
      continue;
    }
    if( i > 1 )
    {
      command += L" ";
    }
    command += asName;
  }

  // now that we now the name of the command the user wants us to learn
  // we must get the name of the file currently selected.
  // this will what we execute/open when the user enters the command.
  wchar_t asPath[ MAX_PATH ];
  auto l = p->GetFile( 0, _countof(asPath), asPath );
  if( l == 0 )
  {
    l = p->GetFolder( 0, _countof(asPath), asPath );
    if( l == 0 )
    {
      l = p->GetUrl( 0, _countof(asPath), asPath );
      if( l == 0 )
      {
        p->Say( L"Error : Learn as <i>... what file</i>?", 100, 5 );
        return;
      }
    }
  }

  // create the command file.
  std::wstring fileName;
  if( !SaveLUAFile( fileName, command, asPath, isPrivileged) )
  {
    p->Say( L"Error : Could not create command file. Do you have the right permissions?", 100, 5 );
    return;
  }

  // does that command already exist???
  bool alreadyExists = m_openAs.find(myodd::strings::lower(command)) != m_openAs.end();

  // Then add the command to the list.
  // note that the path that we are adding is our own file.
  if( AddCommand( p, command.c_str(), fileName.c_str() ) )
  {
    // save the XML now!
    SaveXML();

    std::wstring s;
    s = L"The command : <i>";
    s+= command;
    if (alreadyExists)
    {
      s += L"</i> has been updated.";
    }
    else
    {
      s += L"</i> has been added.";
    }
    p->Say( s.c_str(), 100, 5 );
  }
  else
  {
    std::wstring s;
    s = L"The command : <i>";
    s+= command;
    s+= L"</i> could not be added.";
    p->Say( s.c_str(), 100, 5 );
  }
}

/**
 * Create the command file as a LUA file.
 * By default we will simply call the file that is pass to us and hope the OS
 * Will be better able to handle it.
 * @param std::wstring& if successful this is the file name we created.
 * @param const std::wstring& the command name the user wants.
 * @param const std::wstring& the app/file we want to 'launch'.
 * @return bool success or not.
 */
bool LoaderManager::SaveLUAFile
(
  std::wstring& fileName,
  const std::wstring& command,
  const std::wstring& appPath,
  bool isPrivileged
)
{
  // create the filename.
  auto luaFileName = command;
  myodd::files::AddExtension( luaFileName, L".lua", true);

  // make sure it is valid.
  myodd::files::CleanFileName(luaFileName);

  // then join the plugin path to the lua filename
  auto luaFilePath = myodd::files::Join( GetPluginPath(), luaFileName);

  // we now have a command and a file path
  // we must create the lua file that will contain this request.
  //
  // the LUA file cannot be UNICODE as it will contain invalid characters.
  FILE *stream;
  errno_t err;
  std::wstring exLuaFilePath;
  myodd::files::ExpandEnvironment(luaFilePath, exLuaFilePath);
  if ((err = _wfopen_s(&stream, exLuaFilePath.c_str(), L"wb")) != 0)
  {
    return false;
  }

  //
  // build the comments at the top of the file.
  std::wstring sData;
  sData += L"--\n";
  sData += L"-- Loaded version 0.3\n";
  sData += L"-- am_execute( \"command/exe/shortcut\", \"[commandline arguments]\", [isPrivileged=false]);\n";
  sData += L"-- remove this command with 'unlearn ...'\n";
  sData += L"--\n";

  //
  // add the execute command itself.
  sData += L"am_execute( ";
    
  // unexpand the app path.
  // this is just cosmetic but also allows the user to copy their command
  // files from one machine to another.
  std::wstring unAppPath = appPath;
  myodd::files::UnExpandEnvironment(unAppPath, unAppPath);

  //  remember, we cannot add spaces or anything...
  sData += L"[[";
  sData += unAppPath.c_str();
  sData += L"]], \"\"";
  if (true == isPrivileged)
  {
    sData += L", true";
  }
  else
  {
     sData += L", false";
  }
  sData += L");\n";

  //
  // write the data.
  fwrite( sData.c_str(), sizeof(wchar_t), sData.length(), stream );

  // we created the file.
  fclose( stream );

  //
  //  this is the file name we are using.
  fileName = luaFilePath;

  return true;
}

/**
 * Find a OPENAS_NAMES iterator 
 * The search is not case sensitive.
 * @param const std::wstring& the name we are looking for.
 * @return OPENAS_NAMES::const_iterator end()|the iterator we are looking for.
 */
LoaderManager::OPENAS_NAMES::const_iterator LoaderManager::Find(const std::wstring& name ) const
{
  std::wstring lowerName = myodd::strings::lower( name );
  return m_openAs.find( lowerName.c_str() );
}

// -------------------------------------------------------------
bool LoaderManager::RemoveCommand(AmPlugin* p, const std::wstring& name )
{
  auto it = Find( name );
  if( m_openAs.end() == it )
  {
    //  not one of ours.
    return false;
  }

  //  remove action in list.
  return RemoveActionIfInList(p, name, true );
}

/**
 * Remove an action if it is in _our_ list.
 * We ill return false if we could _not_ remove it.
 * We will return true, if it is not in our list or if we removed it.
 * @param const std::wstring& lowerName the command to remove.
 * @param bool deleteFileIfExists if we wish to remove a file or not.
 * @return bool success or not.
 */
bool LoaderManager::RemoveActionIfInList(AmPlugin* p, const std::wstring& lowerName, bool deleteFileIfExists)
{
  // lowercase TCHAR
  const wchar_t* cpLowerName = lowerName.c_str();

  // we now need to check if it exists already
  // if it does, then we will able to replace it.
  auto itCommand = m_openAs.find(lowerName);
  if (itCommand == m_openAs.end())
  {
    // it does not exist.
    // so ther eis nothing to remove really.
    return true;
  }

  //  this is one of our item, so we muse remove it.
  if (!p->RemoveAction(cpLowerName, itCommand->second.c_str()))
  {
    // we could not remove it.
    auto log = myodd::strings::Format(L"I was unable to remove the command '%s' - '%s'", cpLowerName, itCommand->second.c_str());
    p->Log(AM_LOG_ERROR, log.c_str() );
    return false;
  }

  //  delete the file if we want to.
  if (deleteFileIfExists == true)
  {
    myodd::files::DeleteFile(itCommand->second);
  }

  // log it  
  auto log = myodd::strings::Format(L"Removed command '%s' - '%s'", cpLowerName, itCommand->second.c_str());
  p->Log(AM_LOG_ERROR, log.c_str() );

  // unlearn the unlearn function as well.
  std::wstring sUnLearn = GetUnLearnCommand(lowerName);
  p->RemoveAction(sUnLearn.c_str(), GetThisPath().c_str());

  // and now remove it from our own list.
  m_openAs.erase(itCommand);

  // success
  return true;
}

/**
 * Add a command to the list of commands.
 * @param AmPlugin* the plugin manager
 * @param const std::wstring& the name of the coammnd
 * @param const std::wstring& the path of the command we want to execute.
 * @return bool success or not.
 */
bool LoaderManager::AddCommand(AmPlugin* p, const std::wstring& name, const std::wstring& path )
{
  std::wstring lowerName = myodd::strings::lower( name );
  const wchar_t* cpLowerName = lowerName.c_str();

  // we now need to check if it exists already
  // if it does, then we will able to replace it.
  if (!RemoveActionIfInList( p, lowerName, false ))
  {
    //  could not remove it.
    auto log = myodd::strings::Format(L"I was unable to remove the command '%s' - '%s' to (re)add it.", name.c_str(), path.c_str());
    p->Log(AM_LOG_ERROR, log.c_str() );
    return false;
  }

  // (re)add it to the list with the new path.
  m_openAs[lowerName] = path;

  // and to the action monitor.
  if( !p->AddAction(cpLowerName, path.c_str() ) )
  {
    auto log = myodd::strings::Format(L"I was unable to add the command '%s' - '%s'", name.c_str(), path.c_str());
    p->Log(AM_LOG_ERROR, log.c_str() );
    return false;
  }

  // add the unlearn function as well so the user can remove this command.
  std::wstring sUnLearn = GetUnLearnCommand( lowerName );
  p->AddAction( sUnLearn.c_str(), GetThisPath().c_str() );

  // it seems to have worked.
  auto log = myodd::strings::Format(L"Added command '%s' - '%s'", name.c_str(), path.c_str());
  p->Log(AM_LOG_SUCCES, log.c_str() );
  return true;
}

/**
 * @param const std::wstring& lowerName get the unlearn command name.
 * @return std::wstring the UNLEARN command name.
 */
std::wstring LoaderManager::GetUnLearnCommand(const std::wstring& lowerName)
{
  std::wstring sUnLearn = LOADER_UNLEARN;
  sUnLearn += L" ";
  sUnLearn += lowerName;
  return sUnLearn;
}

// -------------------------------------------------------------
bool LoaderManager::LoadXML(AmPlugin* p )
{
  //  simply go around all the values
  const auto& sFilePath = myodd::strings::WString2String(m_lpFilePath);
  tinyxml2::XMLDocument doc;
  if (tinyxml2::XMLError::XML_SUCCESS != doc.LoadFile(sFilePath.c_str() ))
  {
    return false;
  }

  // get the files document.
  tinyxml2::XMLElement* pElemLoadManager = doc.FirstChildElement( "LoadManager");
  if( !pElemLoadManager )
  {
    return false;
  }

  // get the files document.
  tinyxml2::XMLElement* pElemLoadCommands = pElemLoadManager->FirstChildElement( "Commands" );
  if( pElemLoadCommands )
  {
    tinyxml2::XMLElement* pValues = pElemLoadCommands->FirstChildElement( "Command" );
    while( pValues )
    {
      tinyxml2::XMLElement* pName = pValues->FirstChildElement( "Name");
      tinyxml2::XMLElement* pPath = pValues->FirstChildElement( "Path" );

      // we have to have both values.
      if( pName && pPath )
      {
        //  add that name to the list.
        std::wstring name = myodd::strings::String2WString( pName->GetText() );
        std::wstring path = myodd::strings::String2WString( pPath->GetText() );

        if( myodd::files::FileExists( path ) )
        {
          // and add it to the list of commands.
          AddCommand( p, name.c_str(), path.c_str());
        }
      }

      // Open the Open as items.
      pValues = pValues->NextSiblingElement();
    }
  }
  return true;
}

// -------------------------------------------------------------
bool LoaderManager::SaveXML() const
{
  // create a new plan
  tinyxml2::XMLDocument doc;
  doc.SetBOM(true);

  // save the basic items
  // the declaration
  tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
  decl->SetValue("xml version=\"1.0\" encoding=\"UTF-8\"");
  doc.InsertEndChild(decl);

  tinyxml2::XMLElement* pElemLoadManager = doc.NewElement( "LoadManager" );
  doc.InsertEndChild( pElemLoadManager );

  tinyxml2::XMLElement* pElemLoadCommands = doc.NewElement( "Commands" );
  pElemLoadManager->InsertEndChild( pElemLoadCommands );

  for( OPENAS_NAMES::const_iterator it = m_openAs.begin(); it != m_openAs.end(); ++it )
  {
    tinyxml2::XMLElement* pValue = doc.NewElement( "Command" );
    pElemLoadCommands->InsertEndChild( pValue );

    tinyxml2::XMLElement* pName = doc.NewElement( "Name" );
    const auto& sName = myodd::strings::WString2String(it->first.c_str());
    pName->SetText( sName.c_str() );

    tinyxml2::XMLElement* pPath = doc.NewElement("Path" );
    const auto& sPath = myodd::strings::WString2String(it->second.c_str());
    pPath->SetText( sPath.c_str());

    pValue->InsertEndChild( pName );
    pValue->InsertEndChild( pPath );
  }

  // save the file.
  const auto& sFilePath = myodd::strings::WString2String(m_lpFilePath);
  doc.SaveFile( sFilePath.c_str() );

  return true;
}