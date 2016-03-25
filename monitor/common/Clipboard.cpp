#include "stdafx.h"
#include "clipboard.h"
#include "ClipboardDropData.h"

/**
 * Todo
 * @param void
 * @return void
 */
Clipboard::Clipboard( CWnd* mainWnd, size_t maxMemory ) : _maxMemory( maxMemory )
{
  Init(mainWnd);
}

/**
 * Todo
 * @param void
 * @return void
 */
Clipboard::~Clipboard()
{
}

Clipboard::Clipboard(const Clipboard& rhs) : _maxMemory( 0 )
{
  *this = rhs;
}

const Clipboard& Clipboard::operator=(const Clipboard& rhs)
{
  if (this != &rhs)
  {
    _clipboardData = rhs._clipboardData;
    _maxMemory = rhs._maxMemory;
  }
  return *this;
}

/**
 * The order is very important as some data is always present even when other data is selected.
 * For example if you have files you also have one filename, but if you have one file then you might not
 * have any files.
 * Folders is always given with files.
 * 
 * @param string the container that will, (maybe), have hold our return string.
 * @return BOOL TRUE if we have something to return.
 */
bool Clipboard::GetTextFromClipboard(STD_TSTRING& sText, bool bQuote /*= true */) const
{
  return _clipboardData.GetTextFromClipboard(sText, bQuote);
}

/**
 * Get the currently selected Text
 * @param string that will received the data
 * @return BOOL true if we have Data or FALSE if not.
 */
bool Clipboard::GetText( STD_TSTRING& sText, bool bQuote ) const
{
  return _clipboardData.GetText(sText, bQuote);
}

/**
 * Get the filename # if if exists.
 * 
 * @param string that will received the data
 * @param int the filename number/index we are looking for.
 * @return BOOL true if we have Data or FALSE if not.
 */
bool Clipboard::GetFile( STD_TSTRING& sText, size_t idx, bool bQuote ) const
{
  return _clipboardData.GetFile(sText, idx, bQuote );
}

/**
 * Get the folder if we have anything.
 * 
 * @param string that will received the data
 * @param int the folder number/index we are looking for.
 * @return BOOL true if we have Data or FALSE if not.
 */
bool Clipboard::GetFolder( STD_TSTRING& sText, size_t idx, bool bQuote ) const
{
  return _clipboardData.GetFolder(sText, idx, bQuote );
}

/**
 * Get the URLS if we have anything.
 * 
 * @param string that will received the data
 * @param int the URL number/index we are looking for.
 * @return BOOL true if we have Data or FALSE if not.
 */
bool Clipboard::GetURL( STD_TSTRING& sText, size_t idx, bool bQuote ) const
{
  return _clipboardData.GetURL(sText, idx, bQuote );
}

/**
 * All the structures are pointers so we must get each items and delete them all one by one
 * The structure has its own destructor and will be called for further cleanup
 *
 * @param vector of clipboard formats.
 * @return none
 */
void Clipboard::ResetClipboardFormats(V_CF& cf)const
{
  for( V_CF::const_iterator it = cf.begin(); it != cf.end(); it++)
  {
    delete (*it);
  }

  //  no empty the actual vector as per norml.
  cf.erase(cf.begin(), cf.end() );
}

/**
 * given a CITEMIDLIST get the name.
 *
 * @param LPCWSTR the data we are getting, 'folder' or 'item'
 * @param IShellFolder * the shell container
 * @param LPCITEMIDLIST the container with the info we are reading.
 * @param LPTSTR the return buffer
 * @param DWORD the max size of the buffer itself.
 * @return HRESULT S_OK or S_FALSE
 */
HRESULT Clipboard::GetNameFromPIDL
( 
 LPCWSTR pwzType,
 IShellFolder* pFolder, 
 LPCITEMIDLIST pidl,
 LPTSTR buffer, 
 DWORD lBuffer
 )
{
  STRRET strret;
  
  HRESULT hr = pFolder->GetDisplayNameOf(pidl,SHGDN_NORMAL | SHGDN_FORPARSING, &strret);
  if( !FAILED( hr ) )
  {
    memset( buffer, 0, lBuffer );
    _sntprintf_s( buffer, lBuffer, lBuffer, _T("%ls"), strret.pOleStr );
    TRACE( _T("%ls: %ls\n"), pwzType, strret.pOleStr);
    
    CoTaskMemFree(strret.pOleStr);
  }
  return S_FALSE;
}

