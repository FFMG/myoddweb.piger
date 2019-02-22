#pragma once
#include <vector>
#include "MessageDlg.h"
#include "IMessagesHandler.h"
#include "MessagesHandlerWnd.h"

class MessagesHandler final : public IMessagesHandler
{
public:
  MessagesHandler();
  ~MessagesHandler();

  // cannot copy
  MessagesHandler(const MessagesHandler&) = delete;
  void Operator(const MessagesHandler&) = delete;

  bool Show(const std::wstring& wsText, int nElapse, int nFadeOut) override;
  void CloseAll() override;
  void WaitForAllToComplete() override;

protected:

  static void MessagePump( HWND hWnd);

  // remove old/unused messages clogging up the vector
  void ClearUnused();

  typedef std::vector<MessageDlg*> MessagesCollection;
  MessagesCollection _collection;

  /**
   * \brief the messages window that allows us to post messages from
   *        threads that are not the main one.
   */
  MessagesHandlerWnd _messagesHandlerWnd;

  /**
   * \brief the mutex to ensure that data is only updated once at a time.
   */
  std::mutex _mutex;

  /**
   * \brief the thread id that 'created' this message collection
   */
  const std::thread::id _threadId;
};

