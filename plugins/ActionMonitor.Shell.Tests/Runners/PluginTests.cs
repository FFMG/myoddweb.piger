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
