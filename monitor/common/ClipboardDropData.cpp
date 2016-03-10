#include "stdafx.h"
#include "ClipboardDropData.h"


ClipboardDropData::ClipboardDropData()
{
}

ClipboardDropData::~ClipboardDropData()
{
}

/**
 * Create the clipboard data from a DROPFILES
 * If the pointer is created, it is up to the calling function to delete.
 * @param DROPFILES* dropFiles the dropfiles pointer.
 * @return ClipboardDropData* pointer to this class.
 */
ClipboardDropData* ClipboardDropData::FromDROPFILES(DROPFILES* dropFiles)
{
  // the return pointer.
  ClipboardDropData* df = new ClipboardDropData();

  try
  {
    // get all the files.
    wchar_t* files = (wchar_t*)(dropFiles + 1);
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

        //  add the file.
        df->_files.push_back(buf);

        // reset the position of the buffer.
        bufLen = 0;
      }

      i++;
    }

    // did we find anything?
    if (df->_files.size() == 0)
    {
      delete df;
      return NULL;
    }
  }
  catch( ... )
  {
    //  something didn't work!
    delete df;
    return NULL;
  }

  // we are done
  return df;
}

