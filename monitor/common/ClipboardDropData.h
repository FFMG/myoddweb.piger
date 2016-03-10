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

protected:
  //
  typedef std::vector<STD_TSTRING> DropFiles;
  DropFiles _files;
};

