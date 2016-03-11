// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Action_h__
#define __Action_h__

#pragma once

#include <vector>

//  the clipboard code
#include "../common/clipboard.h"

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
  virtual bool DoIt( const STD_TSTRING& szCommandLine, bool isPrivileged);

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

  // ----------------------------
  const Clipboard& GetClipBoard() const{ return m_clipBoard;}

protected:
  bool DoItWithNoCommandLine( bool isPrivileged );

protected:
  STD_TSTRING toSingleLine( LPCTSTR  ) const;
  Clipboard m_clipBoard;
  

public:
  static bool Execute( const std::vector<STD_TSTRING>& argv, bool isPrivileged );

private:  
  STD_TSTRING m_szCommand;  //  the command line only.
  STD_TSTRING m_szFile;     //  the full path+extention
  STD_TSTRING m_szExt;      //  the extention
};

#endif // __Action_h__
