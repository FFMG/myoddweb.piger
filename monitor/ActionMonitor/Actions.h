// Actions.h: interface for the Actions class.
//////////////////////////////////////////////////////////////////////

#ifndef __Actions_h__
#define __Actions_h__

#pragma once
#include "ActionsCore.h"
#include "Action.h"
#include <mutex>

// the name of protected directories
static LPCTSTR AM_DIRECTORY_IN  = _T("__in");           //  dir of actions that will run at start
static LPCTSTR AM_DIRECTORY_OUT = _T("__out");          //  dir of actions that will run at exit time
static LPCTSTR AM_DIRECTORY_TMP = _T("__tmp");          //  dir that will not be run/loaded - used for files/app needed by other actions
static LPCTSTR AM_DIRECTORY_PLUGIN = _T("__plugins");   //  dir that will not be run/loaded - used for files/app needed by other actions

class Actions  
{
public:
	Actions();
	virtual ~Actions();

  bool Add( Action* action );
  bool Add( LPCTSTR szText, LPCTSTR szPath );
  bool Remove( LPCTSTR szText, LPCTSTR szPath );
  bool Find( UINT idx, LPCTSTR szText, MYODD_STRING& stdPath );

  MYODD_STRING toChar( ) const;

protected:
  DISALLOW_COPY_AND_ASSIGN(Actions);

protected:

  struct COMMANDS_VALUE
  {
    MYODD_STRING color;
    bool bBold;
    bool bItalic;
    COMMANDS_VALUE() :
      bBold  (0),
      bItalic(0)
    {
    }
    COMMANDS_VALUE( MYODD_STRING c, bool b, bool i ) :
      bBold( b), bItalic( i ), color( c ) 
    {
    }
  protected:
    DISALLOW_COPY_AND_ASSIGN(COMMANDS_VALUE);
  };

  static MYODD_STRING toChar( const MYODD_STRING& s, const COMMANDS_VALUE& cv );
  void GetCommandValue( LPCTSTR lpName, COMMANDS_VALUE& cv ) const;

protected:
  //  vectors containing all the commands we can call
  typedef std::vector<Action*> array_of_actions;
  typedef Actions::array_of_actions::const_iterator array_of_actions_it;
  array_of_actions m_Actions;
  array_of_actions m_ActionsMatch;

  array_of_actions::const_iterator Find( const MYODD_STRING& szText, const MYODD_STRING& szPath );
public:
  //  select one item down in the list
  void down();

  //  select one item up in the list
  void up();

protected:
  virtual void ParseDirectory( LPCTSTR  rootDir, LPCTSTR  extentionDir );

public:
  virtual void Init();

protected:
  // clear the search string and posible actions match list.
  void ClearSearch();

  // the command currently chosen
  size_t m_uCommand;

public:
  const Action* GetCommand( MYODD_STRING* cmdLine = NULL ) const;
  void SetAction( Action* tmpAction );

protected:
  Action* m_tmpAction;

public:
  /**
   * Get the current action as it was entered by the user.
   * This is the action that the user typed in.
   * So if the action is "Google" but the user typed if "Goo" then we will only return "Goo"
   *
   * @return const MYODD_STRING& the current action.
   */
  inline const MYODD_STRING& getActionAsTyped() const
  { 
    return m_sActionAsTyped;
  }

  void CurrentActionReset();
  void CurrentActionAdd( TCHAR c );
  void CurrentActionBack();

  void ClearAll();
protected:
  // this is the text that the user is currently typing
  MYODD_STRING m_sActionAsTyped;

protected:
  size_t BuildMatchList( );
  size_t BuildMatchList( std::vector<MYODD_STRING>& exploded );

protected:
  virtual bool IsReservedDir( LPCTSTR ) const;

  std::mutex _mutex;
};
#endif // __Actions_h__
