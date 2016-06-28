using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AMPowerShellCmdLets.myodd;

namespace AMPowerShellCmdLetsTest
{
  [TestClass]
  public class ipcdatatest
  {
    [TestMethod]
    public void TestHasGuidByDefault()
    {
      var emptyIpcData = new IpcData();

      // we have a guid by default.
      Assert.IsTrue(emptyIpcData.HasGuid());

      //  check that the string is not empty
      Assert.IsFalse( string.IsNullOrEmpty(emptyIpcData.Guid) );
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentException), "The argument must have at least a version number.")]
    public void TestTryingToPassEmptyByteArray()
    {
      new IpcData( new byte[0] );
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentException), "The argument must have at least a version number.")]
    public void TestTryingToPassByteArrayTooSmall()
    {
      //  the size should be 4
      new IpcData(new byte[3] { 0,0,0 });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentException), "The version number is too big for our current number.")]
    public void TestVersionNumberIsPastOurCurrentNumber()
    {
      //  the current version number is 100
      new IpcData(new byte[4] { 200, 0, 0, 0 });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentException), "Unknown argument type, I am unable to read the size/data for it.")]
    public void TestDataTypeIsUnknown()
    {
      //  current version number is 100

      new IpcData(new byte[6] { 100, 0, 0, 0, //  version
                                12, 0         //  unknown type
                              });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentOutOfRangeException), "Unknown argument type, I am unable to read the size/data for it.")]
    public void TestNotEnoughDataForDataType()
    {
      //  current version number is 100

      new IpcData(new byte[5] { 100, 0, 0, 0, //  version
                                2             //  too small.
                              });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentOutOfRangeException), "There is no data to read the int32.")]
    public void TestNoDataToReadInteger()
    {
      //  give a data type of int32, but no data to read
      new IpcData(new byte[6] { 100, 0, 0, 0, //  version
                                2, 0          //  data type int32
                              });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentOutOfRangeException), "There is no data to read the int32.")]
    public void TestNotEnoughDataToReadAnInteger()
    {
      //  give a data type of int32, but no data to read
      new IpcData(new byte[9] { 100, 0, 0, 0, //  version
                                2, 0,         //  data type int32
                                0, 0, 0       //  not enough data to make an int32
                              });
    }

    [TestMethod]
    [ExpectedException(typeof(ArgumentOutOfRangeException), "There is no data to read the int32.")]
    public void TestCanReadOneInt32ButNotEnoughDataToReadAnotherInteger()
    {
      //  give a data type of int32, but no data to read
      new IpcData(new byte[15] { 100, 0, 0, 0, //  version
                                 2, 0,         //  data type int32
                                 20, 0, 0, 0,  //  first interger is valid
                                 2, 0,         //  data type int32
                                 0, 0, 0       //  not enough data to make an int32
                               });
    }
  }
}
