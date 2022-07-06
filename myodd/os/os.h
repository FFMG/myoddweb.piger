#pragma once

#ifndef __myodd_Os_h
#define __myodd_Os_h

#include "../common/includes.h"

namespace myodd{ namespace os{
  typedef enum _pe_architecture {
    ARCHITECTURE_UNKNOWN = 0x0000,
    ARCHITECTURE_ANYCPU = 0x0001,
    ARCHITECTURE_X86 = 0x010B,
    ARCHITECTURE_x64 = 0x020B
  } ARCHITECTURE;

  ARCHITECTURE GetImageArchitecture(const std::wstring& modulePath);
  ARCHITECTURE GetImageArchitecture(const wchar_t* modulePath);

  // check if the current process is elevated or not.
  bool IsElevated();
} //  os
} //  myodd
#endif __myodd_Os_h