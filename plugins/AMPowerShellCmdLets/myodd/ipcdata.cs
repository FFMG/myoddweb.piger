﻿using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace AMPowerShellCmdLets.myodd
{
  public class IpcData
  {
    private enum DataTye : short
    {
      None = 0,
      Guid = 1,
      Int32 = 2,
      String = 3,
      StringAscii = 4
    };

    /// <summary>
    /// Check if we have a valid guid or not.
    /// </summary>
    /// <returns></returns>
    public bool HasGuid()
    {
      return !string.IsNullOrEmpty(Guid);
    }

    /// <summary>
    /// Get the number of arguments we have in the IpcData
    /// </summary>
    public uint ArgumentsCount { get; private set; }

    /// <summary>
    /// Get the current Guid for this IpcData
    /// </summary>
    public string Guid { get; }

    // The current version number
    // this will help us to read the data.
    private const int VersionNumber = 100;

    private byte[] Bytes { get; set; }

    private IntPtr? HGlobal { get; set; }

    public IpcData()
    {
      // make sure it is null by default.
      HGlobal = null;

      // we have no arguments.
      ArgumentsCount = 0;

      // create a brand new byte and just add the version number.
      Bytes = BitConverter.GetBytes(VersionNumber);

      // set the guid
      Guid = System.Guid.NewGuid().ToString();

      // add this guid.
      AddGuid();
    }

    /// <summary>
    /// Constructor with given bytes.
    /// </summary>
    /// <param name="bytes">The bytes data</param>
    public IpcData(byte[] bytes)
    {
      // make sure it is null by default.
      HGlobal = null;

      // we have no arguments.
      ArgumentsCount = 0;

      //  we have no guid.
      Guid = "";

      // create a brand new byte and just add the version number.
      Bytes = BitConverter.GetBytes(VersionNumber);

      // now read it.
      var dataSize = bytes.Length;
      if (dataSize < sizeof(int))
      {
        throw new ArgumentException("Invalid data size, we need at least the version number.");
      }

      // the pointer to keep track of where we are.
      var pointer = 0;

      //  get the version number
      var versionNumber = ReadVersionNumber(bytes, ref pointer);
      if( versionNumber > VersionNumber )
      {
        throw new ArgumentException("The given version number is greater than the supported version number.");
      }

      //  now read all the data
      for (; pointer < dataSize;)
      {
        // get the next item type.
        var dataType = ReadDataType(bytes, ref pointer);
        switch (dataType)
        {
          case DataTye.Guid:
            {
              // set the guid.
              Guid = ReadGuid(bytes, ref pointer);
            }
            break;

          case DataTye.Int32:
            {
              var dataValue = ReadInt32(bytes, ref pointer);
            }
            break;

          case DataTye.String://string unicode
            {
              var sDataValue = ReadString(bytes, ref pointer);
            }
            break;

          case DataTye.StringAscii:
            {
              var sDataValue = ReadAsciiString(bytes, ref pointer);
            }
            break;

          case DataTye.None:
          default:
            // no point goint further as we do no know the size of that argument.
            // even if we can move the pointer forward, it would be more luck than anything.
            throw new ArgumentException("Unknown argument type, I am unable to read the size/data for it.");
        }

        switch (dataType)
        {
          case DataTye.Guid:
            //  the guid does not count as an argument.
            break;

          default:
            //  we found a valid argument.
            ++ArgumentsCount;
            break;
        }
      }
    }

    ~IpcData()
    {
      // do we need to free some memory?
      ResetPtr();
    }

    /// <summary>
    /// Get current data type.
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static DataTye ReadDataType(byte[] bytes, ref int pointer)
    {
      //  check that we have enough data.
      if (pointer + sizeof(short) > bytes.Length)
      {
        throw new ArgumentOutOfRangeException("We there is not enough data to read the data type.");
      }

      // create the destination item
      var dst = new byte[sizeof(short)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dst, 0, sizeof(short));

      // update the pointer.
      pointer += sizeof(short);

      // return the converted value.
      return (DataTye)BitConverter.ToInt16(dst, 0);
    }

    /// <summary>
    /// Get the string an integer.
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static int ReadInt32(byte[] bytes, ref int pointer)
    {
      //  check that we have enough data.
      if( pointer + sizeof(int) > bytes.Length)
      {
        throw new ArgumentOutOfRangeException("We there is not enough data to read.");
      }

      // create the destination item
      var dst = new byte[sizeof(int)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dst, 0, sizeof(int));

      // update the pointer.
      pointer += sizeof(int);

      // return the converted value.
      return BitConverter.ToInt32(dst, 0);
    }

    /// <summary>
    /// Get the string as unicode
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static string ReadString(byte[] bytes, ref int pointer)
    {
      // create the destination item
      var dstDataSize = new byte[sizeof(int)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dstDataSize, 0, sizeof(int));
      var dataSize = BitConverter.ToInt32(dstDataSize, 0);

      // update the pointer.
      pointer += sizeof(int);

      //  now get the data itself
      // create the destination item
      var dstData = new byte[dataSize*sizeof(char)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dstData, 0, dataSize * sizeof(char));

      // update the pointer.
      pointer += dataSize * sizeof(char);

      // return the converted value.
      return Encoding.Unicode.GetString(dstData );
    }

    /// <summary>
    /// Get the string as ascii
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static string ReadAsciiString(byte[] bytes, ref int pointer)
    {
      // create the destination item
      var dstDataSize = new byte[sizeof(int)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dstDataSize, 0, sizeof(int));
      var dataSize = BitConverter.ToInt32(dstDataSize, 0);

      // update the pointer.
      pointer += sizeof(int);

      //  now get the data itself
      // create the destination item
      var dstData = new byte[dataSize];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dstData, 0, dataSize );

      // update the pointer.
      pointer += dataSize;

      // return the converted value.
      return Encoding.ASCII.GetString(dstData);
    }

    /// <summary>
    /// Get the Guid, (a string as unicode)
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static string ReadGuid(byte[] bytes, ref int pointer)
    {
      // this is a wide string
      return ReadString(bytes, ref pointer);
    }

    /// <summary>
    /// Get the current version number.
    /// </summary>
    /// <param name="bytes">the bytes we are reading from</param>
    /// <param name="pointer">the current pointer location</param>
    /// <returns></returns>
    private static int ReadVersionNumber(byte[] bytes, ref int pointer)
    {
      // create the destination item
      var dst = new byte[sizeof(int)];

      // copy from where we are to where we are going.
      Array.Copy(bytes, pointer, dst, 0, sizeof(int));

      // update the pointer.
      pointer += sizeof(int);

      // return the converted value.
      return BitConverter.ToInt32(dst, 0);
    }

    /// <summary>
    /// Add the GUID as a string.
    /// </summary>
    /// <returns></returns>
    private void AddGuid()
    {
      Add(Combine(new byte[][]{
        BitConverter.GetBytes( (short)DataTye.Guid ),     //  short
        BitConverter.GetBytes( Guid.Length),        //  Int32, size is guaranteed.
        System.Text.Encoding.Unicode.GetBytes(Guid)
        }
      ));
    }

    /// <summary>
    /// Add a string to our list.
    /// </summary>
    /// <param name="stringToAdd">the string to add</param>
    /// <param name="asUnicode">If we want to send it as unicode or not.</param>
    public void Add( string stringToAdd, bool asUnicode = true )
    {
      if (asUnicode)
      {
        var stringInBytes = Encoding.Unicode.GetBytes(stringToAdd);
        Add(Combine(new byte[][]{
        BitConverter.GetBytes( (short)DataTye.String ),   //  short
        BitConverter.GetBytes( stringToAdd.Length),       //  Int32, size is guaranteed.
        stringInBytes
        }
        ));
      }
      else
      {
        Add(Combine(new byte[][]{
        BitConverter.GetBytes( (short)DataTye.StringAscii ),   //  short
        BitConverter.GetBytes( stringToAdd.Length),         //  Int32, size is guaranteed.
        System.Text.Encoding.ASCII.GetBytes(stringToAdd)
        }
        ));
      }

      // update the argument count.
      ArgumentsCount++;
    }

    /// <summary>
    /// Add a number to the list
    /// </summary>
    /// <param name="numberToAdd">The number to add.</param>
    public void Add(int numberToAdd)
    {
      Add( Combine( new[]
      {
        BitConverter.GetBytes( (short)DataTye.Int32),  //  short
        BitConverter.GetBytes(numberToAdd)                //  int32
        }               
      ));

      // update the argument count.
      ArgumentsCount++;
    }

    /// <summary>
    /// Add a created byte to our current byte.
    /// </summary>
    /// <param name="newBytes"></param>
    private void Add( byte[] newBytes )
    {
      //  join the old and the new together.
      Bytes = Combine( Bytes, newBytes );
    }

    /// <summary>
    /// Join one or more pointer together.
    /// </summary>
    /// <param name="arrays"></param>
    /// <returns></returns>
    private static byte[] Combine(params byte[][] arrays)
    {
      var rv = new byte[arrays.Sum(a => a.Length)];
      var offset = 0;
      foreach (var array in arrays)
      {
        System.Buffer.BlockCopy(array, 0, rv, offset, array.Length);
        offset += array.Length;
      }
      return rv;
    }

    /// <summary>
    /// Clear the pointer and free memeory if need be.
    /// </summary>
    private void ResetPtr()
    {
      if (HGlobal != null)
      {
        Marshal.FreeHGlobal((IntPtr)HGlobal);
        HGlobal = null;
      }
    }

    /// <summary>
    /// Get the data size fully created.
    /// </summary>
    /// <returns>int the size of the item</returns>
    public int GetSize()
    {
      // we know that the size is, at the very least, the version number.
      // if it is null, then we have no data
      // but we have the version number.
      return Bytes?.Length ?? sizeof(int);
    }

    /// <summary>
    /// Get the pointer for transmission.
    /// </summary>
    /// <returns></returns>
    public IntPtr GetPtr()
    {
      if( null != HGlobal )
      {
        return (IntPtr)HGlobal;
      }

      // make sure we clean what needst to be
      ResetPtr();

      // build the pointer.
      // var msgBytes = System.Text.Encoding.Default.GetBytes(msg);
      var bytesSize = Bytes.Length;
      HGlobal = Marshal.AllocHGlobal(bytesSize);
      Marshal.Copy(Bytes, 0, (IntPtr)HGlobal, bytesSize);

      return (IntPtr)HGlobal;
    }
  }
}
