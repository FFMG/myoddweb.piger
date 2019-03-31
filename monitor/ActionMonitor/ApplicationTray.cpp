#include "stdafx.h"
#include "ApplicationTray.h"
#include "resource.h"

ApplicationTray::ApplicationTray() : 
  CommonWnd( L"ApplicationTray"),
  _tray( nullptr ),
  _application( nullptr ),
  _menu(nullptr)
{
  _menu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_TRAY));
}

ApplicationTray::~ApplicationTray()
{
  DestroyMenu(_menu);
  _menu = nullptr;

  if (_tray != nullptr)
  {
    _tray->Close();
  }
  delete _tray;
}

void ApplicationTray::SetApplication(IApplication* application)
{
  _application = application;

  Create();

  myodd::files::Version _ver;
#ifdef _DEBUG
  auto strSay = myodd::strings::Format(_T("Action Monitor [Debug]: %d.%d.%d.%d"),
#else
  auto strSay = myodd::strings::Format(_T("Action Monitor : %d.%d.%d.%d"),
#endif
    _ver.GetFileVersionMajor(),
    _ver.GetFileVersionMinor(),
    _ver.GetFileVersionMaintenance(),
    _ver.GetFileVersionBuild());

  const auto instance = GetModuleHandle(nullptr);
  const auto active = LoadIcon(instance, (LPCTSTR)MAKEINTRESOURCE(IDR_MAINFRAME_ACTIVE));
  const auto inactive = LoadIcon(instance, (LPCTSTR)MAKEINTRESOURCE(IDR_MAINFRAME));
  _tray = new TrayWnd(active, inactive);
  _tray->Create();
  _tray->SetToolTip(strSay);
  _tray->SetMenu(_menu, GetSafeHwnd());
}

LRESULT ApplicationTray::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_COMMAND:
    switch (wParam)
    {
    case ID_TRAY_EXIT:
      OnTrayExit();
      break;

    case ID_TRAY_RELOAD:
      OnTrayReload();
      break;

    case ID_TRAY_VERSION:
      OnTrayVersion();
      break;
    }
    return 0L;

  default:
    break;
  }
  return CommonWnd::OnMessage(msg, wParam, lParam);
}

void ApplicationTray::OnTrayExit()
{
  assert(_application != nullptr);
  _application->Close();
}

void ApplicationTray::OnTrayVersion()
{
  assert(_application != nullptr);
  _application->Version();
}

void ApplicationTray::OnTrayReload()
{
  assert(_application != nullptr);
  _application->Restart();
}

void ApplicationTray::SetActive()
{
  assert(_tray != nullptr);
  _tray->SetActive();
}

void ApplicationTray::SetInactive()
{
  assert(_tray != nullptr);
  _tray->SetInactive();
}