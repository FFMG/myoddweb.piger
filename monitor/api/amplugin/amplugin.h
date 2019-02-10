#pragma once

#include <string>

#define PLUGIN_API __declspec(dllexport)

//
// The possible messages returned by am_Msg( ... )
enum AM_RESPONSE
{
  AM_RESP_NONE = 0,                     //
  AM_RESP_FALSE = AM_RESP_NONE,         //  not the same as error.
  AM_RESP_TRUE = 1,
  AM_RESP_SUCCESS,
  AM_RESP_THROW,                        // an exception was caught
  AM_RESP_FILE_ERR,                     // a file was not found.
  AM_RESP_MISSING_FN,                   // a required function is missing.
  AM_RESP_UNKNOWN_ERR,                  // there was an error, if possible, choose a more descriptive error.
  AM_RESP_NOT_SUPPORTED,                // the message was received, but was not handled as I don't know how.
  AM_RESP_BUFFER_SIZE,                  // the given buffer was probably too small.
};

//
// All the messages that are passed to the am_Msg( ... ) function
//
enum AM_MSG
{
  //
  //  !DO NOT CHANGE THOSE NUMBERS!
  //  !EVERYTHING WILL BREAK IF THEY NO LONGER MATCH!
  AM_MSG_NONE         = 0,  //
  AM_MSG_NAME         = 1,  //  get the plugin name
                            //  WPARAM = buffer size
                            //  LPARAM = buffer
  AM_MSG_INIT         = 2,  //  Called when the plugin is loaded.
                            //    LPARAM = AmPlugin*
  AM_MSG_DEINIT       = 3,  //  Called when the plugin is about to get destroyed.
                            //    LPARAM = 0 / WPARAM = 0
  AM_MSG_MAIN         = 4,  //  Ask the plugin to execute the action.
                            //    LPARAM = AmPlugin*
                            //    return AM_RESP_TRUE|AM_RESP_FALSE
  AM_MSG_PATH_CMD     = 5,  //  The root commands temp path (./).
                            //  LPARAM = const wchar_t* the temp path.
  AM_MSG_PATH_IN      = 6,  //  The root commands 'in' path (./__in).
                            //  LPARAM = const wchar_t* the temp path.
  AM_MSG_PATH_OUT     = 7,  //  The root commands 'out' path (./__out).
                            //  LPARAM = const wchar_t* the temp path.
  AM_MSG_PATH_TMP     = 8,  //  The root commands 'temp' path (./__tmp).
                            //  LPARAM = const wchar_t* the temp path.
  AM_MSG_PATH_PLUGIN  = 9,  //  The root commands 'plugin' path (./__tmp).
                            //  LPARAM = const wchar_t* the temp path.
};

//
// The various log levels
//
enum AM_LOG
{
  AM_LOG_SUCCES = 1,
  AM_LOG_ERROR = 2,
  AM_LOG_WARNING = 3,
  AM_LOG_MESSAGE = 4,
  AM_LOG_SYSTEM = 5,
};

#ifdef  _WIN64
# define AM_UINT unsigned __int64
# define AM_INT __int64
#else
# define AM_UINT unsigned int
# define AM_INT long
#endif //  _WIN64

// The main message function
// see the various messages to see what WPARAM/LPARAM are used for.
extern "C" PLUGIN_API AM_RESPONSE am_Msg(AM_MSG msg, AM_UINT wParam, AM_INT lParam);

// add the libs
#ifdef _DEBUG
# ifdef _WIN64
#   pragma comment(lib, "amplugin64d.lib" )
#else
#   pragma comment(lib, "amplugind.lib" )
# endif
#else
# ifdef _WIN64
#   pragma comment(lib, "amplugin64.lib" )
# else
#   pragma comment(lib, "amplugin.lib" )
# endif
#endif

//
// The AmPlugin path, we can call those functions to do extra operations.
//
class AmPlugin
{
public:
  AmPlugin();
  virtual ~AmPlugin();

public:
  // the various functions
  virtual double Version() = 0;
  virtual bool Say(const wchar_t* msg, const unsigned int nElapse, const unsigned int nFadeOut) const = 0;
  virtual size_t GetCommand(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) = 0;

  // get the full action as given by the user
  virtual size_t GetAction(unsigned int nBufferLength, wchar_t* lpBuffer) = 0;

  // get the number of parameters passed.
  virtual size_t GetCommandCount() const = 0;
  virtual bool Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged) = 0;
  virtual int GetString(unsigned int nBufferLength, wchar_t* lpBuffer) = 0;
  virtual size_t GetFile(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const = 0;
  virtual size_t GetFolder(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const = 0;
  virtual size_t GetUrl(unsigned int idx, unsigned int nBufferLength, wchar_t* lpBuffer) const = 0;
  virtual bool AddAction(const wchar_t* szText, const wchar_t* szPath) = 0;
  virtual bool RemoveAction(const wchar_t* szText, const wchar_t* szPath) = 0;
  virtual bool FindAction(unsigned int idx, const wchar_t* szText, unsigned int nBufferLength, wchar_t* lpBuffer) = 0;

  // Get the window that was in the forground, (at the time of the call).
  // the return value is a windows HWND
  virtual void* GetForegroundWindow() const = 0;

  // The error types are
  //   AM_LOG_SUCCES = Success = 1
  //   AM_LOG_ERROR = Error= 2
  //   AM_LOG_WARNING = Warning = 3
  //   AM_LOG_MESSAGE = Message = 4
  //   AM_LOG_SYSTEM = System = 5
  virtual void Log(unsigned int logType, const wchar_t* szText) = 0;
};