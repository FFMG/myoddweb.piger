using ActionMonitor.Interfaces;

namespace ActionMonitor.Shell.Runners
{
  internal interface IRunner
  {
    /// <summary>
    /// The action monitor.
    /// </summary>
    IActionMonitor ActionMonitor { get; }
  }
}
