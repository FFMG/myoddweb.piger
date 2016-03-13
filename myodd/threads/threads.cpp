#include "stdafx.h"

#include <windows.h>
#include "threads.h"

namespace myodd{ namespace threads{
  CritSection::CritSection()
  {
    InitializeCriticalSection( &_cs );
  }
  
  CritSection::~CritSection()
  {
    DeleteCriticalSection( &_cs );
  }

  /**
   * Enter the critical section and ensure that only one thread uses the resources.
   * @param none
   * @return none
   */
  void CritSection::Lock() 
  {
    EnterCriticalSection( &_cs );
  };

  /**
   * Leave the critical section and release the resources to other threads.
   * @param none
   * @return none
   */
  void CritSection::Unlock() 
  {
    LeaveCriticalSection( &_cs );
  };

  /**
   * Attempts to enter a critical section without blocking. If the call is successful, 
   * the calling thread takes ownership of the critical section.
   * @param none
   * @return bool true if we managed to enter the critical section, false otherwise.
   */
  bool CritSection::Try()
  {
    return ( 0 != TryEnterCriticalSection( &_cs ) );
  }
} //  threads
} //  myodd