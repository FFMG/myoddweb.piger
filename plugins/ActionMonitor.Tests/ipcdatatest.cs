using System;
using ActionMonitor.Myodd;
using NUnit.Framework;

namespace ActionMonitor.Tests
{
  internal enum ExpectedIpcDataType : byte
  {
    None = 0,
    Guid = 1,
    Int32 = 2,
    Int64 = 3,
    String = 4,
    StringAscii = 5,
  };

  [TestFixture]
  public class IpcDataTest
  {
    [Test]
    public void TestHasGuidByDefault()
    {
      var emptyIpcData = new IpcData();

      // we have a guid by default.
      Assert.IsTrue(emptyIpcData.HasGuid());

      //  check that the string is not empty
      Assert.IsFalse( string.IsNullOrEmpty(emptyIpcData.Guid) );
    }

    [Test]
    public void TestGetSingleTypes()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add( "Hello" );
      emptyIpcData.Add( "World" );

      // check the values.
      Assert.AreEqual( "Hello", emptyIpcData.Get<string>(0));
      Assert.AreEqual( "World", emptyIpcData.Get<string>(1));
    }

    [Test]
    public void TestGetDifferentTypes()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("Hello");
      emptyIpcData.Add("World");
      emptyIpcData.Add( 42 );
      emptyIpcData.Add( "Something", false );

