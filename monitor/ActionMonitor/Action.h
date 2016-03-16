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
  virtual ActiveAction* CreateActiveAction( const STD_TSTRING& szCommandLine, bool isPrivileged) const;

  // Same as CreateActiveAction( ... ) but we don't get anything from the clipboard
  // only will use what was given to us without further checks.
  ActiveAction* CreateActiveActionDirect(const STD_TSTRING& szCommandLine, bool isPrivileged ) const;

  //  convert to a LPCTSTR
  const STD_TSTRING& Command() const;
  
  // the lenght of the command.
  size_t Len() const { return _szCommand.length();}

protected:
  ActiveAction* CreateActiveActionWithNoCommandLine( bool isPrivileged ) const;

protected:
  STD_TSTRING toSingleLine( LPCTSTR  ) const;

public:
  // ----------------------------
  //  this is the full file name + extentions
  const STD_TSTRING& File() const { return m_szFile; }
  const STD_TSTRING& Extension() const { return m_szExt; }

public:
  static bool Execute( const std::vector<STD_TSTRING>& argv, bool isPrivileged );

private:  
  STD_TSTRING _szCommand;  //  the command line only.
  STD_TSTRING m_szFile;     //  the full path+extention
  STD_TSTRING m_szExt;      //  the extention
};

#endif // __Action_h__