/**
 * Parse the clipboard data and add file names, texts and what ever info could be used.
 * We cannot do much with non text items.
 * Update this function as new clipboard items are discovered.
 *
 * @param vector of clipboard data been parsed.
 * @return none.
 */
void Clipboard::ParseClipboard( V_CF& s_cf )
{
  USES_CONVERSION;

  for( V_CF::const_iterator it = s_cf.begin(); it != s_cf.end(); it++ )
  {
    try
    {
      ClipboardData* cf = *it;
      if( NULL == cf )
      {
        continue;
      }

      switch( cf->uFormat )
      {
      case 0x0c006: /* FILENAME */
        {
          //  a file name was added
          const wchar_t* lp = T_A2T((CHAR*)cf->data);
          AddFileName( lp );
        }
        break;

      case 0x0c007: /*FILENAMEW*/
        {
          //  a file name was added
          const wchar_t* lp = (WCHAR*)cf->data;
          AddFileName(lp);
        }
        break;

      case CF_UNICODETEXT:
        {
          //  normal, selected text.
          const wchar_t* lp = (WCHAR*)cf->data;
          SetText( lp );
        }
        break;

      case CF_TEXT:
        {
          //  normal, selected text.
          const wchar_t* lp = T_A2T( (CHAR*)cf->data );
          SetText( lp );
        }
        break;

      case CF_HDROP:
        {
          ClipboardDropData* cdd = (ClipboardDropData*)cf->data;
          const std::vector<STD_TSTRING>& files = cdd->Files();
          for (std::vector<STD_TSTRING>::const_iterator it = files.begin(); it != files.end(); ++it)
          {
            AddFileName( *it );
          }
        }
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
void Clipboard::Init( CWnd* mainWnd )
{ 
  // the current clipboard data
  V_CF clipboard_format;

  // the copied data made for this call.
  V_CF clipboard_format_new;

  try
  {
    //  save all the current clipboard data
    //  we use our current window, (NULL), for the work.
    GetCurrentData(NULL, clipboard_format);
    
    //  then copy the data from the previous foreground window.
    CopyDataFromForgroundWindow(mainWnd);
    
    // empty all the data we might have from previous calls.
    ResetClipBoardData();

    //  ok, so now we can get this window data
    GetCurrentData(mainWnd, clipboard_format_new);

    //  now that we have a brand new array of clipboards simply
    //  add as much info as possible to our structure
    //  that way we can choose what we actually want to use.
    ParseClipboard( clipboard_format_new );
    
    //  finally that we have saved that we need to restore the clipboard to the sate it was in b4.
    RestoreClipboard( NULL, clipboard_format );
  }
  catch(...)
  {
    ASSERT(0);  //  something bad has happened.
  }

  //  clear everything
  ResetClipboardFormats( clipboard_format);
  ResetClipboardFormats( clipboard_format_new);
}

/**
 * Restore the previous values in the clipboard.
 *
 * @param HWND the window that will own the clipboard
 * @param vector the array of Clipboard data we are trying to restore
 * @return BOOL success or not
 */
bool Clipboard::RestoreClipboard
(
 CWnd* wnd, 
 V_CF& s_cf 
 )
{
  if( NULL == wnd )
  {
    HWND hCurrent = GetForegroundWindow();
    wnd = CWnd::FromHandle( hCurrent );
    if( NULL == wnd )
    {
      return false;
    }
  }

  if (!::OpenClipboard(wnd->GetSafeHwnd()))
  {
    return false;
  }
  
  //  because we are restoring we need to remove the current value(s).
  EmptyClipboard();
  
  // now go around and restore all the Clipboard data(s) to the clipboard itself.
  for( V_CF::const_iterator it = s_cf.begin(); it != s_cf.end(); it++ )
  {
    ClipboardData* cf = *it;
    if( NULL == cf )
    {
      continue;
    }

    try
    {
      cf->ToClipboard();
    }
    catch(... )
    {
      _ASSERT(0); //  the main reason for failure is probably because  
                  //  there is a format in the Clipboard that I am not handling properly
                  //  there should be a way of sending me a mail when this happens so we can look into fixing it.
    }
  }
  
  //  all done
  //  allow other apps to use the clipbaord.
  CloseClipboard();

  return true;
}

/**
 * Todo
 * @param void
 * @return void
 */
void Clipboard::CopyDataFromForgroundWindow( CWnd* cPossibleWindow  )
{
  if (NULL == cPossibleWindow || !::IsWindow(cPossibleWindow->GetSafeHwnd())) 
  {
    return;
  }

  //  HWND hThis    = AfxGetMainWnd()->GetSafeHwnd();
  //  HWND hCurrent = GetForegroundWindow();
  //  if( hThis == hCurrent )
  //  {
  cPossibleWindow->SetForegroundWindow( );
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

  //  let the key be pressed for a short while. 
  Sleep(200);

  input[0].ki.dwFlags = 
  input[1].ki.dwFlags = KEYEVENTF_KEYUP ;
  input[0].ki.time = input[1].ki.time =  GetTickCount() ;
  SendInput( 2, input, sizeof(INPUT) ) ; 

  //  give the window half a chance to finish their work.
  //  and more importantly get, (when we call sleep), we surrender our thread call to give other threads
  //  a chance to do their work.
  Sleep(200);
}

/**
 * Todo
 * @param void
 * @param void
 * @return void
 */
void Clipboard::GetCurrentData
(
 CWnd* wnd, 
 V_CF& cf
 )
{
  try
  {
    //  clear the structure
    ResetClipboardFormats( cf );
    
    // we cannot go further if we have no data
    if( NULL == wnd )
    {
      HWND hCurrent = GetForegroundWindow();
      wnd = CWnd::FromHandle( hCurrent );
      if( NULL == wnd )
      {
        return;
      }
    }

    // do we have anything saved?
    if (CountClipboardFormats() == 0)
    {
      return;
    }

    //  open the clipboard for that window
    if (!OpenClipboard(wnd->GetSafeHwnd()))
    {
      return;
    }

    //  get the current text data 
    //  if there is nothing 
    UINT uFormat = EnumClipboardFormats(0); 
    while (uFormat) 
    { 
      //  read that data from the clipbaord
      ClipboardData *current = ClipboardData::FromClipboard( uFormat, _maxMemory );
      if( current )
      {
        //  we cannot do much about NULL clipboard data
        //  this is probably private data that we could not use anyway.
        cf.push_back(current);
      }
      
      //  get the next format
      uFormat = EnumClipboardFormats(uFormat); 
    }
    
    //  empty the clipboard
    EmptyClipboard();

    // close the clipboard.
    CloseClipboard(); 
  }
  catch(... )
  {
      _ASSERT(0); //  the main reason for failure is probably because  
                  //  there is a format in the Clipboard that I am not handling properly
                  //  there should be a way of seending me a mail when this happens so we can look into fixing it.
  }
}

/**
 * Add a filename to our list of filenames.
 * @param const STD_TSTRING& fileName the filename we want to add.
 */
void Clipboard::AddFileName(const STD_TSTRING& fileName)
{
  // add it to our list.
  _clipboardData.AddPossibleFileName(fileName);
}

/**
* Add a filename to our list of filenames.
* @param const STD_TSTRING& fileName the filename we want to add.
*/
void Clipboard::SetText(const STD_TSTRING& srcText)
{
  //  set the text value.
  _clipboardData.SetText(srcText);

  // it is posible that the text selected represents a file
  // or a directory, in that case it is useful to display it to the APIs
  AddFileName(srcText);
}

/**
 * Todo
 * @param void
 * @return void
 */
void Clipboard::ResetClipBoardData()
{
  // emtpty everything
  _clipboardData.Reset();
}
