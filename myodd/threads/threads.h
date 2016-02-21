#pragma once
#ifndef _Threads_h
#define _Threads_h

namespace myodd{ namespace threads{
  class CritSection
  {
    CritSection( const CritSection& );
    const CritSection& operator=( const CritSection& ); // disable copy

  public:
    CritSection();
    virtual ~CritSection();

    virtual bool Try();
    virtual void Lock();
    virtual void Unlock();

  private:
    CRITICAL_SECTION    _cs;
  };  //  CritSection

  // locks a critical section, and unlocks it automatically
  // when the lock goes out of scope
  class AutoLockTry
  {
    // make copy constructor and assignment operator inaccessible
    AutoLockTry(const AutoLockTry& );
    const AutoLockTry& operator=(const AutoLockTry& );

  protected:
    CritSection& _pLock;
    bool _HasLock;
  public:
    AutoLockTry( const CritSection& plock ) : 
        _pLock( *(const_cast<CritSection*>(&plock)) ),
        _HasLock( false )
        {
          // lock while in a const function...
          _HasLock = _pLock.Try();
        }

    AutoLockTry( CritSection& plock ) : 
        _pLock( plock ),
        _HasLock( false )
        {
          _HasLock = _pLock.Try();
        };

    ~AutoLockTry() 
    {
      if( HasLock() )
      {
        _pLock.Unlock();
      }
    }; // AutoLockTry

    bool HasLock() const{ return _HasLock;}
    bool TryLock( unsigned int numberOfTries )
    {
      for( unsigned int i = 0; !HasLock() && i < numberOfTries; ++i )
      {
        Sleep(0);
        _HasLock = _pLock.Try();
      }
      return HasLock();
    }
  };

  // locks a critical section, and unlocks it automatically
  // when the lock goes out of scope
  class AutoLock 
  {
    // make copy constructor and assignment operator inaccessible
    AutoLock(const AutoLock& );
    const AutoLock& operator=(const AutoLock& );

  protected:
    CritSection& _pLock;

  public:
    AutoLock( const CritSection& plock ) : 
      _pLock( *(const_cast<CritSection*>(&plock)) )
    {
      // lock while in a const function...
      _pLock.Lock();
    }

    AutoLock( CritSection& plock ) : 
      _pLock( plock )
    {
      _pLock.Lock();
    };

    ~AutoLock() 
    {
      _pLock.Unlock();
    }; // AutoLock
  };
} //  threads
} //  myodd
#endif // _Threads_h
