#pragma once
#include "ITray.h"
#include "TrayWnd.h"
#include "CommonWnd.h"
#include "IApplication.h"

class ApplicationTray : public ITray, protected CommonWnd
{
public:
  ApplicationTray();
  virtual ~ApplicationTray();

  void SetActive() override;
  void SetInactive() override;

  void SetApplication( IApplication* application );
protected:

  HMENU _menu;

  TrayWnd* _tray;
  IApplication* _application;

  LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

  void OnTrayExit();
  void OnTrayVersion();
  void OnTrayReload();
};

