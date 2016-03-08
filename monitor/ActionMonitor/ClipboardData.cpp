#include "stdafx.h"
#include "ClipboardData.h"

ClipboardData::ClipboardData()
{
  //  null all the values.
  NullAll();
}

ClipboardData::~ClipboardData()
{
  //  free the memory/data
  Free();
}

/**
 * Free the meta file clipboard data if we have one.
 */
void ClipboardData::FreeClipboardEnhmetafile()
{
  if (data)
  {
    DeleteEnhMetaFile((HENHMETAFILE)data);
    data = NULL;
  }
  FreeClipboardName();
}

/**
 * Free the default clipboard data if we have one.
 */
void ClipboardData::FreeClipboardDefault()
{
  if (data)
  {
    delete[] data;
    data = NULL;
  }
  FreeClipboardName();
}

/**
 * Free the clipboard name if we have one.
 */
void ClipboardData::FreeClipboardName()
{
  // free the data name
  if (NULL == dataName)
  {
    return;
  }
  delete[] dataName;
  dataName = NULL;
}

/**
 * Delete all the data and ensure that the resources are freed.
 */
void ClipboardData::Free()
{
  switch (uFormat)
  {
  case CF_ENHMETAFILE:
    FreeClipboardEnhmetafile();
    break;

  default:
    FreeClipboardDefault();
    break;
  }
  
  // Null everything.
  NullAll();
}

/**
 * Null all the values.
 */
void ClipboardData::NullAll()
{
  uFormat = 0;
  dataSize = 0;
  data = NULL;
  dataName = NULL;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param HGLOBAL hData the clipboard data.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromFromClipboardHDrop(HGLOBAL hData)
{
  //  https://msdn.microsoft.com/en-us/library/windows/desktop/bb776408%28v=vs.85%29.aspx
  std::vector<STD_TSTRING> res;
  DROPFILES* df = (DROPFILES*)GlobalLock(hData);
  wchar_t* files = (wchar_t*)(df + 1);
  wchar_t buf[MAX_PATH];
  int bufLen = 0;
  int i = 0;

  while (true)
  {
    buf[bufLen++] = files[i];

    if (files[i] == '\0')
    {
      if (bufLen == 1)
      {
        break;
      }

      res.push_back(buf);
      bufLen = 0;
    }

    i++;
  }

  // release the lock
  GlobalUnlock(hData);
  return NULL;
}

/**
 * Create the clipboard data for a given format. 
 * This assumes that the clipboard is open.
 * @param HGLOBAL hData the clipboard data.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromFromClipboardEnhmetafile(HGLOBAL hData)
{
  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();

  //  copy the meta file.
  cf->data = (unsigned char*)CopyEnhMetaFileW((HENHMETAFILE)hData, NULL);
  cf->dataSize = 0;
  cf->uFormat = CF_ENHMETAFILE;

  // return the created filedata.
  return cf;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param HGLOBAL hData the clipboard data. 
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromFromClipboardBitmap(HGLOBAL hData)
{
  SIZE_T dataSize = sizeof(BITMAP);
  unsigned char* data = new unsigned char[dataSize];
  if (!GetObject(hData, sizeof(BITMAP), (void *)data))
  {
    delete[] data;
    return NULL;
  }

  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();

  //  copy the meta file.
  cf->data = data;
  cf->dataSize = dataSize;
  cf->uFormat = CF_BITMAP;

  // return the created filedata.
  return cf;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param UINT format the format we are looking for.
 * @param HGLOBAL hData the clipboard data.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromFromClipboardDefault(UINT format, HGLOBAL hData)
{
  // Get the size of the data
  SIZE_T dataSize = GlobalSize(hData);
  unsigned char *data = NULL;

  //  if the size is zero then there is no data.
  // it could be a type of data that has no use for the data??
  if (dataSize > 0)
  {
    //  we have some data, so we can try and get it.
    unsigned char* lptstr = (unsigned char*)GlobalLock(hData);
    if (lptstr == NULL)
    {
      return NULL;
    }

    // Allocate data and copy the data
    data = new unsigned char[dataSize];
    memcpy(data, lptstr, dataSize);

    // we can now free the memory.
    GlobalUnlock(data);
  }

  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();

  //  save the data to the struct and add it to the vector.
  cf->data = data;
  cf->dataSize = dataSize;
  cf->uFormat = format;
  cf->dataName = ClipboardData::GetClipboardName(format);

  return cf;
}

/**
 * Get the clipboard name if there is one.
 * @param UINT format the format we want the name of.
 * @return wchar_t* the format name or NULL if there isn't one, (or not needed).
 */
wchar_t* ClipboardData::GetClipboardName(UINT format)
{
  // if it is a known type then we don't need/have the name
  if (format < CF_MAX)
  {
    return NULL;
  }

  //  get the text from the clipboard.
  static const unsigned l = 256;  //  max len of the meta file
  wchar_t* dataName = new wchar_t[l + 1];
  memset(dataName, 0, l + 1);
  if (GetClipboardFormatNameW(format, dataName, l) == 0)
  {
    //  either it did not work
    // or the value we found is not good.
    delete[] dataName;
    return NULL;
  }

  // return the value we found.
  return dataName;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param UINT format the format we want to copy/get.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromFromClipboard(UINT format)
{
  if (!IsClipboardFormatAvailable(format))
  {
    return NULL;
  }

  // Get the handle of the Global memory that contains the text
  HGLOBAL hData = GetClipboardData(format);
  if (hData == NULL)
  {
    return NULL;
  }

  switch (format)
  {
  case CF_HDROP:
    return ClipboardData::FromFromClipboardHDrop(hData);
    break;

  case CF_ENHMETAFILE:
    return ClipboardData::FromFromClipboardEnhmetafile(hData);

  case CF_BITMAP:
    return ClipboardData::FromFromClipboardBitmap(hData);

  default:
    return ClipboardData::FromFromClipboardDefault(format, hData);
  }

  //  never reached
  return NULL;
}
