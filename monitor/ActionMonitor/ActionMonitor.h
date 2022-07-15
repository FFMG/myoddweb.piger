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
#include "IApplication.h"

#define _MAX_CMD_LINE_ARGS  128

class CActionMonitorApp final : public CWinApp
{
public:
	CActionMonitorApp();
  virtual ~CActionMonitorApp();
  
	virtual BOOL InitInstance();

  // Implementation
	DECLARE_MESSAGE_MAP()

  static bool InitConfig(const ::myodd::commandline_variables& vm);
  static void InitLog();
  static void InitReservedPaths();
  void InitMaxClipboardSize();

public:
  static size_t GetMaxClipboardMemory();

public:
  bool CreateAndShowActionDialog();

  //  virtual int ExitInstance();
  int ExitInstance() override;

  // check Mutext and so on
  bool CanStartApp();

  // self elevate us to admin.
  static void SelfElavate();

  /**
   * \brief the handle of the mutex, this is to ensure that we only have once 
   *        instance of the app running at a time.
   */
  HANDLE _mutex;

  /**
   * \brief The maximum memory we want to use when getting clipboard data.
   */
  size_t _maxClipboardSize;

private:
  /**
   * \brief the application
   */
  IApplication* _application;
};

CActionMonitorApp& App();
