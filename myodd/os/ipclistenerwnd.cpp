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
#include "stdafx.h"
#include "ipclistenerwnd.h"
#include "ipcdata.h"
#include "../threads/lock.h"
#include <assert.h>

namespace myodd {
namespace os {
  IpcListenerWnd::IpcListenerWnd(const wchar_t* pszClassName, threads::Key& mutex, IpcMessageHandler& handler) :
    _mutex( mutex ),
    _handler( handler )
  {
    if( !CreateClass(pszClassName) )
    {
      throw "Can't register IpcListener window class.";
    }

    // one way or another the class was created.
    // so we can create our listener accordingly.
    if (!CWnd::CreateEx(0, pszClassName, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr))
    {
      throw "Can't create IpcListener window.";
    }

    CWnd::UpdateWindow();

    // save this pointer
    SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  }

  bool IpcListenerWnd::CreateClass(const wchar_t* pszClassName)
  {
    //  the instance of this module
    const auto hInstance = ::GetModuleHandle(nullptr);
    WNDCLASSEX wcx;
    memset(&wcx, 0, sizeof(WNDCLASSEX));

    if (GetClassInfoEx(hInstance, pszClassName, &wcx))
    {
      return true;
    }

    wcx.cbSize = sizeof(wcx);
    wcx.style = 0;
    wcx.lpfnWndProc = WindowProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wcx.lpszMenuName = nullptr;
    wcx.lpszClassName = pszClassName;
    wcx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcx))
    {
      return false;
    }
    return true;
  }

  bool IpcListenerWnd::MessageStructFromCopyData(const COPYDATASTRUCT& cds, IpcMessageStruct& ipcMessageStructure)
  {
    //  clear the values
    memset(&ipcMessageStructure, 0, sizeof(ipcMessageStructure));

    // is the data we were given the right size?
    if (cds.cbData != sizeof(ipcMessageStructure))
    {
      //  nope, it is not, something is wrong.
      return false;
    }

    // copy the data over.
    memcpy_s(&ipcMessageStructure, sizeof(ipcMessageStructure), cds.lpData, cds.cbData);

    // success.
    return true;
  }

  LRESULT CALLBACK IpcListenerWnd::WindowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
  {
    //  is it a copy data message?
    // if not, then it is just a message for this window.
    if (uMsg != WM_COPYDATA)
    {
      // pass this to our window...
      return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    const auto wndClass = reinterpret_cast<IpcListenerWnd *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (nullptr == wndClass)
    {
      // pass this to whatever window that is.
      return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    //  cast it to COPYDATASTRUCT
    const auto pcds = reinterpret_cast<COPYDATASTRUCT*>(lParam);

    return wndClass->_WindowProc(*pcds);
  }

  LRESULT IpcListenerWnd::_WindowProcSend(const COPYDATASTRUCT& cds) const
  {
    // pass this to the parent window
    IpcMessageStruct ims;
    if (!MessageStructFromCopyData(cds, ims))
    {
      return 0;
    }
    return _handler.HandleIpcSendMessage(ims.uMsg, static_cast<unsigned int>(ims.wParam), static_cast<long>(ims.lParam));
  }

  LRESULT IpcListenerWnd::_WindowProcPost(const COPYDATASTRUCT& cds) const
  {
    // pass this to the parent window
    IpcMessageStruct ims;
    if (!MessageStructFromCopyData(cds, ims))
    {
      return 0;
    }

    // pass this to the handler
    return _handler.HandleIpcPostMessage(ims.uMsg, ims.wParam, ims.lParam);
  }

  LRESULT IpcListenerWnd::_WindowProcCopy(const COPYDATASTRUCT& cds) const
  {
    try
    {
      //  try and decrypt the message that was sent.
      IpcData ipcRequest(static_cast<unsigned char*>(cds.lpData), cds.cbData);

      // just call the handler
      IpcData ipcResponse(ipcRequest.GetGuid());
      if( !_handler.HandleIpcMessage( ipcRequest, ipcResponse ) )
      {
        return 0;
      }

      // did we get a guid, if we do then we need to send the response back to the sender.
      if (!ipcRequest.HasGuid())
      {
        // we do not have a guid, but it is still a success.
        // the user might not expect a response.
        return 1;
      }

      // try and open the file map, it should be opened by the sender.
      // if not, then there is an error somewhere.
      const auto hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, ipcRequest.GetGuid().c_str());
      if (hMapFile == nullptr)
      {
        // we _did_ get a guid, but we could not open it.
        // so this is no longer a success.
        return 0;
      }

      //  get the buffer.
      const auto pBuf = static_cast<unsigned char*>(MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        1000
      ));

      if (pBuf != nullptr)
      {
        //  get the response pointer.
        const auto pData = ipcResponse.GetPtr();

        // the data size, we must make sure it is 4bytes
        // a signed int in c# is 4 bytes and should also be in c++
        auto datasize = static_cast<signed int>(ipcResponse.GetSize());
#ifdef DEBUG
        assert(sizeof(datasize) == 4);
#endif // DEBUG
        memcpy(pBuf, &datasize, sizeof(datasize));

        //  now add the data itself.
        memcpy(pBuf + sizeof(datasize), pData, datasize);

        // clean the buffer.
        UnmapViewOfFile(pBuf);
      }
      CloseHandle(hMapFile);
    }
    catch (...)
    {
      // received some invalid data.
      return 0;
    }

    // success
    return 1;
  }

  LRESULT IpcListenerWnd::_WindowProc(const COPYDATASTRUCT& cds) const
  {
    // lock the thread
    threads::Lock autoLock(_mutex);

    // then simply reconstruct the message
    switch (static_cast<IpcMessageType>(cds.dwData))
    {
    case IpcMessageType::Send:
      return _WindowProcSend(cds);

    case IpcMessageType::Post:
      return _WindowProcPost(cds);

    case IpcMessageType::Copy:
      return _WindowProcCopy(cds);

    default:
      //  unknown message type
      break;
    }

    // if we reach this then we have an unknwon type or the window is no longer valid..
    return 0;
  }
}
}
