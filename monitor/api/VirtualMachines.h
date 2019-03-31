#pragma once
#include "IVirtualMachines.h"
#include "IApplication.h"

class VirtualMachines final : public IVirtualMachines
{
public:
  explicit VirtualMachines(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& ipcListener);
  ~VirtualMachines();

  // prevent copies.
  void operator=(const VirtualMachines&) = delete;
  VirtualMachines(const VirtualMachines&) = delete;

  void Destroy() override;
  IVirtualMachine& Get(IVirtualMachines::Type type) override;

private:
  /**
   * \brief the messages handler
   */
  IMessagesHandler& _messagesHandler;

  /**
   * \brief the applications handler.
   */
  IApplication& _application;

  /**
   * \brief the ipc listener
   */
  IIpcListener& _ipcListener;

  /**
   * \brief all the virtual machines created
   */
  std::map<IVirtualMachines::Type, IVirtualMachine*> _machines;

  /**
   * \brief the key for our thread locking
   */
  myodd::threads::Key _key;
};

