#pragma once
#include <vector>
#include "MessageDlg.h"
#include "IMessages.h"

class Messages final : public IMessages
{
public:
  Messages();
  ~Messages();

  // cannot copy
  Messages(const Messages&) = delete;
  void Operator(const Messages&) = delete;

  bool Show(const std::wstring& wsText, int nElapse, int nFadeOut) override;
  void KillAll() override;
  void WaitForAllToComplete() override;

protected:

  static void MessagePump( HWND hWnd);

  // remove old/unused messages clogging up the vector
  void ClearUnused();

  typedef std::vector<MessageDlg*> MessagesCollection;
  MessagesCollection _collection;

  std::mutex _mutex;
};

