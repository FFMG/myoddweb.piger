#include "StdAfx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "LoaderManager.h"

#include <myoddinclude.h>

static LPCWSTR LOADER_LEARN = L"learn";
static LPCWSTR LOADER_UNLEARN = L"unlearn";

// -------------------------------------------------------------
LoaderManager::LoaderManager( const std::wstring& pluginPath ) :
  m_pluginPath( pluginPath )
{
}

// -------------------------------------------------------------
LoaderManager::~LoaderManager(void)
{
}

// -------------------------------------------------------------
void LoaderManager::Init( amplugin* p  )
{
  // Add our own commands.
  WCHAR thisPath[ MAX_PATH ];
  int l = p->GetCommand( 0, _countof(thisPath), thisPath );
  if( l > 0 )
  {
    m_thisPath = thisPath;
    p->AddAction( LOADER_LEARN, thisPath );
  }

  // we now need to load all the items we already have in our XML
  std::wstring stdXml;
  myodd::files::Join( stdXml, GetPluginPath(), L"loader.xml" );
  if( myodd::files::ExpandEnvironment( stdXml, m_lpFilePath ) )
  {
    //  make sure that this file path does exist.
    if( myodd::files::CreateFullDirectory( m_lpFilePath, true ) )
    {
      LoadXML( p );
    }
  }
}

// -------------------------------------------------------------
void LoaderManager::Exit( )
{
}

// -------------------------------------------------------------
void LoaderManager::Main( amplugin* p  )
{
  int l = 0;
  //  get the name of the action
  WCHAR asAction[ 1024 ];
  l = p->GetAction( _countof(asAction), asAction );
  if( l == 0 )
  {
    p->Say( L"Error : Could not get action name.", 100, 5 );
    return;
  }

  if( _wcsicmp( LOADER_LEARN, asAction ) == 0 )
  {
    Learn( p );
  }
  else if( _wcsnicmp( LOADER_UNLEARN, asAction, wcslen(LOADER_UNLEARN) ) == 0 )
  {
    if( wcslen(asAction) >= wcslen(LOADER_UNLEARN)+1 )
    {
      // get the command name
      // and be sure to add a char for the space
      LPCWSTR lpName =  asAction+wcslen(LOADER_UNLEARN) + 1;
      UnLearn( p, wcslen(lpName) == 0 ? NULL : lpName );
    }
  }
  else
  {
    STD_TSTRING s = _T("Error : Unknown action : ");
    s += asAction;
    p->Say( s.c_str(), 100, 5 );
    return;
  }
}

/**
 * remove one of OUR commands from the list.
 * @param amplugin* the plugin manager
 * @param LPCWSTR the name of the command we are removing.
 * @return none
 */
