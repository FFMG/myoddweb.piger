#pragma once

namespace myodd {
  namespace log {
    enum class LogType : unsigned int
    {
      None = 0,
      Success,
      Error,
      Warning,
      Message,
      System,
    };

    typedef void(*LogEventCallback)(LogType uiType, const MYODD_CHAR* pszLine, void* pContext);
  }
}
