using System;
using System.IO;
using ActionMonitor.Interfaces;
using ActionMonitor.Shell.Runners;
using NUnit.Framework;

namespace ActionMonitor.Shell.Tests.Runners
{
  [TestFixture]
  internal class PluginTests
  {
    [Test]
    public void MonitorCannotBeNull()
    {
      var fs = new Moq.Mock<FileSystemInfo>();
      Assert.Throws<ArgumentNullException>(() =>
      {
        var _ = new Plugin(null, fs.Object);
      });
    }

    [Test]
    public void FileSystemCannotBeNull()
    {
      var am = new Moq.Mock<IActionMonitor>();
      Assert.Throws<ArgumentNullException>(() =>
      {
        var _ = new Plugin(am.Object, null );
      });
    }
  }
}
