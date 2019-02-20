#pragma once
#include <vector>
#include "MessageDlg.h"

class Messages final
{
public:
  ~Messages();

  static bool Show(const std::wstring& wsText, int nElapse, int nFadeOut);
  static void KillAll();
  static void WaitForAllToComplete();

  // cannot copy
  Messages(const Messages&) = delete;
  void Operator(const Messages&) = delete;

protected:
  // cannot create directlry
  Messages();

  // the one and only instance
  // will be destroyed when we get out.
  static Messages& Instance()
  {
    static Messages singleInstance;
    return singleInstance;
  }

  bool _Show(const std::wstring& wsText, int nElapse, int nFadeOut);
  void _KillAll();
  void _WaitForAllToComplete();

  static void MessagePump( HWND hWnd);

  // remove old/unused messages clogging up the vector
  void ClearUnused();

  typedef std::vector<MessageDlg*> MessagesCollection;
  MessagesCollection _collection;

  std::mutex _mutex;
};

