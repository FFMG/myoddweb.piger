#pragma once

class ClipboardDropData
{
protected:
  ClipboardDropData();

public:
  virtual ~ClipboardDropData();

public:
  static ClipboardDropData* FromDROPFILES(DROPFILES* dropFiles, size_t maxMemory);

  // return all the files.
  const std::vector<MYODD_STRING>& Files() const { return _files; }
  size_t GetDropfilesSize() const;
  bool PopulateDropFiles(DROPFILES *df, const size_t maxSize ) const;

protected:
  static size_t CalculateBits(DROPFILES *df);

protected:
  //
  typedef std::vector<MYODD_STRING> DropFiles;
  DropFiles _files;

private:
  // ty shall not copy...
  DISALLOW_COPY_AND_ASSIGN(ClipboardDropData);
};

