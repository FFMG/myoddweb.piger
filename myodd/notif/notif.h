#ifndef _Notify_H_
#define _Notify_H_
#pragma once

#include "../myodd.h"
#include "../threads/threads.h"
#include <assert.h>
#include <vector>

namespace myodd{ namespace notif{

template <typename T>
class Notifications : protected myodd::threads::CritSection
{
protected:
  struct _NOTIFY_LPARAM
  {
    T fnNotif;
    MYODD_LPARAM lParam;
  };
  typedef std::vector<_NOTIFY_LPARAM > NOTIFY_VECTOR;

  // all the functions
  NOTIFY_VECTOR m_vectorOfFunctions;

  /**
   * Copy notifications
   * @param const Notifications& the source notification
   * @return none.
   */
  virtual void _Copy( const Notifications& nf )
  {
    if( this == &nf )
    {
      return; //  no need to copy ourselves
    }

    m_iNotificationCount = nf.m_iNotificationCount;
    m_vectorOfFunctions  = nf.m_vectorOfFunctions;
  }
public:
  Notifications() :
      m_iNotificationCount( 0 )
  {
  }

  Notifications( const Notifications& nf )
  {
    _Copy( nf );
  }

  virtual ~Notifications()
  {
    // this means that someone did not remove themselves properly
    // this could be the cause for an ugly bug as we might have been 'notifying' a class that
    // was deleted a long time ago.
    // remove notifications in the destructor that way you KNOW that it will not be called randomly.
#ifdef _DEBUG
    if( m_vectorOfFunctions.size() > 0 )
      assert( m_vectorOfFunctions.size() == 0 );
#endif
  }

  const Notifications& operator=( const Notifications& nf )
  {
    _Copy( nf );
    return *this;
  }

  /**
   * Add an long/Function to the list of items
   * @param T the typename of the function
   * @param MYODD_LPARAM the param we want to add
   * @return bool true if the item was added, false if it already exists.
   */
  virtual bool AddNotif( const T& fnNotif, MYODD_LPARAM lParam )
  {
    myodd::threads::AutoLock autoLock( *this );
    // Make sure that this notification does not already exists.
    // it would not make sense to have it more than once.
    for( NOTIFY_VECTOR::const_iterator it = m_vectorOfFunctions.begin();
         it != m_vectorOfFunctions.end();
         ++it )
    {
      if( (*it).fnNotif == fnNotif && (*it).lParam == lParam )
      {
        return false; //  already exists.
      }
    }

    _NOTIFY_LPARAM lp = {fnNotif, lParam};

    // just add this function and lParam.
    m_vectorOfFunctions.push_back( lp );
    return true;
  }

  /**
   * Add an long/Function to the list of items
   * @param T the typename of the function
   * @param void* the param we want to add
   * @return bool true if the item was added, false if it already exists.
   */
  virtual bool AddNotif( const T& fnNotif, void* lpVoid )
  {
    // just add this function and lParam.
    return AddNotif( fnNotif, (MYODD_LPARAM)lpVoid );
  }

  /**
   * Get the total number of notifications in the vector.
   * This is the total number of notifs called.
   * @param none
   * @return size_t the number of notifications.
   */
  size_t GetNumberOfNotifs() const
  {
    return (m_vectorOfFunctions.size());
  }

  /**
   * Get the next function/MYODD_LPARAM in the list.
   * If we have no more MYODD_LPARAM and/or functions then we will return false
   * @param unsigned int the index number of the notification we want.
   * @param T the typename of the function
   * @param MYODD_LPARAM the param we want to remove
   * @return bool if a function was found or not.
   */
  bool GetNotif( unsigned int idx, T& fnNotif, MYODD_LPARAM& lParam )
  {
    if( idx >= m_vectorOfFunctions.size() )
    {
      return false;
    }

    if( AreNotificationBlocked() )
    {
      return false;
    }
    
    NOTIFY_VECTOR::const_iterator it = m_vectorOfFunctions.begin() + idx;
    lParam  = (*it).lParam;
    fnNotif = (*it).fnNotif;

    return true;
  }

