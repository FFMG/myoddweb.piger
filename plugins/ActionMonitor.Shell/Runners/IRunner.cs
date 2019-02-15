using System.IO;
using ActionMonitor.Interfaces;

namespace ActionMonitor.Shell.Runners
{
  internal interface IRunner
  {
    /// <summary>
    /// The action monitor.
    /// </summary>
    IActionMonitor ActionMonitor { get; }

    /// <summary>
    /// The path being executed.
    /// </summary>
    FileSystemInfo FileSystemInfo { get; }

    /// <summary>
    /// Tell the runner to do.
    /// </summary>
    void Go();
  }
}
