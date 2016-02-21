// Clipboard.cpp: implementation of the CClipboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Clipboard.h"

#include <atlbase.h>
#include "shlObj.h"
#include "shlGuid.h"    // IID_IShellFolder

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define GetPIDLFolder(pida) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[0])
#define GetPIDLItem(pida, i) (LPCITEMIDLIST)(((LPBYTE)pida)+(pida)->aoffset[i+1])

CClipboard::CClipboard( HWND hwnd ) : hwndMain( hwnd )
{
  Init();
}

CClipboard::~CClipboard()
{
}

std::string CClipboard::implode
(
 const std::vector<std::string> &s, 
 std::string glue 
 )
{
  std::string r = "";
  for( unsigned c = 0; c<s.size(); c++)
  {
    r = ((c>0)?	(r + glue) : ("")) + s[c];
  }
  return r;
}

/**
 * The order is very important as some data is always present even when other data is selected.
 * For example if you have files you also have one filename, but if you have one file then you might not
 * have any files.
 * Folders is always given with files.
 */
BOOL CClipboard::GetTextFromClipboard( std::string &sText )
{
  if( clipboard_data.cdTEXT.length() > 0 )
  {
    sText = clipboard_data.cdTEXT.c_str();
    return TRUE;
  }
  else
  if( clipboard_data.cdFILENAMES.size() > 0 )
  {
    sText = "\"" + implode( clipboard_data.cdFILENAMES, "\" \"" ) + "\"";
    return TRUE;
  }
  else
  if( clipboard_data.cdFILENAME.size() > 0 )
  {
    sText = "\"" + clipboard_data.cdFILENAME + "\"";
    return TRUE;
  }
  else
  if( clipboard_data.cdFOLDER.length() > 0 )
  {
    sText = "\"" + clipboard_data.cdFOLDER + "\"";
    return TRUE;
  }
  return FALSE;
}

CClipboard::CLIPBOARD_FORMAT * CClipboard::GetDataFromClipboard( UINT format, HWND h )
{
  CLIPBOARD_FORMAT *cf = NULL;
  if (!IsClipboardFormatAvailable(format)) 
    return cf;
  
  // Get the handle of the Global memory that contains the text
  HGLOBAL hData = GetClipboardData(format); 
  if (hData == NULL) 
  {
    return cf;
  }
  // Get the size of the data
  SIZE_T DataSize = GlobalSize(hData);
  BYTE* lptstr = (BYTE*)GlobalLock(hData); 
  if (lptstr == NULL) 
  { 
    return cf;
  }
  // Allocate data and copy the data
  BYTE *data = new BYTE[DataSize];
  memcpy(data, lptstr, DataSize);
  GlobalUnlock(data); 
  
  cf = new CLIPBOARD_FORMAT();
  cf->data      = data;
  cf->dataSize  = DataSize;
  cf->uFormat   = format;
  
  if( format <= 14 /*CF_TEXT(1) -> CF_ENHMETAFILE(14)*/)
  {
    cf->dataName = NULL;
  }
  else 
  {
    static const unsigned l = 100;  //  max len of the meta file
    cf->dataName = new char[l+1];
    memset( cf->dataName, 0, l+1 );
    if( GetClipboardFormatName(format, cf->dataName, l) == 0 )
    {
      delete [] cf->dataName;
      cf->dataName = NULL;
    }
  }
  
  // deal with special cases.
  switch( format )
  {
  case CF_ENHMETAFILE:
    cf->dataMetaFile = ::CopyEnhMetaFile((HENHMETAFILE)hData, NULL );
    break;
    
  default:
    cf->dataMetaFile = 0;
    break;
  }// special cases.
  return cf; 
}

void CClipboard::ClearStruct(std::vector<CLIPBOARD_FORMAT *> *cf)
{
  for( unsigned loop = 0; loop < cf->size(); loop++)
  {
    CLIPBOARD_FORMAT *current = *(cf->begin()+loop);
    if( current )
    {
      delete current;
    }
  }
  cf->erase(cf->begin(), cf->end() );
}

