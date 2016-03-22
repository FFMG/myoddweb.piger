#pragma once

class ClipboardData
{
protected:
  ClipboardData();

public:
  virtual ~ClipboardData();

protected:

  static ClipboardData* FromClipboardHDrop(HGLOBAL hData, size_t maxMemory);
  static ClipboardData* FromClipboardEnhmetafile(HGLOBAL hData, size_t maxMemory);
  static ClipboardData* FromClipboardBitmap(HGLOBAL hData, size_t maxMemory);
  static ClipboardData* FromClipboardDefault( UINT format, HGLOBAL hData, size_t maxMemory);

  static wchar_t* GetClipboardName( UINT format );

public:
  static ClipboardData* FromClipboard(UINT format, size_t maxMemory);

  void ToClipboard();

public:
  void* data;
  SIZE_T  dataSize;
  UINT uFormat;
  wchar_t* dataName;

  //  reset all the data
  void Free();
  void FreeClipboardName();
  void FreeClipboardEnhmetafile();
  void FreeClipboardHDrop();
  void FreeClipboardBitmap();
  void FreeClipboardDefault();
  void NullAll();

  static HBITMAP CopyBitmap(HBITMAP hBitmapSrc);

private:
  // ty shall not copy...
  DISALLOW_COPY_AND_ASSIGN(ClipboardData);
};