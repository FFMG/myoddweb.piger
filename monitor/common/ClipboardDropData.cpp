#include "stdafx.h"
#include "ClipboardDropData.h"


ClipboardDropData::ClipboardDropData()
{
}

ClipboardDropData::~ClipboardDropData()
{
}

/**
 * Calculate how many bits all the files will take.
 * @param DROPFILES * the data containter that has all the files
 * @return size_t the total number of bits we need.
 */
size_t ClipboardDropData::CalculateBits(DROPFILES *df)
{
  // all the files sepatrated by '\0' and end with '\0\0'
  wchar_t* files = (wchar_t*)(df + 1);

  // the total size
  int totalLen = 0;

  // the current files size.
  int currentLen = 0;
  while (true)
  {
    // add one character
    currentLen++;

    // is that character an end of file?
    if (files[totalLen] == '\0')
    {
      // did we find 2 in a row?
      if (currentLen == 1)
      {
        break;
      }
      // reset the position of the buffer.
      currentLen = 0;
    }

    // increase the size.
    totalLen++;
  }

  // return the total lenght together with 
  // the size of a wchar_t
  return (totalLen*sizeof(wchar_t));
}

/**
 * Create the clipboard data from a DROPFILES
 * If the pointer is created, it is up to the calling function to delete.
 * @param DROPFILES* dropFiles the dropfiles pointer.
 * @param size_t maxMemory the max memory we want to allow to prevent lock-up.
 * @return ClipboardDropData* pointer to this class.
 */
ClipboardDropData* ClipboardDropData::FromDROPFILES(DROPFILES* dropFiles, size_t maxMemory)
{
  // will this use more memory than we want to?
  if (maxMemory > 0 && ClipboardDropData::CalculateBits(dropFiles) > maxMemory)
  {
    return NULL;
  }

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

/**
 * Given the files we have, calculate the DROPFILES file size.
 * @return size_t the size needed for the DROPFILES pointer.
 */
size_t ClipboardDropData::GetDropfilesSize() const
{
  size_t size = 0;
  size += sizeof(DROPFILES);

  for (DropFiles::const_iterator it = Files().begin(); it != Files().end(); ++it)
  {
    // the total memory size that we will be using.
    // we add one more character at the end of the '\0'
    size += ((*it).size() + 1)*sizeof(wchar_t);
  }
  size += sizeof(L'\0'); // a trailling '\0'
  
  return size;
}

/**
 * Recreate the DROPFILES* structure.
 * Call GetDropfilesSize() to get the structuer size
 * @param DROPFILES* df the structure we want to repopulate.
 * @param const size_t maxSize the maximum amount of space we have available to use.
 *                             this is the memory size, not the number of character space.
 *                             so the total size must include everything.
 * @return boolean if this worked or not.
 */
bool ClipboardDropData::PopulateDropFiles(DROPFILES *df, const size_t maxSize) const
{
  size_t offset = maxSize;
  offset -= sizeof(DROPFILES);

  // where we will start putting the files.
  df->pFiles = sizeof(DROPFILES); 
  
  // we are using wide chars.
  df->fWide = TRUE;
  
  // the pointer that will contain all the file.
  wchar_t* ptr = (wchar_t*)(df + 1);

  for (DropFiles::const_iterator it = Files().begin(); it != Files().end(); ++it)
  {
    // get the number of characters in the file.
    // the number of characters is the number of wchar_t, not the size it takes in memory.
    int len = (*it).size();

    // check that we have space for that.
    // as we are checking the memory, we * sizeof(wchar_t)
    offset -= len * sizeof(wchar_t);
    if (offset < 0)
    {
      //  this will not work.
      return false;
    }

    // copy the file name to the pointer.
    memcpy(ptr, (*it).c_str(), len * sizeof(wchar_t));
    ptr += len;

    // and a single '\0' for the next file, (if we have one).
    offset -= sizeof(L'\0');
    if (offset < 0)
    {
      //  this will not work.
      return false;
    }
    *ptr = L'\0';
    ptr++;
  }

  offset -= sizeof(L'\0');
  if (offset < 0)
  {
    //  this will not work.
    return false;
  }

  // add the final character.
  *ptr = L'\0';

  // success
  return true;
}