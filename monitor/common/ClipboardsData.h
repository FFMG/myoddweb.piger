#pragma once

class ClipboardsData
{
public:
  ClipboardsData();
  ClipboardsData( const ClipboardsData& rhs );
  virtual ~ClipboardsData();

  const ClipboardsData& operator=(const ClipboardsData& rhs);

  void SetText(const MYODD_STRING& text );
  void AddPossibleFileName(const MYODD_STRING& possibleFilename);
  void Reset();

public:
  // accessors.
  bool GetFolder(MYODD_STRING& sText, size_t idx, bool bQuote ) const;
  bool GetFile(MYODD_STRING& sText, size_t idx, bool bQuote) const;
  bool GetURL(MYODD_STRING& sText, size_t idx, bool bQuotec) const;

  bool GetTextFromClipboard(MYODD_STRING& sText, bool bQuote ) const;
  bool GetText(MYODD_STRING& sText, bool bQuote ) const;

protected:
  static MYODD_STRING Quote( const MYODD_STRING& sSrc, bool bQuote );

protected:
  MYODD_STRING _text;
  
  typedef std::map<MYODD_STRING, MYODD_STRING > DataFiles;
  DataFiles _fileNames;
};