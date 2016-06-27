using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AMPowerShellCmdLets.myodd;

namespace AMPowerShellCmdLetsTest
{
  [TestClass]
  public class ipcdatatest
  {
    [TestMethod]
    public void TestHasGuid()
    {
      var emptyIpcData = new IpcData();
      Assert.IsTrue(emptyIpcData.HasGuid());
    }
  }
}
