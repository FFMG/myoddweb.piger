#include "stdafx.h"
#include "Messages.h"


Messages::Messages()
{
}

Messages::~Messages()
{
}

/**
 * \brief remove all the completed on screen messages.
 */
void Messages::ClearUnused()
{
  while (_collection.size() > 0)
  {
    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    const auto it = _collection.begin();
    if (it == _collection.end())
    {
      break;
    }

    const auto dlg = static_cast<MessageDlg*>(*it);
    if (dlg != nullptr)
    {
      if (dlg->IsRunning())
      {
        break;
      }
    }
    //  otherwise we can remove it from the list
    _collection.erase(it);
  }
}

/**
 * \brief display a message
 * \param wsText the message we want to display
 * \param nElapse how long we want to display the message for.
 * \param nFadeOut where we want to fade the window from.
 * \return if we were able to display the message or not.
 */
bool Messages::Show(const std::wstring& wsText, const int nElapse, const int nFadeOut)
{
  // Sanity check
  if (wsText.length() == 0)
  {
    return false;
  }

  //  look for old messages to remove
  ClearUnused();

  try
  {
    const auto messageDlg = new MessageDlg();
    messageDlg->Create(wsText, nElapse, nFadeOut);

    // start the fade message and pass a lambda
    // function so we are called back when the window is deleted
    // this is so we can remove it from our list here.
    messageDlg->FadeShowWindow([&](CWnd* dlg)
    {
      // protected the vector for a short while.
      myodd::threads::Lock guard(_mutex);

      // look for the window we want to delete.
      const auto saved = std::find(_collection.begin(), _collection.end(), dlg);
      if (saved != _collection.end())
      {
        // remove it from the list
        _collection.erase(saved);
      }
    });

    // protected the vector for a short while.
    myodd::threads::Lock guard(_mutex);

    // add it to our list of messages.
    _collection.push_back(messageDlg);

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
void Messages::KillAll()
{
  //  remove what is complete.
  ClearUnused();

  // protected the vector for a short while.
  myodd::threads::Lock guard(_mutex);

  // kill the other messages;
  for (auto it = _collection.begin();
    it != _collection.end();
    ++it)
  {
    //  clear what is still good.
    const auto dlg = static_cast<MessageDlg*>(*it);
    if (dlg != nullptr)
    {
      dlg->FadeKillWindow();
    }
  }

  // now that we asked for windows to be closed.
  // we can wait for them to close.
  WaitForAllToComplete();
}

/**
 * \brief Wait for all the active windows to complete.
 */
void Messages::WaitForAllToComplete()
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

    //  otherwise we can remove it
    _collection.erase(it);
  }
}

/**
 * \brief pump all the messages for a given window.
 * \param hWnd the handle of the window messages we are pumping.
 */
void Messages::MessagePump(const HWND hWnd)
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
