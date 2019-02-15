using System;
using System.IO;
using ActionMonitor.Interfaces;

namespace ActionMonitor.Shell.Runners
{
  internal class Plugin : IRunner
  {
    /// <inheritdoc />
    public IActionMonitor ActionMonitor { get; }

    /// <inheritdoc />
    public FileSystemInfo FileSystemInfo { get; }

    public Plugin(IActionMonitor actionMonitor, FileSystemInfo fileSystemInfo)
    {
      // set the action monitor interface.
      ActionMonitor = actionMonitor ?? throw new ArgumentNullException( nameof(actionMonitor) );

      // set the file infor.
      FileSystemInfo = fileSystemInfo ?? throw new ArgumentNullException(nameof(fileSystemInfo));
    }

    /// <inheritdoc />
    public void Go()
    {
      // do it.
    }
  }
}
