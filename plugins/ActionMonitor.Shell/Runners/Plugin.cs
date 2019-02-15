using System;
using ActionMonitor.Interfaces;

namespace ActionMonitor.Shell.Runners
{
  internal class Plugin : IRunner
  {
    /// <inheritdoc />
    public IActionMonitor ActionMonitor { get; }

    public Plugin(IActionMonitor actionMonitor)
    {
      // set the Unique Id.
      ActionMonitor = actionMonitor ?? throw new ArgumentNullException( nameof(actionMonitor) );
    }
  }
}
