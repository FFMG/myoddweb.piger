#pragma once

class ClipboardDropData
{
protected:
  ClipboardDropData();

public:
  virtual ~ClipboardDropData();

public:
  static ClipboardDropData* FromDROPFILES(DROPFILES* dropFiles);

  // return all the files.
  const std::vector<STD_TSTRING>& Files() const { return _files; }
  int GetDropfilesSize() const;
  bool PopulateDropFiles(DROPFILES *df, const int maxSize ) const;

protected:
  //
  typedef std::vector<STD_TSTRING> DropFiles;
  DropFiles _files;

private:
  // ty shall not copy...
  DISALLOW_COPY_AND_ASSIGN(ClipboardDropData);
};