  /**
   * Remove a MYODD_LPARAM from the list. If the list is empty then we will remove it.
   * @param T the typename of the function
   * @param MYODD_LPARAM the param we want to remove
   * @return bool if something was removed we will return true.
   */
  bool RemoveNotif( const T& fnNotif, MYODD_LPARAM lParam )
  {
    myodd::threads::AutoLock autoLock( *this );

    // Make sure that this notification does not already exists.
    // it would not make sense to have it more than once.
    for( NOTIFY_VECTOR::const_iterator it = m_vectorOfFunctions.begin();
         it != m_vectorOfFunctions.end();
         ++it )
    {
      if( (*it).fnNotif == fnNotif && (*it).lParam == lParam )
      {
        m_vectorOfFunctions.erase( it );
        return true; // we found it and removed it.
      }
    }
    
    // if we are here we never found it.
    return false;
  }

  /**
   * Remove a MYODD_LPARAM from the list. If the list is empty then we will remove it.
   * @param T the typename of the function
   * @param void* the pointer we want to remove
   * @return bool if something was removed we will return true.
   */
  bool RemoveNotif( const T& fnNotif, void* lpVoid )
  {
    return RemoveNotif( fnNotif, (MYODD_LPARAM)lpVoid );
  }

  /**
   * Remove all the notifications.
   * @return none.
   */
  void RemoveAllNotifs( )
  {
    myodd::threads::AutoLock autoLock( *this );

    // just delete the whole lot
    m_vectorOfFunctions.erase( m_vectorOfFunctions.begin(), m_vectorOfFunctions.end() );
  }

  /**
   * remove a stop notification to the reference counter.
   * If the reference is more than 0 then no notifications are sent
   * @param none
   * @return none
   */
  virtual void UnblockNotifications()
  {
    myodd::threads::AutoLock autoLock( *this );

    //  how can this be more than 0
    // there should only be one Stop/Start.
    ASSERT(m_iNotificationCount > 0 );
    if( m_iNotificationCount > 0 )
      --m_iNotificationCount;
  }

  /**
   * Add a stop notification to the reference counter.
   * If the reference is more than 0 then no notifications are sent
   * @param none
   * @return none
   */
  virtual void BlockNotifications()
  {
    myodd::threads::AutoLock autoLock( *this );
    ++m_iNotificationCount;
  }

  /**
   * Check if all notifications are blocked
   * When this happens we don't send any notifications.
   * @param none
   * @return bool if notifications are blocked or not.
   */
  virtual bool AreNotificationBlocked() const
  {
    return (m_iNotificationCount > 0);
  }

private:
  unsigned int m_iNotificationCount;
};

typedef void ( *NOTIFY_DEFAULT )(MYODD_LPARAM lParam);
class NotificationsDefault : public Notifications<NOTIFY_DEFAULT>
{
public:
  NotificationsDefault() : Notifications<NOTIFY_DEFAULT>()
  {
  }

  NotificationsDefault( const NotificationsDefault& nd)
  {
    _Copy( nd );
  }

  const NotificationsDefault& operator=( const NotificationsDefault& nd )
  {
    _Copy( nd );
    return *this;
  }

  virtual void _Copy( const NotificationsDefault& nf )
  {
    __super::_Copy( nf );
  }

  virtual ~NotificationsDefault()
  {
  }

  /**
   * Call all the functions with the default param.
   * @return none.
   */
  void Notify()
  {
    MYODD_LPARAM lParam;
    NOTIFY_DEFAULT fnNotif;
    unsigned idx = 0;
    while( GetNotif( idx++, fnNotif, lParam ) )
    {
      (*fnNotif)( lParam );
    }
  }
};

} //  config
} //  notif
#endif // _Notify_H_