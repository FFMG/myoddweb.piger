#include "os.h"
#include <Windows.h>
#define MKPTR(p1,p2) ((DWORD_PTR)(p1) + (DWORD_PTR)(p2))

namespace myodd { namespace os {

bool IsElevated()
{
  bool fRet = false;
  HANDLE hToken = NULL;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
    TOKEN_ELEVATION Elevation;
    unsigned long cbSize = sizeof(TOKEN_ELEVATION);
    if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
      fRet = Elevation.TokenIsElevated > 0;
    }
  }
  if (hToken) {
    CloseHandle(hToken);
  }
  return fRet;
}

void* GetOffsetFromRva(IMAGE_DOS_HEADER *pDos, IMAGE_NT_HEADERS *pNt, unsigned long rva) {
  IMAGE_SECTION_HEADER *pSecHd = IMAGE_FIRST_SECTION(pNt);
  for (unsigned long i = 0; i < pNt->FileHeader.NumberOfSections; ++i, ++pSecHd) {
    // Lookup which section contains this RVA so we can translate the VA to a file offset
    if (rva >= pSecHd->VirtualAddress && rva < (pSecHd->VirtualAddress + pSecHd->Misc.VirtualSize)) {
      unsigned long delta = pSecHd->VirtualAddress - pSecHd->PointerToRawData;
      return (void*)MKPTR(pDos, rva - delta);
    }
  }
  return NULL;
}

ARCHITECTURE GetImageArchitecture(IMAGE_DOS_HEADER *pDosHd) 
{
  if (IsBadReadPtr(pDosHd, sizeof(pDosHd->e_magic)) || pDosHd->e_magic != IMAGE_DOS_SIGNATURE)
    return ARCHITECTURE_UNKNOWN;

  // Parse and validate the NT header
  IMAGE_NT_HEADERS *pNtHd = (IMAGE_NT_HEADERS*)MKPTR(pDosHd, pDosHd->e_lfanew);
  if (IsBadReadPtr(pNtHd, sizeof(pNtHd->Signature)) || pNtHd->Signature != IMAGE_NT_SIGNATURE)
    return ARCHITECTURE_UNKNOWN;

  // First, naive, check based on the 'Magic' number in the Optional Header.
  ARCHITECTURE architecture = (ARCHITECTURE)pNtHd->OptionalHeader.Magic;

  // If the architecture is x86, there is still a possibility that the image is 'AnyCPU'
  if (architecture == ARCHITECTURE_X86) 
  {
    IMAGE_DATA_DIRECTORY comDirectory = pNtHd->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
    if (comDirectory.Size) 
    {
      IMAGE_COR20_HEADER *pClrHd = (IMAGE_COR20_HEADER*)GetOffsetFromRva(pDosHd, pNtHd, comDirectory.VirtualAddress);
      
      // Check to see if the CLR header contains the 32BITONLY flag, if not then the image is actually AnyCpu
      // if pClrHd is NULL then we are not really a .NET file.
      if (pClrHd && (pClrHd->Flags & COMIMAGE_FLAGS_32BITREQUIRED) == 0)
      {
        architecture = ARCHITECTURE_ANYCPU;
      }
    }
  }

  return architecture;
}

ARCHITECTURE GetImageArchitecture(const std::wstring& modulePath)
{
  return GetImageArchitecture(modulePath.c_str());
}

ARCHITECTURE GetImageArchitecture(const wchar_t* modulePath)
{
  // is it for us?
  if (NULL == modulePath)
  {
    //  do not call freelibrary 
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/ms683199%28v=vs.85%29.aspx
    HMODULE hModule = GetModuleHandle(NULL);
    if (NULL == hModule)
    {
      return ARCHITECTURE_UNKNOWN;
    }
    return GetImageArchitecture ((IMAGE_DOS_HEADER*)hModule);
  }

  HANDLE hFile = CreateFile(modulePath, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
  if (NULL == hFile)
  {
    return ARCHITECTURE_UNKNOWN;
  }

  unsigned long fsize = GetFileSize(hFile, NULL);
  unsigned long buffersize = fsize;//+0x2000;
  BYTE *buffer = new BYTE[buffersize];
  
  ARCHITECTURE pe = ARCHITECTURE_UNKNOWN;
  unsigned long read;
  if (ReadFile(hFile, buffer, fsize, &read, NULL))
  {
    IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER*)buffer;
    pe = GetImageArchitecture(idh);
  }

  delete[]buffer;
  CloseHandle(hFile);

  return pe;
}

} //  os
} //  myodd