HRESULT CClipboard::GetNameFromPIDL
( 
 LPCWSTR pwzType,
 IShellFolder* pFolder, 
 LPCITEMIDLIST pidl,
 LPSTR buffer, 
 DWORD lBuffer
 )
{
  STRRET strret;
  
  HRESULT hr = pFolder->GetDisplayNameOf(pidl,SHGDN_NORMAL | SHGDN_FORPARSING, &strret);
  if( !FAILED( hr ) )
  {
    memset( buffer, 0, lBuffer );
    _snprintf( buffer, lBuffer, "%ls", strret.pOleStr );
    TRACE(_T("%ls: %ls\n"), pwzType, strret.pOleStr);
    
    CoTaskMemFree(strret.pOleStr);
  }
  return S_FALSE;
}

/**
 * Parse the clipboard data and add file names, texts and what ever info could be used.
 * We cannot do much with non text items.
 * Update this function as new clipboard items are discovered.
 *
 * @param vector of clipboard data.
 * @return none.
 */
void CClipboard::ParseClipboard( std::vector<CLIPBOARD_FORMAT *> *v_cf )
{
  clipboard_data.cdFILENAMES.erase( clipboard_data.cdFILENAMES.begin(), clipboard_data.cdFILENAMES.end() );
  for( unsigned u = 0; u < v_cf->size(); u++ )
  {
    try
    {
      CLIPBOARD_FORMAT * cf = *(v_cf->begin()+u);
      if( NULL == cf )
      {
        continue;
      }
      switch( cf->uFormat )
      {
      case 0x0c074: //  Shell IDList Array
        {
          IShellFolder* spDesktop;
          HRESULT hr = SHGetDesktopFolder(&spDesktop); 
          if( !FAILED(hr ) )
          {
            CIDA* cida = (CIDA*)cf->data;
            LPCITEMIDLIST lpFolder  = GetPIDLFolder( cida );
            //  Get the folder name
            char szFolder[MAX_PATH]; 
            hr = GetNameFromPIDL(L"Folder", spDesktop, lpFolder, szFolder, MAX_PATH );
            if( !FAILED(hr ) )
            {
              //  save this folder name
              clipboard_data.cdFOLDER = szFolder;
              
              for (UINT i=0; i<cida->cidl; ++i)
              {
                IShellFolder* spFolder;
                hr = spDesktop->BindToObject(lpFolder, 0, IID_IShellFolder, (void**)&spFolder);
                if( !FAILED( hr ) )
                {
                  LPCITEMIDLIST pidlMachine = GetPIDLItem(cida, i);
                  char szFileName[MAX_PATH]; 
                  hr = GetNameFromPIDL(L"Item", spFolder, pidlMachine, szFileName, MAX_PATH );
                  if( !FAILED(hr ) )
                  {
                    clipboard_data.cdFILENAMES.push_back( szFileName );
                  }
                  spFolder->Release();
                }// BindToObject
                
              }// each items in the folder
              
            }// GetNameFromPIDL(...)
            
            spDesktop->Release();
          }// SHGetDesktopFolder(...)
        }
        break;
        
      case 0x0c006: /* FILENAME - (case 0x0c007: FILENAMEW )*/
        //  a file name was added
        clipboard_data.cdFILENAME = (char*)cf->data;
        break;
        
      case CF_TEXT:
        //  normal text
        clipboard_data.cdTEXT = (char*)cf->data;
        break;
        
      default:  //  not handled.
        break;
      }
    }
    catch(... )
    {
      _ASSERT(0); //  the main reason for failure is probably because  
                  //  there is a format in the Clipboard that I am not handling properly
                  //  there should be a way of seending me a mail when this happens so we can look into fixing it.
    }
  }// for each clipboard items
}

/**
 * Init the clipboard. 
 *  1- Save the current clipboard
 *  2- Call copy from the active window
 *  3- Save what ever info was returned
 *  4- Restore the clipboard
 *
 * @param none
 * @return none
 */
void CClipboard::Init()
{ 
  //  save all the current clipboard data
  std::vector<CLIPBOARD_FORMAT *> clipboard_format;
  GetCurrentData(NULL, &clipboard_format);
  
  
  //  then copy the data from the previous foreground window.
  CopyDataFromForgroundWindow( hwndMain );
  
  //  ok, so now we can get this window data
  std::vector<CLIPBOARD_FORMAT *> clipboard_format_new;
  GetCurrentData(hwndMain, &clipboard_format_new);
  
  //  now that we have a brand new array of clipboards simply
  //  add as much info as posible to our structure
  //  that way we can choose what we actually want to use.
  ParseClipboard( &clipboard_format_new );
  
  //  now that we have saved that we need to restore the clipboard to the sate it was in b4.
  RestoreClipboard( NULL, &clipboard_format );
  
  //  clear everything
  ClearStruct( &clipboard_format);
  ClearStruct( &clipboard_format_new);
}

