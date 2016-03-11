#pragma once

class ClipboardsData
{
public:
  ClipboardsData();
  ClipboardsData( const ClipboardsData& rhs );
  virtual ~ClipboardsData();

  const ClipboardsData& operator=(const ClipboardsData& rhs);

  void SetText(const STD_TSTRING& text );
  void AddPossibleFileName(const STD_TSTRING& possibleFilename);
  void Reset();

public:
  // accessors.
  bool GetFolder(STD_TSTRING& sText, size_t idx, bool bQuote ) const;
  bool GetFile(STD_TSTRING& sText, size_t idx, bool bQuote) const;
  bool GetURL(STD_TSTRING& sText, size_t idx, bool bQuotec) const;

  bool GetTextFromClipboard(STD_TSTRING& sText, bool bQuote ) const;
  bool GetText(STD_TSTRING& sText, bool bQuote ) const;

protected:
  static STD_TSTRING Quote( const STD_TSTRING& sSrc, bool bQuote );

protected:
  STD_TSTRING _text;
  
  typedef std::map<STD_TSTRING, STD_TSTRING > DataFiles;
  DataFiles _fileNames;
};