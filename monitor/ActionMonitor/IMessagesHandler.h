#pragma once
class IMessagesHandler
{
public:
  IMessagesHandler();
  virtual ~IMessagesHandler();

  /**
   * \brief display a message
   * \param sText the text we want to display
   * \param elapseMiliSecondsBeforeFadeOut how long we want to display the message for before we fade out.
   * \param totalMilisecondsToShowMessage how long we want the message to be displayed before fading out.
   * \return if we were able to display the message or not.
   */
  virtual bool Show(const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) = 0;

  /**
   * \brief Kill all the currently active message windows.
   */
  virtual void CloseAll() = 0;

  /**
   * \brief Wait for all the active windows to complete.
   */
  virtual void WaitForAllToComplete() = 0;
};

