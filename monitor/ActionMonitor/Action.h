// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Action_h__
#define __Action_h__

#pragma once

#include <vector>

class ActiveAction;
class Action  
{
public:
	Action();
  
  const Action& operator=(const Action& );
  Action( const Action&);
  Action( LPCTSTR szCommand, LPCTSTR szPath = NULL );

	virtual ~Action();

  void Reset();
  void SetCommandPath( LPCTSTR szPath );

  // Do that action with the arguments passed
  // if we have no argument then we look in the clipboard
  virtual ActiveAction* CreateActiveAction(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const;

  // Same as CreateActiveAction( ... ) but we don't get anything from the clipboard
  // only will use what was given to us without further checks.
  ActiveAction* CreateActiveActionDirect(CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged ) const;

  //  convert to a LPCTSTR
  const MYODD_STRING& Command() const;
  
  // the lenght of the command.
  size_t Len() const { return _szCommand.length();}

protected:
  ActiveAction* CreateActiveActionWithNoCommandLine(CWnd* pWnd, bool isPrivileged ) const;

protected:
  MYODD_STRING toSingleLine( LPCTSTR  ) const;

public:
  // ----------------------------
  //  this is the full file name + extentions
  const MYODD_STRING& File() const { return m_szFile; }
  const MYODD_STRING& Extension() const { return m_szExt; }

  static bool Execute( const std::vector<MYODD_STRING>& argv, bool isPrivileged );

private:  
  MYODD_STRING _szCommand;  //  the command line only.
  MYODD_STRING m_szFile;     //  the full path+extention
  MYODD_STRING m_szExt;      //  the extention
};

#endif // __Action_h__
