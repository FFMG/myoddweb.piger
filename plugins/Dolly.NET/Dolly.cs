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
      _monitor.Say( sentence, 300, 4 );
      return Task.FromResult(true);
    }

    public Task InitializeAsync(IActionMonitor monitor, CancellationToken token)
    {
      _monitor = monitor ?? throw new ArgumentNullException( nameof(monitor));
      return Task.FromResult(0);
    }
  }
}
