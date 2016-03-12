// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Action_h__
#define __Action_h__

#pragma once

#include <vector>

class Action  
{
public:
	Action();
  
  const Action& operator=(const Action& );
  Action( const Action&);
  Action( LPCTSTR szText, LPCTSTR szPath = NULL );

	virtual ~Action();

  void Reset();
  void SetCommandPath( LPCTSTR szPath );

  // Do that action with the arguments passed
  // if we have no argument then we look in the clipboard
  virtual bool DoIt( const STD_TSTRING& szCommandLine, bool isPrivileged) const;

  // Same as DoIt( ... ) but we don't get anything from the clipboard
  // only will use what was given to us without further checks.
  bool DoItDirect(const STD_TSTRING& szCommandLine, bool isPrivileged ) const;

  //  convert to a LPCTSTR
  LPCTSTR toChar() const;

  // ----------------------------
  //  this is the full file name + extentions
  LPCTSTR CommandToFile() const  { return m_szFile.c_str();  }

  // ----------------------------
  size_t len() const { return m_szCommand.length();}

protected:
  bool DoItWithNoCommandLine( bool isPrivileged ) const;

#ifdef ACTIONMONITOR_API_PLUGIN
  bool DoItDirectPlugin( bool isPrivileged) const;
#endif

#ifdef ACTIONMONITOR_API_PY
  bool DoItDirectPython( bool isPrivileged ) const;
#endif // ACTIONMONITOR_API_PY

#ifdef ACTIONMONITOR_API_LUA
  bool DoItDirectLua( bool isPrivileged ) const;
#endif // ACTIONMONITOR_API_LUA

protected:
  STD_TSTRING toSingleLine( LPCTSTR  ) const;
  
public:
  static bool Execute( const std::vector<STD_TSTRING>& argv, bool isPrivileged );

private:  
  STD_TSTRING m_szCommand;  //  the command line only.
  STD_TSTRING m_szFile;     //  the full path+extention
  STD_TSTRING m_szExt;      //  the extention
};

#endif // __Action_h__