void LoaderManager::UnLearn( amplugin* p, LPCWSTR lpName  )
{
  if( NULL == lpName )
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
void LoaderManager::Learn( amplugin* p  )
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
    WCHAR asName[ MAX_PATH ];
    int l = p->GetCommand( i, _countof(asName), asName );
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
  WCHAR asPath[ MAX_PATH ];
  int l = p->GetFile( 0, _countof(asPath), asPath );
  if( l == 0 )
  {
    l = p->GetFolder( 0, _countof(asPath), asPath );
    if( l == 0 )
    {
      l = p->GetURL( 0, _countof(asPath), asPath );
      if( l == 0 )
      {
        p->Say( L"Error : Learn as <i>... what file</i>?", 100, 5 );
        return;
      }
    }
  }

  // create the command file.
  std::wstring fileName;
  if( !SaveLUAFile( fileName, command, asPath, false ) )
  {
    p->Say( L"Error : Could not create command file. Do you have the right permissions?", 100, 5 );
    return;
  }

  // Then add the command to the list.
  // note that the path that we are adding is our own file.
  if( AddCommand( p, command.c_str(), fileName.c_str() ) )
  {
    // save the XML now!
    SaveXML();

    std::wstring s;
    s = L"The command : <i>";
    s+= command;
    s+= L"</i> has been added.";
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
  const std::wstring command,
  const std::wstring appPath,
  bool isPrivileged
)
{
  // create the filename.
  std::wstring luaFileName = command;
  luaFileName += L".lua";

  // make sure it is valid.
  myodd::files::CleanFileName(luaFileName);
  std::wstring luaFilePath;
  myodd::files::Join(luaFilePath, GetPluginPath(), luaFileName);

  USES_CONVERSION;

  // we now have a command and a file path
  // we must create the lua file that will contain this request.
  //
  // the LUA file cannot be UNICODE as it will contain invalid characters.
  FILE *stream;
  errno_t err;
  std::wstring exLuaFilePath;
  myodd::files::ExpandEnvironment(luaFilePath, exLuaFilePath);
  if ((err = fopen_s(&stream, T_T2A(exLuaFilePath.c_str()), "wb")) != 0)
  {
    return false;
  }

  //
  // build the comments at the top of the file.
  std::string sData;
  sData += "--\n";
  sData += "-- Loaded version 0.2\n";
  sData += "-- am_execute( \"command/exe/shortcut\", \"[commandline arguments]\", [isPrivileged=false]);\n";
  sData += "-- remove this command with 'unlearn ...'\n";
  sData += "--\n";

  //
  // add the execute command itself.
  sData += "am_execute( \"";

  // unexpand the app path.
  // this is just cosmetic but also allows the user to copy their command
  // files from one machine to another.
  std::wstring unAppPath = appPath;
  myodd::files::UnExpandEnvironment(unAppPath, unAppPath);
  sData += T_T2A(myodd::strings::replace(unAppPath, L"\\", L"\\\\").c_str());
  sData += "\", \"\"";
  if (true == isPrivileged)
  {
    sData += ", true";
  }
  else
  {
     sData += ", false";
  }
  sData += ");\n";

  //
  // write the data.
  fwrite( sData.c_str(), sizeof(CHAR), sData.length(), stream );

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
 * @param LPCWSTR the name we are looking for.
 * @return OPENAS_NAMES::const_iterator end()|the iterator we are looking for.
 */
LoaderManager::OPENAS_NAMES::const_iterator LoaderManager::Find( LPCWSTR name )
{
  std::wstring lowerName = myodd::strings::lower( name );
  OPENAS_NAMES::const_iterator it = m_openAs.find( lowerName.c_str() );
  return it;
}

// -------------------------------------------------------------
bool LoaderManager::RemoveCommand( amplugin* p, LPCWSTR name )
{
  OPENAS_NAMES::const_iterator it = Find( name );
  if( m_openAs.end() == it )
  {
    //  not one of ours.
    return false;
  }

  // remove it from the list and from action monitor.
  p->RemoveAction( name, it->second.c_str() );
  
  // and then delete the actual file itself as that command no longer exists.
  myodd::files::DeleteFile( it->second );

  m_openAs.erase( it );
  
  // the remove action could return false if it has already been removed.
  return true;
}

/**
 * Add a command to the list of commands.
 * @param amplugin* the plugin manager
 * @param LPCWSTR the name of the coammnd
 * @param LPCWSTR the path of the command we want to execute.
 * @return bool success or not.
 */
bool LoaderManager::AddCommand( amplugin* p, LPCWSTR name, LPCWSTR path )
{
  std::wstring lowerName = myodd::strings::lower( name );

  // add it to the list
  // if it already exists then nothing bad will happen.
  m_openAs[ lowerName.c_str() ] = path;

  // and to the action monitor.
  if( !p->AddAction( lowerName.c_str(), path ) )
  {
    return false;
  }

  // add the unlearn function as well so the user can remove this command.
  std::wstring sUnLearn = LOADER_UNLEARN;
  sUnLearn += L" ";
  sUnLearn += lowerName.c_str();
  p->AddAction( sUnLearn.c_str(), GetThisPath().c_str() );
  return true;
}

// -------------------------------------------------------------
bool LoaderManager::LoadXML( amplugin* p )
{
  //  simply go around all the values
  TiXmlDocument doc( m_lpFilePath.c_str() );
  if (!doc.LoadFile() )
  {
    return false;
  }

  // get the files document.
  TiXmlElement* pElemLoadManager = doc.FirstChildElement( _T("LoadManager") );
  if( !pElemLoadManager )
  {
    return false;
  }

  // get the files document.
  TiXmlElement* pElemLoadCommands = pElemLoadManager->FirstChildElement( _T("Commands") );
  if( pElemLoadCommands )
  {
    TiXmlElement* pValues = pElemLoadCommands->FirstChildElement( _T("Command") );
    while( pValues )
    {
      TiXmlElement* pName = pValues->FirstChildElement( _T("Name") );
      TiXmlElement* pPath = pValues->FirstChildElement( _T("Path") );

      // we have to have both values.
      if( pName && pPath )
      {
        //  add that name to the list.
        LPCTSTR name = pName->GetText();
        LPCTSTR path = pPath->GetText();

        if( myodd::files::FileExists( path ) )
        {
          // and add it to the list of commands.
          AddCommand( p, name, path );
        }
      }

      // Open the Open as items.
      pValues = pValues->NextSiblingElement();
    }
  }
  return true;
}

// -------------------------------------------------------------
bool LoaderManager::SaveXML()
{
  // create a new plan
  TiXmlDocument doc;

  // save the basic items
  // the declaration
  TiXmlDeclaration * decl = new TiXmlDeclaration( _T("1.0"), _T("UTF-8"), _T("") );
  doc.LinkEndChild( decl );

  TiXmlElement* pElemLoadManager = new TiXmlElement( _T("LoadManager") );
  doc.LinkEndChild( pElemLoadManager );

  TiXmlElement* pElemLoadCommands = new TiXmlElement( _T("Commands") );
  pElemLoadManager->LinkEndChild( pElemLoadCommands );

  for( OPENAS_NAMES::const_iterator it = m_openAs.begin(); it != m_openAs.end(); ++it )
  {
    TiXmlElement* pValue = new TiXmlElement( _T("Command") );
    pElemLoadCommands->LinkEndChild( pValue );

    TiXmlElement* pName = new TiXmlElement( _T("Name") );
    pName->LinkEndChild( new TiXmlText( it->first.c_str() ) );

    TiXmlElement* pPath = new TiXmlElement( _T("Path") );
    pPath->LinkEndChild( new TiXmlText( it->second.c_str()) );

    pValue->LinkEndChild( pName );
    pValue->LinkEndChild( pPath );
  }

  // save the file.
  doc.SaveFile( m_lpFilePath.c_str() );

  return true;
}