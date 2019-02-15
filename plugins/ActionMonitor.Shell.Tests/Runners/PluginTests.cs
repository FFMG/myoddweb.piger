using System;
using ActionMonitor.Shell.Runners;
using NUnit.Framework;

namespace ActionMonitor.Shell.Tests.Runners
{
  [TestFixture]
  internal class PluginTests
  {
    [Test]
    public void TestUuidCannotBeNull()
    {
      Assert.Throws<ArgumentNullException>(() =>
      {
        var _ = new Plugin(null);
      });
    }

    [Test]
    public void UuidCannotBeEmpty()
    {
      Assert.Throws<ArgumentException>(() =>
      {
        var _ = new Plugin("");
      });
    }

    [Test]
    public void UuidCannotBeFullOfWhileSpaces()
    {
      Assert.Throws<ArgumentException>(() =>
      {
        var _ = new Plugin("      ");
      });
    }

    [TestCase("Hello")]
    [TestCase("Wolrd")]
    [TestCase("1234-5667-897")]
    public void UuidIsSavedAsExpected( string uuid )
    {
      var plugin = new Plugin(uuid);
      Assert.AreEqual( uuid, plugin.Uuid );
    }

    [TestCase("Hello", "    Hello    ")]
    [TestCase("Hello", "    Hello")]
    [TestCase("Hello", "Hello        ")]
    [TestCase("Hello", " Hello ")]
    public void UuidIsTrimmed(string uuid, string given )
    {
      var plugin = new Plugin(given);
      Assert.AreEqual(uuid, plugin.Uuid);
    }
  }
}
