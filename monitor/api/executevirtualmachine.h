#pragma once
#include "IVirtualMachine.h"
#include <map>
#include <os/ipcmessagehandler.h>
#include <threads/lock.h>
#include "ExecuteApi.h"

class ExecuteVirtualMachine : public myodd::os::IpcMessageHandler, public IVirtualMachine
{
public:
  explicit ExecuteVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~ExecuteVirtualMachine();

  bool HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) override;
  virtual bool HandleIpcMessage(ExecuteApi& api, const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse) = 0;

  //  handle the send message.
  bool HandleIpcSendMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam) override;

  //  handle the post message.
  bool HandleIpcPostMessage(unsigned int msg, unsigned __int64 wParam, __int64 lParam) override;

  int Execute(const IActiveAction& action, const std::wstring& pluginFile) override;
  virtual bool Execute(ExecuteApi& api, const IActiveAction& action, const std::wstring& pluginFile) = 0;

  virtual ExecuteApi* CreateApi(const std::wstring& uuid, const IActiveAction& action, IActions& actions, IMessagesHandler& messages) = 0;

  void Destroy() override;
  bool Initialize() override;

private:
  /**
   * \brief if we initialized the virtual machine
   */
  bool _initialized;

  /**
   * \brief the mutex we are using.
   */
  myodd::threads::Key _mutex;

  /**
   * \brief the collection of created apis.
   */
  typedef std::map<std::wstring, ExecuteApi*> Apis;
  Apis _apis;

  /**
   * \brief check if an api is still valid/exists
   * \param uuid the api we are checking
   * \return the handle if the api is valid/exists.
   */
  HANDLE IsApiStillValid(const std::wstring& uuid);

  ExecuteApi* AddApi(const std::wstring& uuid, const IActiveAction& action );
  ExecuteApi* FindApi(const std::wstring& uuid) const;
  void RemoveApi(const std::wstring& uuid );
  void RemoveApis();

  void WaitForApi(const std::wstring& uuid);
  void WaitForAllApis();
};