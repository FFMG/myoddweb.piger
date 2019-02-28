//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once
#include "IMessagesHandler.h"
#include "IActions.h"
#include "ActiveAction.h"
#include "IIpcListener.h"

template <class Key, class Value>
class VirtualMachineLists
{
protected:
  VirtualMachineLists() : _key(L"VirtualMachineLists") {};

public:
  static VirtualMachineLists& Instance()
  {
    static VirtualMachineLists singleton;
    return singleton;
  }

public:
  void Dispose(std::function<void(Key, Value*)> onDispose = nullptr)
  {
    // lock it
    myodd::threads::Lock guard(_key);

    if (onDispose != nullptr)
    {
      // find this thread
      for (auto it = _apis.begin(); it != _apis.end(); ++it)
      {
        onDispose(it->first, it->second);
      }
    }
    _apis.erase(_apis.begin(), _apis.end());
  }

  bool Dispose( const Key& key, std::function<void(Key, Value*)> onDispose = nullptr )
  {
    // lock it
    myodd::threads::Lock guard(_key);

    // find this thread
    const auto it = _apis.find( key );
    if( it == _apis.end() )
    {
      return false;
    }

    if (onDispose != nullptr)
    {
      onDispose(it->first, it->second);
    }

    // remove it.
    _apis.erase(it);

    // we found it.
    return true;
  }

  bool Dispose(Value* api, std::function<void(Key, Value*)> onDispose = nullptr)
  {
    // lock it
    myodd::threads::Lock guard(_key);

    // find this thread
    for (auto it = _apis.begin();
      it != _apis.end();
      ++it)
    {
      if (it->second == api)
      {
        if (onDispose != nullptr)
        {
          onDispose(it->first, it->second);
        }

        // remove it.
        _apis.erase(it);

        // we found it.
        return true;
      }
    }

    // we did not find it.
    return false;
  }

  void AddApi(const Key& key, Value* api)
  {
    // lock it
    myodd::threads::Lock guard(_key);

    const auto it = _apis.find( key );
    if (it != _apis.end())
    {
      // we are trying to add an api
      // to more than one thread.
      throw - 1;
    }

    // add it
    _apis[key] = api;
  }

  Value& GetApi(const Key& key)
  {
    // lock it
    myodd::threads::Lock guard(_key);

    // for now, get the first value...
    // @todo, we need to get the actual thread id running.
    const auto it = _apis.find(key);
    if (it == _apis.end())
    {
      throw - 1;
    }
    return *(it->second);
  }
protected:
  typedef std::map<Key, Value*> ListOfPlugins;
  ListOfPlugins _apis;
  myodd::threads::Key _key;
};

class IVirtualMachine
{
public:
  explicit IVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~IVirtualMachine();

  IVirtualMachine(const IVirtualMachine&) = delete;
  void operator=(const IVirtualMachine&) = delete;

  /**
   * \brief tell the virtual machine to execute the given action
   * \param action the action we are calling
   * \param pluginFile the location of the action, (as it might not be the same as the action itself)
   * \return a number related to the virtual machine.
   */
  virtual int Execute(const ActiveAction& action, const std::wstring& pluginFile) = 0;

  /**
   * \brief Tell the virtual machine to stop all the running actions and destroy them
   *        The virtual machine can keep the action running and simply detach itself from the running action if needed.
   */
  virtual void Destroy() = 0;

  /**
   * \brief Give a chance to the virtual machine to initialize.
   * \return if the initialization worked ork not.
   */
  virtual bool Initialize() = 0;
protected:
  /**
   * \brief Get the message handler.
   */
  IMessagesHandler& GetMessagesHandler() const
  {
    return _messagesHandler;
  }

  IActions& GetActions() const
  {
    return _actions;
  }

  IIpcListener& GetIpcListener() const
  {
    return _ipcListener;
  }
private:
  /**
   * \param the message handler that the virtual machine can use.
   */
  IMessagesHandler& _messagesHandler;

  /**
   * \param the collection of actions
   */
  IActions& _actions;

  /**
   * \brief the ipc listener.
   */
  IIpcListener& _ipcListener;
};
