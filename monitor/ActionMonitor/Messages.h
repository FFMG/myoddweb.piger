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

  typedef std::vector<MessageDlg::Msg*> MessagesMsgCollection;
  MessagesMsgCollection _collectionInOtherThreads;

  /**
   * \brief the mutex to ensure that data is only updated once at a time.
   */
  std::mutex _mutex;

  /**
   * \brief if this message it true, we will stop everyting
   */
  bool _stop;

  /**
   * \brief the thread id that 'created' this message collection
   */
  const std::thread::id _threadId;

  /**
   * \brief Set the flag to stop
   */
  void Stop();

  /**
   * \brief Check if this is stopped
   * \return if we flagged this to stop or not.
   */
  bool Stopped() const;

  myodd::threads::Workers _worker;
  static void Wait(Messages* owner);
};

