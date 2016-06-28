#pragma once

#include <mutex>
#include <afxwin.h>

namespace myodd {
  namespace os {
    class IpcListenerWnd : public CWnd
    {
    protected:
      std::mutex& _mutex;

      enum class IpcMessageType : unsigned int
      {
        None = 0,
        Send = 1,
        Post = 2,
        Copy = 3,
      };

      struct IpcMessageStruct
      {
        //  https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
        unsigned int uMsg;
        unsigned __int64 wParam;
        __int64 lParam;
      };

    public:
      explicit IpcListenerWnd(const wchar_t* pszClassName, HWND pParent, std::mutex& mutex);

    protected:
      static bool MessageStructFromCopyData(const COPYDATASTRUCT& cds, IpcMessageStruct& ipcMessageStructure);
      LRESULT _WindowProc( const COPYDATASTRUCT& cds);
      LRESULT _WindowProcSend(const COPYDATASTRUCT& cds);
      LRESULT _WindowProcPost(const COPYDATASTRUCT& cds);
      LRESULT _WindowProcCopy(const COPYDATASTRUCT& cds);

      HWND _pParentWnd;

    public:
      static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
  }
}
