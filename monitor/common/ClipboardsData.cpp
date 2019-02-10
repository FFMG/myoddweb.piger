#include "stdafx.h"
#include "ClipboardsData.h"

ClipboardsData::ClipboardsData()
{
}

ClipboardsData::ClipboardsData(const ClipboardsData& rhs)
{
  *this = rhs;
}

ClipboardsData::~ClipboardsData()
{
}

const ClipboardsData& ClipboardsData::operator=(const ClipboardsData& rhs)
{
  if (this != &rhs)
  {
    _text = rhs._text;
    _fileNames = rhs._fileNames;
  }
  return *this;
}

/**
 * Reset the clipboard data.
 */
void ClipboardsData::Reset()
{
  _text.clear();
  _fileNames.clear();
}

void ClipboardsData::SetText(const MYODD_STRING& text)
{
  _text = text;
}

void ClipboardsData::AddPossibleFileName(const MYODD_STRING& possibleFilename)
{
  if( !myodd::files::IsFile(possibleFilename) &&
      !myodd::files::IsDirectory(possibleFilename) &&
      !myodd::files::IsURL(possibleFilename)
    )
  {
    return;
  }

  MYODD_STRING first = myodd::strings::lower(possibleFilename);
  if (myodd::files::IsDirectory(first.c_str()))
  {
    // Make sure the trailing slash looks similar for all
    myodd::files::RemoveTrailingBackSlash(first);
    myodd::files::AddTrailingBackSlash(first);
  }

  std::map<MYODD_STRING, MYODD_STRING >::const_iterator it = _fileNames.find(first);
  if (it != _fileNames.end())
  {
    //  we already have it.
    return;
  }

  _fileNames[first] = possibleFilename;
}

/**
 * Get a folder from our list of files.
 * @param MYODD_STRING& sText
 * @param size_t idx
 * @param bool bQuote
 * @return bool if we found the item or not.
 */
bool ClipboardsData::GetFolder(MYODD_STRING& sText, size_t idx, bool bQuote ) const
{
  sText = _T(""); //  reset the contents

  // look around for the first item that match
  size_t actual = 0;
  for (DataFiles::const_iterator it = _fileNames.begin();
       it != _fileNames.end();
       it++
     )
  {
    // is it a folder
    if (!myodd::files::IsDirectory(it->second.c_str()))
    {
      continue;
    }

    // we found one more item
    if (actual != idx)
    {
      // 
      actual++;
      continue;
    }

    // we found what we wanted.
    sText = ClipboardsData::Quote(it->second, bQuote);
    return true;
  }

  // if we are here we found nothing.
  return false;
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
bool ClipboardsData::GetTextFromClipboard(MYODD_STRING& sText, bool bQuote ) const
{
  sText = _T(""); //  reset the contents
  if (GetText(sText, bQuote ))
  {
    return true;
  }

  if (_fileNames.size() == 0)
  {
    return false;
  }

  for ( DataFiles::const_iterator it = _fileNames.begin();
        it != _fileNames.end();
        it++
      )
  {
    if (it != _fileNames.begin())
    {
      sText += _T(" ");
    }
    sText += it->second;
  }

  // quote it.
  sText = ClipboardsData::Quote( sText, bQuote);
  return true;
}

/**
 * Get the currently selected Text
 * @param string that will received the data
 * @param bool bQuote if we want to quote the filename
 * @return BOOL true if we have Data or FALSE if not.
 */
bool ClipboardsData::GetText(MYODD_STRING& sText, bool bQuote ) const
{
  sText = _T(""); //  reset the contents
  if ( _text.length() == 0)
  {
    return false;
  }

  //  get the value we have.
  sText = ClipboardsData::Quote( _text, bQuote );
  return true;
}

/**
 * Get the filename # if if exists.
 *
 * @param string that will received the data
 * @param int the filename number/index we are looking for.
 * @param bool bQuote if we want to quote the filename
 * @return BOOL true if we have Data or FALSE if not.
 */
bool ClipboardsData::GetFile(MYODD_STRING& sText, size_t idx, bool bQuote ) const
{
  sText = _T(""); //  reset the contents

  // look around for the first item that match
  size_t actual = 0;
  for ( std::map<MYODD_STRING, MYODD_STRING >::const_iterator it = _fileNames.begin();
        it != _fileNames.end();
        it++
      )
  {
    if (!myodd::files::IsFile(it->second.c_str()))
    {
      continue;
    }

    // -- we found one more item
    if (actual == idx)
    {
      // we found what we wanted.
      sText = ClipboardsData::Quote(it->second, bQuote);
      return true;
    }
    actual++;
  }

  // if we are here we found nothing.
  return false;
}

/**
 * Get the URLS if we have anything.
 *
 * @param string that will received the data
 * @param int the URL number/index we are looking for.
 * @param bool bQuote if we want to quote the filename
 * @return BOOL true if we have Data or FALSE if not.
 */
bool ClipboardsData::GetUrl(MYODD_STRING& sText, size_t idx, bool bQuote ) const
{
  sText = _T(""); //  reset the contents

  // look around for the first item that match
  size_t actual = 0;
  for (DataFiles::const_iterator it = _fileNames.begin();
       it != _fileNames.end();
       it++)
  {
    if (!myodd::files::IsURL(it->second.c_str()))
    {
      continue;
    }

    // -- we found one more item
    if (actual == idx)
    {
      // we found what we wanted.
      sText = ClipboardsData::Quote(it->second, bQuote);
      return true;
    }
    actual++;
  }

  // if we are here we found nothing.
  return false;
}

MYODD_STRING ClipboardsData::Quote(const MYODD_STRING& sSrc, bool bQuote)
{
  MYODD_STRING sText = sSrc;
  if ( bQuote )
  {
    sText = _T("\"") + sText + _T("\"");
  }
  return sText;
}