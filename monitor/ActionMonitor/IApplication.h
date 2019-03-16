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

class IApplication
{
public:
  IApplication();
  virtual ~IApplication();

  IApplication(const IApplication&) = delete;
  void operator=(const IApplication&) = delete;

  /**
   * \brief show the main window.
   */
  virtual void Show() = 0;

  /**
   * \brief close the main window, (if open), and whatever else needs to be closed.
   *        also send out messages as needed.
   */
  virtual void Close() = 0;

  /**
   * \brief restart the application completely.
   */
  virtual void Restart() = 0;

  /**
   * \brief show the app version number.
   */
  virtual void ShowVersion() = 0;

  /**
   * \brief show all the start actions
   */
  virtual void ShowStart() = 0;

  /**
   * \brief show all the end actions
   */
  virtual void ShowEnd() = 0;

  /**
   * \brief Add an action to our list of actions.
   * \param action the action we are adding.
   * \return success or not.
   */
  virtual bool AddAction(IAction* action) = 0;

  /**
   * \brief Remove an action, if more than one action is found
   *        Then the path will be compared against.
   * \param szText the action we want to remove
   * \param szPath the path of the action we are removing.
   * \return bool if the action was removed or not.
   */
  virtual bool RemoveAction( const std::wstring& szText, const std::wstring& szPath) const = 0;

  /**
   * \brief Find an action to see if it exists already
   * \param idx the index of the action we are looking for.
   * \param szText the name of the command we want to find
   * \return if the action exits or not.
   */
  virtual const IAction* FindAction(unsigned int idx, const std::wstring& szText) const = 0;

  /**
   * \brief ask the os to execute a command
   * \param argv the arguments we will be using/passing, (the first argument is the exe/batch/cmd itself)
   * \param isPrivileged if set to true, we will try and elevate the application
   * \param hProcess the created handle so we can close it
   */
  virtual bool Execute(const std::vector<std::wstring>& argv, const bool isPrivileged, HANDLE* hProcess) const = 0;
};
