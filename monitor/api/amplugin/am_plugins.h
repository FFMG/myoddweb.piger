#pragma once

#define PLUGIN_API __declspec(dllexport)

//
// The possible messages returned by am_Msg( ... )
enum AM_RESPONSE
{
  AM_RESP_NONE          = 0,             //
  AM_RESP_FALSE         = AM_RESP_NONE,  //  not the same as error.
  AM_RESP_TRUE          = 1,
  AM_RESP_SUCCESS,
  AM_RESP_THROW,                         // an exception was caught
  AM_RESP_FILE_ERR,                      // a file was not found.
  AM_RESP_MISSING_FN,                    // a required function is missing.
  AM_RESP_UNKNOWN_ERR,                   // there was an error, if possible, choose a more descriptive error.
  AM_RESP_NOT_SUPPORTED,                 // the message was received, but was not handled as I don't know how.
  AM_RESP_BUFFER_SIZE,                   // the given buffer was probably too small.
};

//
// All the messages that are passed to the am_Msg( ... ) function
enum AM_MSG
{
  AM_MSG_NONE     = 0,        //
  AM_MSG_NAME,                //  get the plugin name
                              //  WPARAM = buffer size
                              //  LPARAM = buffer
  AM_MSG_INIT,                //
                              //  LPARAM = amplugin*
  AM_MSG_DEINIT,
  AM_MSG_MAIN,                //  execute an operation, the main thread is waiting for a response.
                              //  LPARAM = amplugin*
  AM_MSG_PATH_CMD,            //  The root commands temp path (./).
                              //  LPARAM = LPCWSTR the temp path.
  AM_MSG_PATH_IN,             //  The root commands 'in' path (./__in).
                              //  LPARAM = LPCWSTR the temp path.
  AM_MSG_PATH_OUT,            //  The root commands 'out' path (./__out).
                              //  LPARAM = LPCWSTR the temp path.
  AM_MSG_PATH_TMP,            //  The root commands 'temp' path (./__tmp).
                              //  LPARAM = LPCWSTR the temp path.
  AM_MSG_PATH_PLUGIN,         //  The root commands 'plugin' path (./__tmp).
                              //  LPARAM = LPCWSTR the temp path.
};

// The main message function
// see the various messages to see what WPARAM/LPARAM are used for.
extern "C" PLUGIN_API AM_RESPONSE am_Msg( AM_MSG msg, WPARAM wParam, LPARAM lParam );