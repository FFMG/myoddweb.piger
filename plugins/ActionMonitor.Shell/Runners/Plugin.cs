using System;

namespace ActionMonitor.Shell.Runners
{
  internal class Plugin : IRunner
  {
    /// <inheritdoc />
    public string Uuid { get; }

    public Plugin(string uuid)
    {
      // set the Unique Id.
      Uuid = uuid?.Trim() ?? throw new ArgumentNullException( nameof(uuid) );
      if (string.IsNullOrEmpty(Uuid))
      {
        throw new ArgumentException();
      }
    }
  }
}
