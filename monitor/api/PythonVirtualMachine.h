#pragma once

#include "pyapi.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine();
  ~PythonVirtualMachine();

  void Initialize(); 

  int LoadFile( LPCTSTR pyFile );
  static bool IsPyExt( LPCTSTR ext );

protected:
  bool m_isInitialized;
};
