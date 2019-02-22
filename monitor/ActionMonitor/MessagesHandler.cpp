#include "stdafx.h"
#include "MessagesHandler.h"

MessagesHandler::MessagesHandler() :
  _threadId(std::this_thread::get_id())
{
  _messagesHandlerWnd.Create();
}

MessagesHandler::~MessagesHandler()
{
  // stop the window
  _messagesHandlerWnd.Close();
}

/**
 * \brief remove old/unused messages clogging up the vector
 *        this function will obtain the lock before clearing the list.
 */
void MessagesHandler::ClearUnused()
{
  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // we can now call the unsafe function
  UnsafeClearUnused();
}

/**
 * \brief remove old/unused messages clogging up the vector
 *        this function does not use the lock and assumes the caller has the lcoks
 */
void MessagesHandler::UnsafeClearUnused()
{
  for (auto it = _collection.begin(); it != _collection.end(); ++it )
  {
    if((*it)->IsRunning())
    {
      continue;
    }

    //  otherwise we can remove it from the list
    _collection.erase(it);

    // but as we changed the vector, we have to start all over again
    UnsafeClearUnused();
  }
}

/**
 * \brief handle a message dialog that has now been completed.
 * \param dlg the dialog that we just completed.
 */
void MessagesHandler::MessageDialogIsComplete(MessageDlg* dlg)
{
  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // look for the window we want to delete.
  const auto saved = std::find(_collection.begin(), _collection.end(), dlg);
  if (saved == _collection.end())
  {
    return;
  }

  // remove it from the list
  _collection.erase(saved);
}

/**
 * \brief Add a message to the dialog
 * \param dlg the dialog we are adding.
 */
void MessagesHandler::AddMessageDialogToCollection(MessageDlg* dlg)
{
  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // add it to our list of messages.
  _collection.push_back(dlg);
}

/**
 * \brief display a message
 * \param wsText the message we want to display
 * \param nElapse how long we want to display the message for.
 * \param nFadeOut where we want to fade the window from.
 * \return if we were able to display the message or not.
 */
bool MessagesHandler::Show(const std::wstring& wsText, const int nElapse, const int nFadeOut)
{
  // Sanity check
  if (wsText.length() == 0)
  {
    return false;
  }

  if (std::this_thread::get_id() != _threadId)
  {
    // we do not need to use the lock here
    // this is because we will use the lock in the correct thread id...
    return _messagesHandlerWnd.Show( *this, wsText, nElapse, nFadeOut);
  }

  try
  {
    //  look for old messages to remove
    ClearUnused();

    const auto messageDlg = new MessageDlg();
    messageDlg->Create(wsText, nElapse, nFadeOut);

    // start the fade message and pass a lambda
    // function so we are called back when the window is deleted
    // this is so we can remove it from our list here.
    messageDlg->Show([&](CWnd* dlg)
                     {
                       MessageDialogIsComplete(static_cast<MessageDlg*>(dlg));
                     });

    // add the message dialog to the collection.
    AddMessageDialogToCollection(messageDlg);

    // success.
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

/**
 * \brief Kill all the currently active message windows.
 */
void MessagesHandler::CloseAll()
{
  //  remove what is complete.
  ClearUnused();

  // tell everybody to close now.
  SendCloseMessageToAllMessageWindows();

  // now that we asked for windows to be closed.
  // we can wait for them to close.
  WaitForAllToComplete();
}

/**
 * \brief send a request to all running windows to close now.
 */
void MessagesHandler::SendCloseMessageToAllMessageWindows()
{
  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // get a worker so we can do it all in parallel.
  myodd::threads::Workers workers;

  // kill the other messages;
  for (auto it = _collection.begin();
      it != _collection.end();
      ++it)
  {
    //  clear what is still good.
    workers.QueueWorker( &MessageDlg::Close, *it);
  }

  // then wait for all of them to finish.
  workers.WaitForAllWorkers();

  // look for old messages to remove
  // but as we still have the lock
  // we can do it the 'unsafe' way
  UnsafeClearUnused();
}

/**
 * \brief Wait for all the active windows to complete.
 */
void MessagesHandler::WaitForAllToComplete()
{
  // Wait for pending messages
  // we try and get the parent window
  // and if we cannot locate it, then it must be because the window no longer exists
  // and as such we must wait for it.
  while (true)
  {
    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    auto it = _collection.begin();
    if (it == _collection.end())
      break;

    const auto dlg = static_cast<MessageDlg*>(*it);
    if (dlg != nullptr)
    {
      if (dlg->IsRunning())
      {
        // then try and process the remaining messages
        // if the window has not been killed properly.
        const auto hWnd = dlg->GetSafeHwnd();
        if (0 != ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
        {
          //  give the other apps/classes one last chance to do some work.
          MessagePump(nullptr);

          // let go of the thread.
          std::this_thread::yield();

          Sleep(1);

          // just do one message at a time
          // so we don't block others.
          // the peek message should allow all message to be handled
          MessagePump(hWnd);

          // go around one last time
          // to give everyone a chance to close.
          continue;
        }
      }
    }

    //  otherwise we can remove it
    _collection.erase(it);
  }
}

/**
 * \brief pump all the messages for a given window.
 * \param hWnd the handle of the window messages we are pumping.
 */
void MessagesHandler::MessagePump(const HWND hWnd)
{
  //  lock up to make sure we only do one at a time
  MSG msg;
  while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (0 == ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
    {
      break;
    }
  }
}
