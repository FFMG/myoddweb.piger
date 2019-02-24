//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once
#include "Action.h"
#include <mutex>
#include "IActions.h"

// the name of protected directories
static const wchar_t* AM_DIRECTORY_IN  = L"__in";           //  dir of actions that will run at start
static const wchar_t* AM_DIRECTORY_OUT = L"__out";          //  dir of actions that will run at exit time
static const wchar_t* AM_DIRECTORY_TMP = L"__tmp";          //  dir that will not be run/loaded - used for files/app needed by other actions
static const wchar_t* AM_DIRECTORY_PLUGIN = L"__plugins";   //  dir that will not be run/loaded - used for files/app needed by other actions

class Actions : public IActions
{
public:
	Actions();
	virtual ~Actions();

protected:
  DISALLOW_COPY_AND_ASSIGN(Actions);

protected:
  struct COMMANDS_VALUE
  {
    std::wstring color;
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
  void GetCommandValue( const std::wstring& lpName, COMMANDS_VALUE& cv ) const;

protected:
  //  vectors containing all the commands we can call
  typedef std::vector<Action*> array_of_actions;
  typedef array_of_actions::const_iterator array_of_actions_it;
  array_of_actions m_Actions;
  array_of_actions m_ActionsMatch;

  array_of_actions::const_iterator Find( const MYODD_STRING& szText, const MYODD_STRING& szPath );

  virtual void ParseDirectory( LPCTSTR  rootDir, LPCTSTR  extentionDir );

protected:
  // clear the search string and posible actions match list.
  void ClearSearch();

  // the command currently chosen
  size_t m_uCommand;

protected:
  Action* m_tmpAction;

public:
  /**
   * \brief Get the current action as it was entered by the user.
   *        This is the action that the user typed in.
   *        So if the action is "Google" but the user typed if "Goo" then we will only return "Goo"
   *
   * \return The current action as typed by the user.
   */
  const std::wstring& getActionAsTyped() const
  { 
    return m_sActionAsTyped;
  }

  void CurrentActionReset() override;
  void CurrentActionAdd( wchar_t c ) override;
  void CurrentActionBack() override;
  void SetAction(Action* tmpAction) override;
  const Action* GetCommand() const override;
  const std::wstring GetCommandLine() const override;
  const std::wstring toChar() const override;
  void down() override;
  void up() override;
  void Initialize() override;
  bool Add(Action* action) override;
  bool Remove(const std::wstring& szText, const std::wstring& szPath) override;
  const Action* Find(const std::wstring& szText, unsigned int idx ) override;

  void ClearAll();
protected:
  // this is the text that the user is currently typing
  MYODD_STRING m_sActionAsTyped;

protected:
  size_t BuildMatchList( );
  size_t BuildMatchList( std::vector<MYODD_STRING>& exploded );

  virtual bool IsReservedDir(const wchar_t*) const;

  std::mutex _mutex;
};
