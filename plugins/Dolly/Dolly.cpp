// Dolly.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Dolly.h"

PLUGIN_API
AM_RESPONSE am_Msg( AM_MSG msg, WPARAM wParam, LPARAM lParam  )
{
  switch( msg )
  {
  case AM_MSG_INIT:
    {
      //  seed me up...
      unsigned int seed = (unsigned)time(NULL);
      srand(seed);

      //
      // the plugin manager.
      amplugin* p = (amplugin*)(lParam);

      //
      // get the full path of this plugin
      // call 'getCommand( 0, ... ) and it will return this file.
      WCHAR szPath[MAX_PATH];
      size_t l = p->getCommand( 0, MAX_PATH, szPath );

      //
      // add an actio called 'dolly'
      p->addAction( L"Hello, Dolly!", szPath );
    }
    break;

  case AM_MSG_DEINIT:
    // clean up
    break;

  case AM_MSG_MAIN:
    {
      // just say something.
      const
      WCHAR* lyrics[] = {L"Hello, Dolly",
                        L"Well, hello, Dolly",
                        L"It's so nice to have you back where you belong",
                        L"You're lookin' swell, Dolly",
                        L"I can tell, Dolly",
                        L"You're still glowin', you're still crowin'",
                        L"You're still goin' strong",
                        L"We feel the room swayin'",
                        L"While the band's playin'",
                        L"One of your old favourite songs from way back when",
                        L"So, take her wrap, fellas",
                        L"Find her an empty lap, fellas",
                        L"Dolly'll never go away again",
                        L"Hello, Dolly",
                        L"Well, hello, Dolly",
                        L"It's so nice to have you back where you belong",
                        L"You're lookin' swell, Dolly",
                        L"I can tell, Dolly",
                        L"You're still glowin', you're still crowin'",
                        L"You're still goin' strong",
                        L"We feel the room swayin'",
                        L"While the band's playin'",
                        L"One of your old favourite songs from way back when",
                        L"Golly, gee, fellas",
                        L"Find her a vacant knee, fellas",
                        L"Dolly'll never go away",
                        L"Dolly'll never go away",
                        L"Dolly'll never go away again",
      };

      // the range of numbers.
      const double range_max = _countof(lyrics);

      // get the random number
      const double rnd = static_cast<double>(rand());

      // make sure it is in range.
      const int u = static_cast<int>( rnd / (RAND_MAX + 1) * (range_max));

      const WCHAR* lyric = lyrics[u];

      // and then just say it...
      ((amplugin*)(lParam))->say( lyric, 200, 5 );
    }
    break;

  default:
    break;
  }

  // not handled here.
  return AM_RESP_NOT_SUPPORTED;
}