#pragma once
#include <functional>
#include <map>
#include <vector>
#include <queue>
#include <atomic>

#include "lock.h"

namespace myodd {
  namespace threads {
    class Messages
    {
    public:
      Messages();
      virtual ~Messages();

      Messages(const Messages&) = delete;
      void operator=(const Messages&) = delete;

      /**
       * \brief the function.
       */
      typedef std::function<void()> Function;

      /**
       * \brief add a function to handle a given message
       * \param function the function we want to add
       * \param message the message we want to monitor
       */
      int AddFunction(Function function, const std::wstring& message);

      /**
       * \brief post a message 
       * \param message the message we are posting.
       */
      void Post(const std::wstring& message);

      /**
       * \brief start the message pump and wait for a close message
       */
      void Wait();

      /**
       * \brief close the message pump
       */
      void Close();

    private:
      /**
       * \brief add function to a given message id
       * \param function the function we want to add
       * \param msgId the message id we are adding the function for.
       */
      int AddFunctionNoCheck(Function function, int msgId);

      /**
       * \brief process the messages in the queue.
       */
      void ProcessQueue();

      /**
       * \brief pop a message out of the queue
       * \return -1 if we have no messages or the message id.
       */
      int Pop();

      /**
       * \brief clone all the functions for a message id
       *        we make a copy so we can get out of the lock in case a function
       *        adds/removes another id/function
       * \param msgId the message id we are looking.
       * \return a copy of the functions
       */
      std::vector<Function> CloneFunctions(int msgId);

      /**
       * \brief get a message id given a string, add the message if needed.
       * \param message the message we are looking for.
       * \param addIfMissing add the message otherwise return -1
       * \return the message id, (or -1 if it does not exist and we do not wish to add it).
       */
      int GetMessageId(const std::wstring& message, bool addIfMissing );

      /**
       * \brief set to false when we start the message pump
       *        set to true when we want to close and exist the message pump.
       */
      std::atomic<bool> _close;

      /**
       * \brief the messages queue,the messages are added one after
       *        the other and are then removed First in/First out.
       */
      std::queue<int> _queue;

      /**
       * \brief the message string > message id map
       */
      std::map<std::wstring, int> _messages;

      /**
       * \brief the functions mapped to a given message id
       */
      std::map<int, std::vector<Function>> _messagesFunctions;

      /**
       * \brief the key to ensure thread safety
       */
      myodd::threads::Key _key;
    };
  }
}