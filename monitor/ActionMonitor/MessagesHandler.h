#pragma once
#include <vector>
#include <threads/lock.h>
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

  bool Show(const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) override;
  void CloseAll() override;
  void WaitForAllToComplete() override;

protected:
  /**
   * \brief handle a message dialog that has now been completed.
   * \param dlg the dialog that we just completed.
   */
  void MessageDialogIsComplete( MessageDlg* dlg );

  /**
   * \brief Add a message to the dialog
   * \param dlg the dialog we are adding.
   */
  void AddMessageDialogToCollection(MessageDlg* dlg);

  /**
   * \brief send a request to all running windows to close now.
   */
  void SendCloseMessageToAllMessageWindows();

  /** 
   * \brief remove old/unused messages clogging up the vector
   *        this function will obtain the lock before clearing the list.
   */
  void ClearUnused();

  /**
   * \brief remove old/unused messages clogging up the vector
   *        this function does not use the lock and assumes the caller has the lcoks
   */
  void UnsafeClearUnused();

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
  myodd::threads::Key _mutex;

  /**
   * \brief the thread id that 'created' this message collection
   */
  const std::thread::id _threadId;
};

