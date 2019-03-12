#include "stdafx.h"
#include "executevirtualmachine.h"

/**
 * \copydoc
 */
ExecuteVirtualMachine::ExecuteVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener) :
  IVirtualMachine( actions, messagesHandler, iIpcListener ),
  _initialized( false ),
  _mutex( L"Execute Virtual Machine")
{
}

ExecuteVirtualMachine::~ExecuteVirtualMachine()
{
  if (_initialized)
  {
    GetIpcListener().Remove(*this);
    _initialized = false;
  }

  //  Remove all the apis
  RemoveApis();
}

// create the IPC server
bool ExecuteVirtualMachine::Initialize()
{
  if( _initialized == true )
  {
    return true;
  }

  try
  {
    GetIpcListener().Add(*this);
    _initialized = true;
    return true;
  }
  catch( ... )
  {
    return false;
  }
}

//  handle the send message.
bool ExecuteVirtualMachine::HandleIpcSendMessage(const unsigned int msg, const unsigned __int64 wParam, const __int64 lParam)
{
  // we do not handle those
  return false;
}

//  handle the post message.
bool ExecuteVirtualMachine::HandleIpcPostMessage(const unsigned int msg, const unsigned __int64 wParam, const __int64 lParam)
{
  // we do not handle those
  return false;
}

/**
 * \brief Handle the various messages.
 * \param ipcRequest the request we received.
 * \param ipcResponse if we send a response, this will be the container.
 * \return boolean if we handled the message or not.
 */
bool ExecuteVirtualMachine::HandleIpcMessage(const myodd::os::IpcData& ipcRequest, myodd::os::IpcData& ipcResponse)
{
  //  we must have 2 arguments
  // #1 is the uiid
  // #2 is the function name
  const auto argumentCount = ipcRequest.GetNumArguments();
  if ( argumentCount < 2)
  {
    //  this is not for us
    return false;
  }

  //  lock us in to prevent the api from going away
  myodd::threads::Lock lock(_mutex);

  // get the uuid
  const auto uuid = ipcRequest.Get<std::wstring>(0);
  const auto api = FindApi(uuid);
  if (nullptr == api)
  {
    return false;
  }

  //  get the function name
  return HandleIpcMessage( *api, ipcRequest, ipcResponse );
}

int ExecuteVirtualMachine::Execute(const ActiveAction& action, const std::wstring& pluginFile)
{
  Initialize();

  //  create uuid and andd it to our list.
  const auto uuid = myodd::strings::MakeUuid4();
  const auto api = AddApi(uuid, action );
  if( !Execute( *api, action, pluginFile ))
  {
    RemoveApi(uuid);
    return 0;
  }

  // then wait for it to be done.
  WaitForApi(uuid);

  // if we are here, it worked.
  return 1;
}

/**
 * \brief Add an API to our current list, we cannot add duplicates!
 * \param uuid the unique Id we are adding
 * \param action the matching action for this Id.
 * \return the API that manages the action
 */
ExecuteApi* ExecuteVirtualMachine::AddApi(const std::wstring& uuid, const ActiveAction& action )
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  does it exist already
  const auto it = _apis.find(uuid);
  if (it != _apis.end())
  {
    //  yes it does, so just return what we have.
    return it->second;
  }

  //  create the powershell api.
  const auto api = CreateApi(uuid, action, GetActions(), GetMessagesHandler() );

  // add it to the array
  _apis[uuid] = api;

  // return the new psApi
  return api;
}

ExecuteApi* ExecuteVirtualMachine::FindApi(const std::wstring& uuid) const
{
  //  look for it
  const auto it = _apis.find(uuid);
  if (it == _apis.end())
  {
    //  does not seem to exist.
    return nullptr;
  }

  //  return the api
  return it->second;
}

/**
 * \brief Remove a single api from our list of apis.
 * \param uuid the uuid we want to remove.
 */
