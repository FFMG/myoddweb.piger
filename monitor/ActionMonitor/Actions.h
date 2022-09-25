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
#include "IAction.h"
#include <threads/lock.h>
#include "IActions.h"
#include "IApplication.h"

// the name of protected directories
static const wchar_t* AM_DIRECTORY_IN  = L"__in";           //  dir of actions that will run at start
static const wchar_t* AM_DIRECTORY_OUT = L"__out";          //  dir of actions that will run at exit time
static const wchar_t* AM_DIRECTORY_TMP = L"__tmp";          //  dir that will not be run/loaded - used for files/app needed by other actions
static const wchar_t* AM_DIRECTORY_PLUGIN = L"__plugins";   //  dir that will not be run/loaded - used for files/app needed by other actions

class Actions : public IActions
{
public:
	Actions( IApplication& application );
	virtual ~Actions();

  Actions(const Actions&) = delete;
  void operator=(const Actions&) = delete;

protected:
  /**
   * \brief the application manager.
   */
  IApplication& _application;

  class CommandsValue
  {
  private:
    bool _bBold;
    bool _bItalic;

  public:
    CommandsValue() :
      _bBold  (false),
      _bItalic(false)
    {
    }
    CommandsValue( const bool bold, const bool italic ) :
      _bBold( bold ), _bItalic( italic ) 
    {
    }

    CommandsValue( const CommandsValue& rhs) : 
      _bBold(rhs._bBold), _bItalic(rhs._bItalic)
    {

    }

    const CommandsValue& operator=(CommandsValue& rhs )
    {
      if (this != &rhs)
      {
        _bBold = rhs._bBold;
        _bItalic = rhs._bItalic;
      }
      return *this;
    }

    const bool IsItalic() const{ return _bItalic; }
    const bool IsBold() const { return _bBold; }
  };

  static const std::wstring ToChar( const IAction& givenAction, const CommandsValue& cv );
  static const std::wstring ToChar( const std::wstring& givenAction );
  static const CommandsValue GetCommandValue( const std::wstring& lpName, bool bold, bool italic );

protected:
  //  vectors containing all the commands we can call
  typedef std::vector<IAction*> array_of_actions;
  typedef array_of_actions::const_iterator array_of_actions_it;
  array_of_actions _actions;
  array_of_actions _actionsMatch;

  array_of_actions::const_iterator Find( const std::wstring& szText, const std::wstring& szPath );

  virtual void ParseDirectory( const std::wstring& rootDir, const std::wstring& extentionDir );

protected:
  // clear the search string and possible actions match list.
  void ClearSearch();

  // the command currently chosen
  size_t m_uCommand;

  IAction* _actionCurrentlySelected;

public:
  /**
   * \brief Get the current action as it was entered by the user.
   *        This is the action that the user typed in.
   *        So if the action is "Google" but the user typed if "Goo" then we will only return "Goo"
   *
   * \return The current action as typed by the user.
   */
  const std::wstring& GetActionAsTyped() const;

  void CurrentActionReset() override;
  void CurrentActionAdd( wchar_t characterToAdd ) override;
  void CurrentActionBack() override;
  void SetAction(IAction* tmpAction) override;
  const IAction* GetCommand() override;
  std::wstring GetCommandLine() override;
  std::wstring ToChar() override;
  void Down() override;
  void Up() override;
  void Initialize() override;
  bool Add(IAction* action) override;
  bool Remove(const std::wstring& szText, const std::wstring& szPath) override;
  const IAction* Find(const std::wstring& szText, unsigned int idx ) override;

  void ClearAll();
protected:
  // this is the text that the user is currently typing
  std::wstring _sActionAsTyped;

protected:
  void RebuildPosibleListOfActions();
  array_of_actions GetListOfPossibleCommands( );
  array_of_actions GetListOfPossibleCommands( std::vector<std::wstring>& wordsTyped );

  virtual bool IsReservedDir(const wchar_t*) const;

  myodd::threads::Key _mutexActions;
  myodd::threads::Key _mutexActionTemp;
  myodd::threads::Key _mutexActionsMatch;
};