      // check the values.
      Assert.AreEqual("Hello", emptyIpcData.Get<string>(0));
      Assert.AreEqual("World", emptyIpcData.Get<string>(1));
      Assert.AreEqual(42, emptyIpcData.Get<int>(2));
      Assert.AreEqual("Something", emptyIpcData.Get<string>(3));
    }

    [Test]
    public void TestTheArgumentCountIsValid()
    {
      var emptyIpcData = new IpcData();

      Assert.AreEqual( (uint)0, emptyIpcData.ArgumentsCount);
      emptyIpcData.Add("Hello");
      emptyIpcData.Add("World");
      Assert.AreEqual( (uint)2 , emptyIpcData.ArgumentsCount );
      emptyIpcData.Add(42);
      Assert.AreEqual( (uint)3, emptyIpcData.ArgumentsCount);
    }

    [Test]
    public void TestTryingToGetAnEmptyItemOutOfRange()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        var emptyIpcData = new IpcData();
        emptyIpcData.Get<object>(0);
      });
    }

    [Test]
    public void TestTryingToGetANonEmptyItemOutOfRange()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(42);

      Assert.AreEqual(42, emptyIpcData.Get<int>(0));

      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  out of range
        emptyIpcData.Get<object>(1);
      });
    }

    [Test]
    public void TestTryingToPassEmptyByteArray()
    {
      Assert.Throws<ArgumentException>(() =>
      {
        new IpcData(new byte[] { });
      });
    }

    [Test]
    public void TestTryingToPassByteArrayTooSmall()
    {
      Assert.Throws<ArgumentException>(() =>
      {
        //  the size should be 4
        new IpcData(new byte[] { 0, 0, 0 });
      });
    }

    [Test]
    public void TestVersionNumberIsPastOurCurrentNumber()
    {
      Assert.Throws<ArgumentException>(() =>
      {
        //  the current version number is 100
        new IpcData(new byte[] { 200, 0, 0, 0 });
      });
    }

    [Test]
    public void TestDataTypeIsUnknown()
    {
      //  current version number is 100
      Assert.Throws<ArgumentException>(() =>
      {
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                12, 0         //  unknown type
                              });
      });
    }

    [Test]
    public void TestNotEnoughDataForDataType()
    {
      //  current version number is 100
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                2             //  too small.
                              });
      });
    }

    [Test]
    public void TestNoDataToReadGuid()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Guid, 0          //  data type guid
                              });
      });
    }

    [Test]
    public void TestNoDataToReadInteger()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0          //  data type int32
                              });
      });
    }

    [Test]
    public void TestNoDataToReadLong()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int64, 0          //  data type int64
                              });
      });
    }

    [Test]
    public void TestNoDataToReadString()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.String, 0          //  data type string
                              });
      });
    }

    [Test]
    public void TestNoDataToReadStringAscii()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.StringAscii, 0          //  data type string ascii
                              });
      });
    }

    [Test]
    public void TestNotEnoughDataToReadAGuid()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Guid, 0,         //  data type guid
                                0, 0, 0       //  not enough data to make an int32
                              });
      });
    }

    [Test]
    public void TestCanReadGuidSizeButNoData()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Guid, 0,        //  data type guid
                                10, 0, 0,0   //  size of 10
                                             //  but no more data
                              });
      });
    }

    [Test]
    public void TestCanReadStringSizeButNoData()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.String, 0,        //  data type string
                                10, 0, 0,0   //  size of 10
                                             //  but no more data
                              });
      });
    }

    [Test]
    public void TestCanReadAsciiStringSizeButNoData()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.StringAscii, 0,        //  data type ascii string
                                10, 0, 0,0   //  size of 10
                                             //  but no more data
                              });
      });
    }

    [Test]
    public void TestNotEnoughDataToReadAnInteger()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,         //  data type int32
                                0, 0, 0       //  not enough data to make an int32
                              });
      });
    }

    [Test]
    public void TestNotEnoughDataToReadAString()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.String, 0,         //  data type unicode string
                                0, 0, 0       //  not enough data to make an int32
                              });
      });
    }

    [Test]
    public void TestNotEnoughDataToReadAnAsciiString()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.StringAscii, 0,         //  data type ascii string
                                0, 0, 0       //  not enough data to make an int32
                              });
      });
    }
    
    [Test]
    public void TestCanReadOneInt32ButNotEnoughDataToReadAnotherInteger()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                 (byte) ExpectedIpcDataType.Int32, 0,         //  data type int32
                                 20, 0, 0, 0,  //  first interger is valid
                                 (byte) ExpectedIpcDataType.Int32, 0,         //  data type int32
                                 0, 0, 0       //  not enough data to make an int32
                               });
      });
    }

    [Test]
    public void TestCanReadOneInt64ButNotEnoughDataToReadAnotherInteger()
    {
      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0,           //  version
                                 (byte) ExpectedIpcDataType.Int64, 0,                 //  data type int64
                                 20, 0, 0, 0,0,0,0,0,  //  first long is valid
                                 (byte) ExpectedIpcDataType.Int64, 0,                 //  data type int64
                                 20, 0, 0, 0,0,0       //  not enough data to make an int64
                               });
      });
    }

    [Test]
    public void TestStringCanBeSizeZero()
    {
      Assert.DoesNotThrow(() =>
      {
        //  give a data type of int32, but no data to read
        new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.String, 0,        //  data type string
                                0, 0, 0,0    //  size of 0
                                             //  no more data needed.
                              });
      });
    }

    [Test]
    public void TestCastingToDifferentNumbers()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(42);
      emptyIpcData.Add(52);
      emptyIpcData.Add(67);

      // check the values.
      Assert.AreEqual((long)42, emptyIpcData.Get<long>(0));
      Assert.AreEqual((uint)52, emptyIpcData.Get<uint>(1));
      Assert.AreEqual((int)67, emptyIpcData.Get<int>(2));
      Assert.AreEqual((short)67, emptyIpcData.Get<short>(2));

      Assert.AreEqual((double)67, emptyIpcData.Get<double>(2));
      Assert.AreEqual((float)67, emptyIpcData.Get<float>(2));
    }

    [Test]
    public void TestTryingtogetANumberWhenTheArgumentIsAString()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("Hello");

      Assert.Throws<InvalidCastException>(() =>
      {
        // this is not a number.
        emptyIpcData.Get<long>(0);
      });
    }

    [Test]
    public void TestANumberCanBeCastToAString()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add( 12 );

      // we can cast the number to a string.
      Assert.AreEqual( "12", emptyIpcData.Get<string>(0) );
    }

    [Test]
    public void TestByteArraySetsIntOneByte()
    {
      var random = new Random();
      var r1 = (byte)random.Next(0, 255);

      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,        //  data type int
                                r1, 0, 0,0   //  int value
                              });

      Assert.AreEqual(r1 , emptyIpcData.Get<int>(0));
    }

    [Test]
    public void TestByteArraySetsIntTwoBytes()
    {
      var random = new Random();
      var r1 = (byte)random.Next(0, 255);
      var r2 = (byte)random.Next(0, 255);

      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,        //  data type int
                                r1, r2, 0,0   //  int value
                              });

      Assert.AreEqual((r1+r2*256), emptyIpcData.Get<int>(0));
    }

    [Test]
    public void TestByteArraySetsIntThreeBytes()
    {
      var random = new Random();
      var r1 = (byte)random.Next(0, 255);
      var r2 = (byte)random.Next(0, 255);
      var r3 = (byte)random.Next(0, 255);

      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,        //  data type int
                                r1, r2, r3,0   //  int value
                              });

      Assert.AreEqual((r1 + r2 * 256 + r3*256*256), emptyIpcData.Get<int>(0));
    }

    [Test]
    public void TestByteArraySetsIntFourBytes()
    {
      var random = new Random();
      var r1 = (byte)random.Next(0, 255);
      var r2 = (byte)random.Next(0, 255);
      var r3 = (byte)random.Next(0, 255);
      var r4 = (byte)random.Next(0, 255);

      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,        //  data type int
                                r1, r2, r3,r4   //  int value
                              });

      Assert.AreEqual((r1 + r2 * 256 + r3 * 256 * 256 + r4 * 256 * 256*256), emptyIpcData.Get<int>(0));
    }

    [Test]
    public void TestIsNumeric()
    {
      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.Int32, 0,        //  data type int
                                32, 0, 0,0   //  int value
                              });

      Assert.IsTrue(emptyIpcData.IsInt(0));
    }

    [Test]
    public void TestWideStringIsString()
    {
      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.String, 0,        //  data type string
                                5, 0, 0,0,   //  the len
                                (byte)'h', 0,
                                (byte)'e', 0,
                                (byte)'l', 0,
                                (byte)'l', 0,
                                (byte)'o', 0,
                              });

      Assert.IsTrue(emptyIpcData.IsString(0));
      Assert.AreEqual("hello", emptyIpcData.Get<string>(0));
    }

    [Test]
    public void TestAsciiStringIsString()
    {
      //  give a data type of int32, but no data to read
      var emptyIpcData = new IpcData(new byte[] { 100, 0, 0, 0, //  version
                                (byte) ExpectedIpcDataType.StringAscii, 0,        //  data type string
                                5, 0, 0,0,   //  the len
                                (byte)'h',
                                (byte)'e',
                                (byte)'l',
                                (byte)'l',
                                (byte)'o',
                              });

      Assert.IsTrue(emptyIpcData.IsString(0));
      Assert.AreEqual("hello", emptyIpcData.Get<string>(0));
    }

    [Test]
    public void TestIsNumericPastGuid()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(12);

      Assert.IsTrue(emptyIpcData.IsInt(0));
      Assert.IsFalse(emptyIpcData.IsString(0));
    }

    [Test]
    public void TestIsStringPastGuid()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("Hello");

      Assert.IsFalse(emptyIpcData.IsInt(0));
      Assert.IsTrue(emptyIpcData.IsString(0));
    }

    [Test]
    public void TestIsNumericPastGuidAndStrings()
    {
      var random = new Random();
      var r1 = random.Next(0, int.MaxValue);
      var r2 = random.Next(0, int.MaxValue);

      var emptyIpcData = new IpcData();
      emptyIpcData.Add(r1);
      emptyIpcData.Add("Hello");
      emptyIpcData.Add(r2);

      Assert.IsTrue(emptyIpcData.IsInt(0));
      Assert.IsFalse(emptyIpcData.IsInt(1));
      Assert.IsTrue(emptyIpcData.IsInt(2));

      Assert.AreEqual(r1, emptyIpcData.Get<int>(0));
      Assert.AreEqual(r2, emptyIpcData.Get<int>(2));
    }

    [Test]
    public void TestIsNumericPastGuidAndInteger()
    {
      var random = new Random();
      var r1 = random.Next(0, Int32.MaxValue);

      var emptyIpcData = new IpcData();
      emptyIpcData.Add("Hello1");
      emptyIpcData.Add(r1);
      emptyIpcData.Add("Hello2");

      Assert.IsTrue(emptyIpcData.IsString(0));
      Assert.IsFalse(emptyIpcData.IsString(1));
      Assert.IsTrue(emptyIpcData.IsString(2));

      Assert.AreEqual("Hello1", emptyIpcData.Get<string>(0));
      Assert.AreEqual("Hello2", emptyIpcData.Get<string>(2));
    }

    [Test]
    public void TestIsNumericInvalidIndex()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(12);
      emptyIpcData.Add("Hello");

      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        emptyIpcData.IsInt(2);
      });
    }

    [Test]
    public void TestIsStringInvalidIndex()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(12);
      emptyIpcData.Add("Hello");

      Assert.Throws<ArgumentOutOfRangeException>(() =>
      {
        emptyIpcData.IsString(2);
      });
    }

    [Test]
    public void TestGetBooleanFromIntegerEqualOne()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add(1);
      Assert.IsTrue(emptyIpcData.Get<bool>(0)); 
    }

    [Test]
    public void TestGetBooleanFromIntegerNotEqualOne()
    {
      var random = new Random();
      var r = random.Next(2, Int32.MaxValue);
      if(random.Next(0, 1) == 1)
      {
        r *= -1;
      }

      var emptyIpcData = new IpcData();
      emptyIpcData.Add(1);
      Assert.IsTrue(emptyIpcData.Get<bool>(0));
    }

    [Test]
    public void TestGetBooleanTrueString()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("True");
      Assert.IsTrue(emptyIpcData.Get<bool>(0));
    }

    [Test]
    public void TestGetBooleanFalseString()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("False");
      Assert.IsFalse(emptyIpcData.Get<bool>(0));
    }

    [Test]
    public void TestGetBooleanNeitherTrueNorFalseString()
    {
      var emptyIpcData = new IpcData();
      emptyIpcData.Add("NotAValidString");

      Assert.Throws<InvalidCastException>(() =>
      {
        emptyIpcData.Get<bool>(0);
      });
    }

    [Test]
    public void TestFromInt64ToPtrAndBack()
    {
      var ipc = new IpcData();

      var random = new Random();
      var n = (long)(random.NextDouble() * long.MaxValue);
      var uuid = ipc.Guid;
      ipc.Add( long.MaxValue );
      ipc.Add(n);

      var payload = ipc.GetPtr();
      var dataBlockSize = ipc.GetSize();
      var byteArray = new byte[dataBlockSize];

      System.Runtime.InteropServices.Marshal.Copy(payload, byteArray, 0, dataBlockSize);
      var ipc2 = new IpcData(byteArray);

      Assert.AreEqual(uuid, ipc2.Guid);
      Assert.AreEqual(Int64.MaxValue, ipc2.Get<long>(0));
      Assert.AreEqual(n, ipc2.Get<long>(1));
    }

    [Test]
    public void TestFromInt32ToPtrAndBack()
    {
      var ipc = new IpcData();

      var uuid = ipc.Guid;
      var random = new Random();
      var n = (Int32)random.Next(0, Int32.MaxValue);

      ipc.Add(Int32.MaxValue);
      ipc.Add(n);

      var payload = ipc.GetPtr();
      var dataBlockSize = ipc.GetSize();
      var byteArray = new byte[dataBlockSize];

      System.Runtime.InteropServices.Marshal.Copy(payload, byteArray, 0, dataBlockSize);
      var ipc2 = new IpcData(byteArray);

      Assert.AreEqual(uuid, ipc2.Guid);
      Assert.AreEqual(Int32.MaxValue, ipc2.Get<Int32>(0));
      Assert.AreEqual(n, ipc2.Get<Int32>(1));
    }

    [Test]
    public void TestFromInt64And32ToPtrAndBack()
    {
      var ipc = new IpcData();

      var random = new Random();
      var m = (int)(random.NextDouble() * int.MaxValue);
      var n = (long)(random.NextDouble() * long.MaxValue);

      var uuid = ipc.Guid;
      ipc.Add(m);
      ipc.Add(n);

      var payload = ipc.GetPtr();
      var dataBlockSize = ipc.GetSize();
      var byteArray = new byte[dataBlockSize];

      System.Runtime.InteropServices.Marshal.Copy(payload, byteArray, 0, dataBlockSize);
      var ipc2 = new IpcData(byteArray);

      Assert.AreEqual(uuid, ipc2.Guid);
      Assert.AreEqual(m, ipc2.Get<int>(0));
      Assert.AreEqual(n, ipc2.Get<long>(1));
    }

    [Test]
    public void TestFromInt32CanBeConvertedToInt64()
    {
      var ipc = new IpcData();

      var random = new Random();
      var m = (int)(random.NextDouble() * int.MaxValue);
      var n = (long)(random.NextDouble() * long.MaxValue);

      ipc.Add(m);
      ipc.Add(n);

      Assert.AreEqual(m, ipc.Get<int>(0));
      Assert.AreEqual(m, ipc.Get<long>(0)); // as a long
      Assert.AreEqual(n, ipc.Get<long>(1));
    }
  }
}
