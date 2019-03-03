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
};

