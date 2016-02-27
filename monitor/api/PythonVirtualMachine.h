#pragma once

#include "pyapi.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine(void);
  ~PythonVirtualMachine(void);

  void Initialize(); 

  int LoadFile( LPCTSTR pyFile );
  bool IsPyExt( LPCTSTR ext ) const;

protected:
  bool m_isInitialized;
};

// the Python virtual machine
PythonVirtualMachine& GetPyVirtualMachine();
