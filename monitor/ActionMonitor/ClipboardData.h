#pragma once

class ClipboardData
{
protected:
  ClipboardData();

public:
  virtual ~ClipboardData();

protected:
  static ClipboardData* FromClipboardHDrop(HGLOBAL hData);
  static ClipboardData* FromClipboardEnhmetafile(HGLOBAL hData);
  static ClipboardData* FromClipboardBitmap(HGLOBAL hData);
  static ClipboardData* FromClipboardDefault( UINT format, HGLOBAL hData);

  static wchar_t* GetClipboardName( UINT format );

public:
  static ClipboardData* FromClipboard(UINT format);

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