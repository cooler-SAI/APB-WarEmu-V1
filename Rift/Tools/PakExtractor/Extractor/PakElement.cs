using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using PakExtractor;
using FrameWork.zlib;

public class PakElement
{
    public int Id = 0;
    public FileHeader Header;
    public PakFile Owner;

    public byte[] dat = null;

    public ExtendedFileStream Stream = null;
    public PakElement(PakFile Owner, FileHeader Header, ExtendedFileStream Stream)
    {
        this.Owner = Owner;
        this.Id = Owner.GetId();
        this.Stream = Stream;
        this.Header = Header;

        this.Header.Ext = GetExtention();
    }

    public string GetExtention()
    {
        if (Header.Ext.Length <= 0)
        {
            byte[] Data = null;

            if (IsCompress())
            {
                GetBytes();
                Data = dat;
            }
            else
                Data = new byte[Header.ZSize >= 20 ? 20 : Header.ZSize];

            long BackPos = Stream.Position;
            Stream.Position = Header.Start;
            Stream.Read(Data, 0, Data.Length);
            Stream.Position = BackPos;

            Header.Ext = Encoding.UTF8.GetString(Data, 0, Data.Length);

            if (Header.Ext.IndexOf("WAVE") >= 0)
                Header.Ext = ".wav";
            else if (Header.Ext.IndexOf("DDS") >= 0)
                Header.Ext = ".dds";
            else if (Header.Ext.IndexOf("BK") >= 0 || Header.Ext.IndexOf("BIK") >= 0)
                Header.Ext = ".bik";
            else
                Header.Ext = ".unk";

            dat = null;
        }

        return Header.Ext;
    }

    public bool IsCompress()
    {
        return Header.Size != Header.ZSize;
    }

    public void GetBytes()
    {
        if (dat != null)
            return;

        dat = new byte[Header.ZSize];

        long BackPos = Stream.Position;
        Stream.Position = Header.Start;
        Stream.Read(dat, 0, Header.ZSize);
        Stream.Position = BackPos;

        if (IsCompress())
            dat = ZlibMgr.Decompress(dat);
    }
}