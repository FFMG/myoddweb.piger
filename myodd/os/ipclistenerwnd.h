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
#include <afxwin.h>
#include "../threads/lock.h"
#include "ipcmessagehandler.h"

namespace myodd {
  namespace os {
    class IpcListenerWnd final : public CWnd
    {
    protected:
      threads::Key& _mutex;

      enum class IpcMessageType : unsigned int
      {
        None = 0,
        Send = 1,
        Post = 2,
        Copy = 3,
      };

      /**
       * \brief create the class and return false if there was an error
       * \param pszClassName the class name we will be creating our 'window' with
       * \return success or not.
       */
      bool CreateClass(const wchar_t* pszClassName);

      struct IpcMessageStruct
      {
        //  https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
        unsigned int uMsg;
        unsigned __int64 wParam;
        __int64 lParam;
      };

    public:
      explicit IpcListenerWnd(const wchar_t* pszClassName, HWND pParent, myodd::threads::Key& mutex, IpcMessageHandler& handler );

    protected:
      static bool MessageStructFromCopyData(const COPYDATASTRUCT& cds, IpcMessageStruct& ipcMessageStructure);
      LRESULT _WindowProc( const COPYDATASTRUCT& cds);
      LRESULT _WindowProcSend(const COPYDATASTRUCT& cds) const;
      LRESULT _WindowProcPost(const COPYDATASTRUCT& cds) const;
      LRESULT _WindowProcCopy(const COPYDATASTRUCT& cds);

      HWND _pParentWnd;
      IpcMessageHandler& _handler;

    public:
      static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
  }
}
