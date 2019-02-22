//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
using System;
using System.Threading;
using System.Threading.Tasks;
using ActionMonitor.Interfaces;

namespace Dolly.NET
{
  public class Dolly : IAction
  {
    /// <summary>
    /// The action monitor instance.
    /// </summary>
    private IActionMonitor _monitor;

    /// <summary>
    /// A random item so we can pick a sentence
    /// </summary>
    private readonly Random _random = new Random();

    public Task<bool> GoAsync(CancellationToken token)
    {
      var sentences = new[]
      {
        "Hello, Dolly",
        "Well, hello, Dolly",
        "It's so nice to have you back where you belong",
        "You're lookin' swell, Dolly",
        "I can tell, Dolly",
        "You're still glowin', you're still crowin'",
        "You're still goin' strong",
        "We feel the room swayin'",
        "While the band's playin'",
        "One of your old favourite songs from way back when",
        "So, take her wrap, fellas",
        "Find her an empty lap, fellas",
        "Dolly'll never go away again",
        "Hello, Dolly",
        "Well, hello, Dolly",
        "It's so nice to have you back where you belong",
        "You're lookin' swell, Dolly",
        "I can tell, Dolly",
        "You're still glowin', you're still crowin'",
        "You're still goin' strong",
        "We feel the room swayin'",
        "While the band's playin'",
        "One of your old favourite songs from way back when",
        "Golly, gee, fellas",
        "Find her a vacant knee, fellas",
        "Dolly'll never go away",
        "Dolly'll never go away",
        "Dolly'll never go away again",
      };

      var sentence = sentences[_random.Next(sentences.Length)];

      Console.WriteLine($"About to 'Say()' ... {sentence}");
      _monitor.Say( sentence, 100, 200 );
      return Task.FromResult(true);
    }

    public Task InitializeAsync(IActionMonitor monitor, CancellationToken token)
    {
      _monitor = monitor ?? throw new ArgumentNullException( nameof(monitor));
      return Task.FromResult(0);
    }
  }
}
