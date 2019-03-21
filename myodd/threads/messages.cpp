#include "messages.h"
#include "../wnd/wnd.h"

namespace myodd {
  namespace threads {
    Messages::Messages() :
      _close( false )
    {
    }

    Messages::~Messages()
    {
    }

    void Messages::Wait()
    {
      // wait for everything to finish
      const auto wait = std::chrono::milliseconds(5);
      for (; _close == false ;)
      {
#ifdef _WIN32
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
#endif
        // process one message.
        ProcessOne();

        // wait a bit
        std::this_thread::yield();
        std::this_thread::sleep_for(wait);
      }
    }

    void Messages::ProcessOne()
    {
      const auto msgId = Pop();
      if (msgId == -1)
      {
        return;
      }

      const auto functions = CloneFunctions(msgId);
      for (auto function : functions)
      {
        function();
      }
    }

    void Messages::Close()
    {
      _close = true;
    }

    std::vector<Messages::Function> Messages::CloneFunctions(int msgId)
    {
      myodd::threads::Lock guard(_key);

      // we have an id, so we need to call the functions for it.
      const auto functions = _messagesFunctions.find(msgId);
      if( functions == _messagesFunctions.end() )
      {
        return std::vector<Function>();
      }
      return std::vector<Function>(functions->second);
    }

    int Messages::Pop()
    {
      myodd::threads::Lock guard(_key);
      if (_queue.empty())
      {
        return -1;
      }
      const auto msgId = _queue.front();
      _queue.pop();
      return msgId;
    }

    void Messages::Post(const std::wstring& message)
    {
      const auto msgId = GetMessageId(message, false );
      if( msgId == -1 )
      {
        return;
      }

      myodd::threads::Lock guard(_key);
      _queue.push(msgId);
    }

    int Messages::AddFunction(Function function, const std::wstring& message)
    {
      // get the message id.
      const auto msgId = GetMessageId(message, true );

      // then add the function
      return AddFunctionNoCheck(function, msgId);
    }

    int Messages::AddFunctionNoCheck(Function function, int msgId)
    {
      _messagesFunctions[msgId].push_back(function);

      // return the id for it.
      return msgId;
    }

    int Messages::GetMessageId(const std::wstring& message, bool addIfMissing)
    {
      // lock us in
      myodd::threads::Lock guard(_key);

      // first get the id of the messages
      const auto msgAndId = _messages.find(message);
      if (msgAndId != _messages.end())
      {
        return msgAndId->second;
      }

      if( false == addIfMissing )
      {
        return -1;
      }

      auto msgId = _messages.size();
      _messages[message] = msgId;

      // return the id 
      return msgId;
    }
  }
}