void ExecuteVirtualMachine::RemoveApi(const std::wstring& uuid)
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  look for it
  auto it = _apis.find(uuid);
  if (it == _apis.end())
  {
    //  does not seem to exist.
    return;
  }

  // delete the api
  delete it->second;

  // remove it from the map
  _apis.erase(it);
}

/**
 * Remove all the created apis.
 */
void ExecuteVirtualMachine::RemoveApis()
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  delete all the pointers
  for (auto it = _apis.begin(); it != _apis.end(); ++it)
  {
    try
    {
      //  delete it.
      delete it->second;
    }
    catch (const std::exception&)
    {
      //  there was a problem...
      //  but we have to carry on to delete the others.
      myodd::log::LogError(_T("There was a problem deleting a powershellApi : %s"), it->first);
    }
  }

  //  reset the api map
  _apis.clear();
}

/**
 * \brief check if an api is still valid/exists
 * \param uuid the api we are checking
   * \return the handle if the api is valid/exists.
 */
HANDLE ExecuteVirtualMachine::IsApiStillValid(const std::wstring& uuid)
{
  //  lock us in
  myodd::threads::Lock lock(_mutex);
  const auto api = FindApi(uuid);

  // do we have that api?
  if (api == nullptr)
  {
    // no
    return nullptr;
  }

  // get the handle.
  return api->GetHandle();
}

/**
 * \brief wait for all the apis to complete.
 */
void ExecuteVirtualMachine::WaitForAllApis()
{
  const auto sleepForMilliseconds = std::chrono::milliseconds(10);
  for (;;)
  {
    //  lock us in so we can get the count.
    myodd::threads::Lock lock(_mutex);
    if( _apis.size() == 0 )
    {
      // the lock will be released
      break;
    }
    // release the lock while we wait
    lock.Release();

    // give threads a chance to run...
    std::this_thread::yield();
    std::this_thread::sleep_for(sleepForMilliseconds);

    // wait a bit
    myodd::wnd::MessagePump(nullptr);
  }
}

/**
* \brief Wait for an API to complete, (or be removed).
* \param uuid the uuid that we want to wait for.
*/
void ExecuteVirtualMachine::WaitForApi(const std::wstring& uuid)
{
  const auto millisecondsToWait = 500;
  for (;;)
  {
    const auto hProcess = IsApiStillValid(uuid);
    if( nullptr == hProcess )
    {
      // we can get out of this now
      // as the api is no longer valid
      // or has been cleared.
      break;
    }

    // wait for it to finish... a little.
    const auto singleObject = WaitForSingleObject(hProcess, millisecondsToWait );

    // if we didn't timeout then something else happened.
    if (WAIT_TIMEOUT != singleObject)
    {
      if (NO_ERROR != singleObject)
      {
        myodd::log::LogWarning(_T("There was an error running powershell.") );
      }
      CloseHandle(hProcess);
      break;
    }

    // wait a bit
    myodd::wnd::MessagePump(nullptr);
  }

  // if we are here the api either does not exist
  // or it is now complete, we can now remove this api
  RemoveApi(uuid);
}

/**
 * Destroy all the currently running scripts.
 */
void ExecuteVirtualMachine::Destroy()
{
  //  are we initialised?
  if( !_initialized )
  {
    return;
  }

  //  lock us in
  myodd::threads::Lock lock(_mutex);

  //  close all the handles.
  for (auto it = _apis.begin(); it != _apis.end(); ++it)
  {
    try
    {
      const auto hProcess = it->second->GetHandle();
      if( hProcess )
      {
        CloseHandle(hProcess);
        it->second->SetHandle(nullptr);
      }
    }
    catch (const std::exception&)
    {
      //  there was a problem...
      //  but we have to carry on to delete the others.
      myodd::log::LogError(_T("There was a problem deleting a powershellApi : %s"), it->first);
    }
  }

  // release the lock
  lock.Release();

  // wait for all the apis to complete.
  WaitForAllApis();

  //  we can now remove the apis.
  RemoveApis();
}