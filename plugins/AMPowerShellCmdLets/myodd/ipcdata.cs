﻿using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace AMPowerShellCmdLets.myodd
{
  public class IpcData
  {
    enum IpcDataTye : short
    {
      None = 0,
      Int32 = 1,
      String = 2
    };

    // The current version number
    // this will help us to read the data.
    const Int32 VersionNumber = 100;

    private byte[] Bytes { get; set; }

    private IntPtr? HGlobal { get; set; }

    public IpcData()
    {
      // make sure it is null by default.
      HGlobal = null;

      // create a brand new byte
      // and just add the version number.
      Bytes = BitConverter.GetBytes(VersionNumber);
    }

    ~IpcData()
    {
      // do we need to free some memory?
      if (HGlobal != null)
      {
        Marshal.FreeHGlobal( (IntPtr)HGlobal);
        HGlobal = null;
      }
    }

    public void Add( string stringToAdd )
    {
      Add(Combine(new byte[][]{
        BitConverter.GetBytes( (short)IpcDataTye.String),   //  short
        BitConverter.GetBytes( stringToAdd.Length),         //  Int32, size is guaranteed.
        System.Text.Encoding.Default.GetBytes(stringToAdd)
        }
      ));
    }

    public void Add(Int32 numberToAdd)
    {
      Add( Combine( new byte[][]{
        BitConverter.GetBytes( (short)IpcDataTye.Int32),  //  short
        BitConverter.GetBytes(numberToAdd)                //  int32
        }               
      ));
    }

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
    static private byte[] Combine(params byte[][] arrays)
    {
      byte[] rv = new byte[arrays.Sum(a => a.Length)];
      int offset = 0;
      foreach (byte[] array in arrays)
      {
        System.Buffer.BlockCopy(array, 0, rv, offset, array.Length);
        offset += array.Length;
      }
      return rv;
    }

    private void ResetPtr()
    {
      HGlobal = null;
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
      return Bytes == null ? sizeof(Int32) : Bytes.Length;
    }

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

      return (IntPtr)HGlobal;
    }
  }
}
