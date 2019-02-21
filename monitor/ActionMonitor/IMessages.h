#pragma once
class IMessages
{
public:
  IMessages();
  virtual ~IMessages();

  /**
   * \brief display a message
   * \param wsText the message we want to display
   * \param nElapse how long we want to display the message for.
   * \param nFadeOut where we want to fade the window from.
   * \return if we were able to display the message or not.
   */
  virtual bool Show(const std::wstring& wsText, int nElapse, int nFadeOut) = 0;

  /**
   * \brief Kill all the currently active message windows.
   */
  virtual void KillAll() = 0;

  /**
   * \brief Wait for all the active windows to complete.
   */
  virtual void WaitForAllToComplete() = 0;
};

