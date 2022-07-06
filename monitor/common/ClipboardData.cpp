#include "stdafx.h"
#include "ClipboardData.h"
#include "ClipboardDropData.h"

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
 * Free the default clipboard data if we have one.
 */
void ClipboardData::FreeClipboardBitmap()
{
  if (data)
  {
    DeleteObject((HBITMAP)data);
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
 * Free the clipboard hdrop files.
 */
void ClipboardData::FreeClipboardHDrop()
{
  if (data)
  {
    delete static_cast<ClipboardDropData*>(data);
    data = NULL;
  }
  FreeClipboardName();
}

/**
 * Delete all the data and ensure that the resources are freed.
 */
void ClipboardData::Free()
{
  switch (uFormat)
  {
  case CF_HDROP:
    FreeClipboardHDrop();
    break;

  case CF_ENHMETAFILE:
    FreeClipboardEnhmetafile();
    break;

  case CF_BITMAP:
    FreeClipboardBitmap();
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
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromClipboardHDrop(HGLOBAL hData, size_t maxMemory)
{
  //  https://msdn.microsoft.com/en-us/library/windows/desktop/bb776408%28v=vs.85%29.aspx
  std::vector<std::wstring> res;

  // get the files.
  DROPFILES* dfs = (DROPFILES*)GlobalLock(hData);
  ClipboardDropData* drop = ClipboardDropData::FromDROPFILES(dfs, maxMemory);

  // release the lock
  GlobalUnlock(hData);

  // did we actually find anything orthwhile?
  if (NULL == drop)
  {
    return NULL;
  }

  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();

  //  copy the meta file.
  cf->data = static_cast<void*>(drop);
  cf->dataSize = 0;
  cf->uFormat = CF_HDROP;

  // return the format.
  return cf;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param HGLOBAL hData the clipboard data.
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromClipboardEnhmetafile(HGLOBAL hData, size_t maxMemory)
{
  // check the size of the enhenced meta file.
  // in case it is too large.
  if (maxMemory > 0 && GetEnhMetaFileBits((HENHMETAFILE)hData, 0, NULL) > maxMemory)
  {
    return NULL;
  }

  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();

  //  copy the meta file.
  cf->data = CopyEnhMetaFileW((HENHMETAFILE)hData, NULL);
  cf->dataSize = 0;
  cf->uFormat = CF_ENHMETAFILE;

  // return the created filedata.
  return cf;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param HGLOBAL hData the clipboard data.
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromClipboardBitmap(HGLOBAL hData, size_t maxMemory)
{
  // check the memory size.
  if (maxMemory > 0 && GetBitmapSize((HBITMAP)hData) > maxMemory)
  {
    return NULL;
  }

  // build the data clipboard so we can restore it.
  ClipboardData *cf = new ClipboardData();
  
  //  copy the meta file.
  cf->data = static_cast<void*>(CopyBitmap( (HBITMAP)hData ));
  cf->dataSize = 0;
  cf->uFormat = CF_BITMAP;

  // return the created filedata.
  return cf;
}

/**
 * Create the clipboard data for a given format.
 * This assumes that the clipboard is open.
 * @param UINT format the format we are looking for.
 * @param HGLOBAL hData the clipboard data.
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromClipboardDefault(UINT format, HGLOBAL hData, size_t maxMemory)
{
  // Get the size of the data
  SIZE_T dataSize = GlobalSize(hData);

  //  is it too big?
  if (maxMemory > 0 && dataSize > maxMemory)
  {
    return NULL;
  }

  unsigned char *data = NULL;

  //  if the size is zero then there is no data.
  // it could be a type of data that has no use for the data??
  if (dataSize > 0)
  {
    //  we have some data, so we can try and get it.
    void* lptstr = GlobalLock(hData);
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
  cf->data = static_cast<void*>(data);
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
  static const unsigned l = 1024;  //  max len of the meta file
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
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardData*|NULL either the data or NULL if the format does not exist.
 */
ClipboardData* ClipboardData::FromClipboard(UINT format, size_t maxMemory)
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
    return ClipboardData::FromClipboardHDrop(hData, maxMemory );

  case CF_ENHMETAFILE:
    return ClipboardData::FromClipboardEnhmetafile(hData, maxMemory );

  case CF_BITMAP:
    return ClipboardData::FromClipboardBitmap(hData, maxMemory );

  default:
    return ClipboardData::FromClipboardDefault(format, hData, maxMemory );
  }

  //  never reached
  return NULL;
}

void ClipboardData::ToClipboard()
{
  //  do we have a name to restore??
  UINT format = uFormat;
  if ( dataName != NULL)
  {
    UINT u = RegisterClipboardFormat( dataName);
    if (u > 0)
    {
      //  the format has changed it seems.
      format = u;
    }
  }

  switch (format)
  {
  case CF_HDROP:
    {
      ClipboardDropData* cdd = static_cast<ClipboardDropData*>(data);

      // get the size of the clipboard.
      size_t size = cdd->GetDropfilesSize();
      if (size > 0)
      {
        // make some room for this DROPFILES item.
        HGLOBAL hGlobal = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, size);

        // lock it so we can now use it.
        DROPFILES *df = (DROPFILES*)GlobalLock(hGlobal);
        
        // populate the data if we can.
        if (cdd->PopulateDropFiles(df, size))
        {
          // release the lock
          GlobalUnlock(hGlobal);

          // set the data in the clipboard.
          SetClipboardData(CF_HDROP, hGlobal);
        }
        else
        {
          // release the lock
          GlobalUnlock(hGlobal);

          // free the memory
          GlobalFree(hGlobal);
        }
      }
    }   
    break;

  case CF_ENHMETAFILE:
    SetClipboardData(CF_ENHMETAFILE, CopyEnhMetaFile((HENHMETAFILE)data, NULL));
    break;

  case CF_BITMAP:
    SetClipboardData(CF_BITMAP, CopyBitmap( (HBITMAP)data ));
    break;

  default:
    if (dataSize > 0)
    {
      //  get some data
      HGLOBAL hGlobal = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, dataSize);
      void* pMem = GlobalLock(hGlobal);
      memcpy(pMem, data, dataSize);
      GlobalUnlock(hGlobal);

      // we can no save the data.
      SetClipboardData(format, hGlobal);
    }
    else
    {
      //  there is no data to set...
      SetClipboardData(format, 0);
    }
    break;
  }
}

/**
 * Get the size, in bits, of the source bitmap.
 * @param HBITMAP hBitmapSrc the bitmap we want to get the size of.
 * @return size_t the size of the bitmap.
 */
size_t ClipboardData::GetBitmapSize(HBITMAP hBitmapSrc)
{
  BITMAP  bitmap;
  if (0 == GetObject(hBitmapSrc, sizeof(BITMAP), &bitmap))
  {
    //  this failed, we don't know the size.
    return 0;
  }
  return (bitmap.bmHeight*bitmap.bmWidth*bitmap.bmBitsPixel);
}

/**
 * Copy an hbitmap.
 * @param HBITMAP hBitmapSrc the source bitmap.
 * @return HBITMAP the copy bitmap handle
 */
HBITMAP ClipboardData::CopyBitmap(HBITMAP hBitmapSrc)
{
  BITMAP  bitmap;
  HBITMAP hBitmapDst;
  HDC     hdcSrc, hdcDst;

  GetObject(hBitmapSrc, sizeof(BITMAP), &bitmap);
  hBitmapDst = CreateBitmapIndirect(&bitmap);

  hdcSrc = CreateCompatibleDC(NULL);
  hdcDst = CreateCompatibleDC(NULL);

  SelectObject(hdcSrc, hBitmapSrc);
  SelectObject(hdcDst, hBitmapDst);

  BitBlt(hdcDst, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
    hdcSrc, 0, 0, SRCCOPY);

  DeleteDC(hdcSrc);
  DeleteDC(hdcDst);

  return hBitmapDst;
}