/**
 * Restore the previous values in the clipboad.
 *
 * @param HWND the window that will own the clipboad
 * @parna vector the array of Clipboard data we are trying to restore
 * @return BOOL success or not
 */
BOOL CClipboard::RestoreClipboard
(
 HWND h, 
 std::vector<CLIPBOARD_FORMAT *> *s_cf 
 )
{
  if( ! ::OpenClipboard(h) )
    return FALSE;
  
  EmptyClipboard();
  
  for( unsigned u = 0; u < s_cf->size(); u++ )
  {
    CLIPBOARD_FORMAT * cf = *(s_cf->begin()+u);
    if( NULL == cf )
    {
      continue;
    }
    try
    {
      UINT format = cf->uFormat;
      if( cf->dataName != NULL )
      {
        UINT u = RegisterClipboardFormat( cf->dataName );
        if( u > 0 )
        {
          format = u;
        }
      }
      
      HANDLE hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, cf->dataSize );
      LPVOID pMem = GlobalLock( hMem );
      
      memcpy( pMem, cf->data, cf->dataSize );
      
      GlobalUnlock( hMem );
      SetClipboardData( format, hMem );
    }
    catch(... )
    {
      _ASSERT(0); //  the main reason for failure is probably because  
                  //  there is a format in the Clipboard that I am not handling properly
                  //  there should be a way of seending me a mail when this happens so we can look into fixing it.
    }
  }
  
  CloseClipboard();
  return TRUE;
}

void CClipboard::CopyDataFromForgroundWindow( HWND hPossibleWindow )
{
  //  HWND hThis    = AfxGetMainWnd()->GetSafeHwnd();
  //  HWND hCurrent = GetForegroundWindow();
  //  if( hThis == hCurrent )
  //  {
  ::SetForegroundWindow(hPossibleWindow);
  //  }
  //  else
  //  {
  //    hwndMain = hCurrent;
  //  }

  INPUT input[2] ;
  ZeroMemory( input, sizeof(input) ) ;
  input[0].type = input[1].type = INPUT_KEYBOARD ;
  input[0].ki.wVk = VK_CONTROL ;
  input[1].ki.wVk = (0x41 + ('c'-'a'));
  input[0].ki.time = input[1].ki.time =  GetTickCount() ;
  SendInput( 2, input, sizeof(INPUT) ) ; 
  Sleep(50);

  input[0].ki.dwFlags = 
  input[1].ki.dwFlags = KEYEVENTF_KEYUP ;
  input[0].ki.time = input[1].ki.time =  GetTickCount() ;
  SendInput( 2, input, sizeof(INPUT) ) ; 
  Sleep(50);
/*
  keybd_event(VK_CONTROL,0,0,0);
  keybd_event (0x43, 0, 0, 0 ); //Send the C key (43 is "C")
  keybd_event (0x43, 0, KEYEVENTF_KEYUP, 0);
  keybd_event (VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);// 'Left Control Up

  //  give the window half a chance to finish their work.
  //  and more importantly get, (when we call sleep), we surender our thread call to give other threads
  //  a chance to do their work.
  Sleep(1000);
*/  
}

void CClipboard::GetCurrentData
(
 HWND h, 
 std::vector<CLIPBOARD_FORMAT *> *cf
 )
{
  try
  {
    ClearStruct( cf );
    
    if (CountClipboardFormats() == 0) 
      return; 
    
    if (!OpenClipboard(h)) 
      return; 
    
    //  get the current text data 
    //  if there is nothing 
    UINT uFormat = EnumClipboardFormats(0); 
    while (uFormat) 
    { 
      CLIPBOARD_FORMAT *current = GetDataFromClipboard( uFormat, h );
      if( current )
      {
        //  we cannot do much about NULL clipboard data
        //  this is probably private data that we could not use anyway.
        cf->push_back(current);
      }
      
      //  get the next format
      uFormat = EnumClipboardFormats(uFormat); 
    }
    
    //  empty the clipboard
    EmptyClipboard();
    
    CloseClipboard(); 
  }
  catch(... )
  {
      _ASSERT(0); //  the main reason for failure is probably because  
                  //  there is a format in the Clipboard that I am not handling properly
                  //  there should be a way of seending me a mail when this happens so we can look into fixing it.
  }
}
