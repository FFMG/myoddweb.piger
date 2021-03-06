#include "stdafx.h"
#include "MessagesHandler.h"

MessagesHandler::MessagesHandler() :
  _threadId(std::this_thread::get_id()),
  _mutex( L"Messages Handler" )
{
  _messagesHandlerWnd.Create();
}

MessagesHandler::~MessagesHandler()
{
  // stop the window
  _messagesHandlerWnd.Close();

  // remove whatever still needs to be
  // we are not waiting for anything anymore
  ClearUnused();
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
  if (std::this_thread::get_id() != _threadId)
  {
    // only the owner thread can delete things
    // so we do not want to do anything here.
    return;
  }

  for (auto it = _collection.begin(); it != _collection.end(); ++it )
  {
    if((*it)->IsRunning())
    {
      continue;
    }

    // free the memroy
    delete (*it);

    //  otherwise we can remove it from the list
    _collection.erase(it);

    // but as we changed the vector, we have to start all over again
    UnsafeClearUnused();

    // break out so we do not go around 
    // what is now a broken vector loop
    break;
  }
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
 * \param sText the text we want to display
 * \param elapseMiliSecondsBeforeFadeOut how long we want to display the message for before we fade out.
 * \param totalMilisecondsToShowMessage how long we want the message to be displayed before fading out.
 * \return if we were able to display the message or not.
 */
bool MessagesHandler::Show(const std::wstring& sText, const long elapseMiliSecondsBeforeFadeOut, const long totalMilisecondsToShowMessage)
{
  // Sanity check
  if (sText.length() == 0)
  {
    return false;
  }

  if (std::this_thread::get_id() != _threadId)
  {
    // we do not need to use the lock here
    // this is because we will use the lock in the correct thread id...
    return _messagesHandlerWnd.Show( *this, sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage);
  }

  try
  {
    //  look for old messages to remove
    ClearUnused();

    const auto messageDlg = new MessageDlg();
    messageDlg->Create(sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage);

    // start the fade message.
    messageDlg->Show( );

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
  myodd::threads::Workers workers;

  // get all the pointers and add them all to a worker.
  // then wait for them all to finish outside the lock
  // because the act of closing the dialog will try and update the collection.
  myodd::threads::Lock guard(_mutex);
  for( auto it = _collection.begin(); it != _collection.end(); ++it )
  {
    // get the pointer.
    auto* copy = *it;

    // add this item to the list of worker.
    // if the message is completed it will try and remove itself from the list
    // it will not work as we still have the lock.
    // but we will release it asap.
    workers.QueueWorker( &MessageDlg::Close, copy );
  }

  // make a copy of the pointer.
  guard.Release();

  // wait for all the workers to finisher their work. 
  // when they complete they call "OnComplete" and will cause
  // the lock to be obtained.
  // but we released the lock, so they should be able to complete their work.
  // wait for all the workers to finish.
  workers.WaitForAllWorkers([&]()
  {
    myodd::wnd::MessagePump(nullptr);
    return true;
  });

  // they should now all be complete.
  // so we can clear them.
  // we cannot call the unsafe version as we do not have the lock.
  ClearUnused();
}

/**
 * \brief Wait for all the active windows to complete.
 */
void MessagesHandler::WaitForAllToComplete()
{
  if (std::this_thread::get_id() != _threadId)
  {
    // only the owner thread can delete dialogs
    // so we will send a message ... back to us.
    _messagesHandlerWnd.WaitForAllToComplete(*this);
    return;
  }

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

    myodd::wnd::MessagePump(nullptr);

    const auto dlg = static_cast<MessageDlg*>(*it);
    if (dlg != nullptr)
    {
      if (dlg->IsRunning())
      {
        myodd::wnd::MessagePump(dlg->GetSafeHwnd() );

        // let go of the thread.
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // go around one last time
        // to give everyone a chance to close.
        continue;
      }
    }

    // remember that nullptr does not matter
    delete dlg;

    //  otherwise we can remove it
    // this should never really happen because we have onComplete()
    // that should be called when we remove it.
    _collection.erase(it);
  }
}
