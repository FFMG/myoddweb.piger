#pragma once
#include "IMessagesHandler.h"
#include "IActions.h"
#include "ActiveAction.h"

class IVirtualMachine
{
public:
  explicit IVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler);
  virtual ~IVirtualMachine();

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

private:
  /**
   * \param the message handler that the virtual machine can use.
   */
  IMessagesHandler& _messagesHandler;

  /**
   * \param the collection of actions
   */
  IActions& _actions;
};