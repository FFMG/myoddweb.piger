#pragma once

class ClipboardData
{
protected:
  ClipboardData();

public:
  virtual ~ClipboardData();

protected:
  static ClipboardData* FromFromClipboardHDrop(HGLOBAL hData);
  static ClipboardData* FromFromClipboardEnhmetafile(HGLOBAL hData);
  static ClipboardData* FromFromClipboardBitmap(HGLOBAL hData);
  static ClipboardData* FromFromClipboardDefault( UINT format, HGLOBAL hData);

  static wchar_t* GetClipboardName( UINT format );

public:
  static ClipboardData* FromFromClipboard(UINT format);

private:
  // ty shall not copy...
  ClipboardData(const ClipboardData&cf);
  const ClipboardData& operator= (const ClipboardData&cf);

public:
  unsigned char* data;
  SIZE_T  dataSize;
  UINT uFormat;
  wchar_t* dataName;

  //  reset all the data
  void Free();
  void FreeClipboardName();
  void FreeClipboardEnhmetafile();
  void FreeClipboardDefault();
  void NullAll();
};