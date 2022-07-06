#pragma once

class ClipboardsData
{
public:
  ClipboardsData();
  ClipboardsData( const ClipboardsData& rhs );
  virtual ~ClipboardsData();

  const ClipboardsData& operator=(const ClipboardsData& rhs);

  void SetText(const std::wstring& text );
  void AddPossibleFileName(const std::wstring& possibleFilename);
  void Reset();

public:
  // accessors.
  bool GetFolder(std::wstring& sText, size_t idx, bool bQuote ) const;
  bool GetFile(std::wstring& sText, size_t idx, bool bQuote) const;
  bool GetUrl(std::wstring& sText, size_t idx, bool bQuotec) const;

  bool GetTextFromClipboard(std::wstring& sText, bool bQuote ) const;
  bool GetText(std::wstring& sText, bool bQuote ) const;

protected:
  static std::wstring Quote( const std::wstring& sSrc, bool bQuote );

protected:
  std::wstring _text;
  
  typedef std::map<std::wstring, std::wstring > DataFiles;
  DataFiles _fileNames;
};