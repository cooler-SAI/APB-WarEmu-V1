using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using System.Xml.Serialization;

using FrameWork.NetWork;
using FrameWork.zlib;

using PakExtractor;

[Serializable]
public class FileHeader
{
    //public string Unk1 = "";
    public int ZSize = 0;
    public int Size = 0;
    public int Unk2 = 0;
    public int Start = 0;
    //public string Unk3 = "";
    public string Ext = "";
}

[Serializable]
public class PakHeaders
{
    public string FileName="";
    public int One = 0;
    public int FileSize = 0;
    public int Padding = 0;
    public int Hsize = 0;
    public int HeaderSize = 0;

    public List<FileHeader> Files = new List<FileHeader>();
}

static public class ExtractorMgr
{
    static public string ExtractingFolder = "";
    static public Thread ExtractThread;

    static public Dictionary<string, PakFile> _Paks = new Dictionary<string, PakFile>();
    static public Dictionary<string, PakHeaders> _Headers = new Dictionary<string, PakHeaders>();
    static public void LoadXmls()
    {
        Extractor.Data.CellClick += new DataGridViewCellEventHandler(CellClick);
        Thread ExtractThread = new Thread(new ThreadStart(Extracting));
        ExtractThread.Start();

        try
        {
            XmlSerializer X = new XmlSerializer(typeof(PakHeaders));

            string[] Urls = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.conf", SearchOption.AllDirectories);
            foreach (string Url in Urls)
            {
                PakHeaders Hrs = X.Deserialize(new FileStream(Url, FileMode.Open)) as PakHeaders;

                FileInfo Info = new FileInfo(Url);
                _Headers.Add(Hrs.FileName, Hrs);
            }

            Extractor.Instance.Tool("Loaded " + _Headers.Count + " Headers");
        }
        catch (Exception e)
        {
            Extractor.Instance.Tool(e.ToString());
        }
    }
    static public PakHeaders GetHeader(string FileName)
    {
        if (_Headers.ContainsKey(FileName))
            return _Headers[FileName];
        else
            return null;
    }
    static public void SaveXml(PakHeaders Headers)
    {
        Extractor.Instance.Tool("Saving Header : " + Headers.FileName);

        _Headers.Add(Headers.FileName, Headers);

        Directory.CreateDirectory(Directory.GetCurrentDirectory() + "/Headers/");

        FileStream Stream = new FileStream(Directory.GetCurrentDirectory()+"/Headers/"+Headers.FileName+".conf",FileMode.Create);
        XmlSerializer X = new XmlSerializer(typeof(PakHeaders));
        X.Serialize(Stream, Headers);
    }


    static public PakFile DecryptPak(FileInfo Info, bool Print)
    {
        if (!_Paks.ContainsKey(Info.FullName))
        {
            PakFile Pak = new PakFile(Info);
            _Paks.Add(Info.FullName, Pak);
            Pak.DeList();

            Thread t = new Thread(new ThreadStart(Pak.DeList));
            t.Start();

            if(!Print)
                t.Join();
        }

        if(Print)
            PrintPak(Info);

        return _Paks[Info.FullName];
    }
    static public void PrintPak(FileInfo Info)
    {
        _Paks[Info.FullName].PrintHeaders();
    }

    static public void ExtractPack()
    {
        string FullName = (Extractor.Box.SelectedItem as FileInfo).FullName;
        PakFile Pak = _Paks[FullName];

        foreach (ElementPak Element in Pak._Elements)
            AddToExtract(Element);
    }
    static public void CellClick(object sender, EventArgs a)
    {
        DataGridViewCell Cell = Extractor.Data.CurrentCell;
        if (Cell.OwningColumn.Name == "Extract")
        {
            string FullName = (Extractor.Box.SelectedItem as FileInfo).FullName;

            PakFile Pak = _Paks[FullName];
            ElementPak Element = Pak._Elements.Find(info => info.Id == int.Parse(Cell.OwningRow.Cells[0].Value.ToString()));

            if (Element != null)
                AddToExtract(Element);
        }
    }

    #region ExtractThread

    static public List<ElementPak> _ToExtract = new List<ElementPak>();

    static public void AddToExtract(ElementPak Element)
    {
        if (ExtractingFolder.Length <= 0)
        {
            FolderBrowserDialog Dial = new FolderBrowserDialog();
            Dial.SelectedPath = Directory.GetCurrentDirectory();
            Dial.ShowDialog();
            ExtractingFolder = Dial.SelectedPath;
        }

        lock (_ToExtract)
            _ToExtract.Add(Element);
    }

    static public bool IsRunning = true;
    static public void Extracting()
    {
        while (IsRunning)
        {
            ElementPak[] ToExtract;
            lock (_ToExtract)
            {
                ToExtract = _ToExtract.ToArray();
                _ToExtract.Clear();
            }

            if (ToExtract.Length > 0)
            {
                int Total = ToExtract.Length;
                for (int i = 0; i < Total; ++i)
                {
                    Extractor.Instance.Progress((i * 100) / Total);

                    ElementPak Element = ToExtract[i];

                    Extractor.Instance.Tool("Extracting : " + Element.Id);
                    string Folder = ExtractingFolder + "/Extracted/" + Element.Owner.FileName + "/";
                    Directory.CreateDirectory(Folder);

                    Element.GetBytes();
                    FileStream Stream = new FileStream(Folder + "/" + Element.Id + Element.Header.Ext, FileMode.Create);
                    Stream.Write(Element.dat, 0, Element.dat.Length);
                    Stream.Close();
                    Element.dat = null;

                    Thread.Sleep(50);
                }
            }

            Thread.Sleep(50);
        }
    }

    #endregion
}

public class PakFile
{
    public int Ids = 0;
    public int GetId()
    {
        return System.Threading.Interlocked.Increment(ref Ids);
    }

    public string FileName = "";
    public PakHeaders Headers;

    public ExtendedFileStream Stream;
    public PacketIn Str;
    public List<ElementPak> _Elements = new List<ElementPak>();

    public PakFile(FileInfo Info)
    {
        FileName = Info.Name;
        Headers = ExtractorMgr.GetHeader(FileName);

        Extractor.Instance.Tool(Headers != null ? "Loading from cache" : "First loading");

        try
        {
            Stream = new ExtendedFileStream(Info.FullName);
        }
        catch (Exception e)
        {
            Extractor.Instance.Tool(e.ToString());
        }
    }

    public void DeList()
    {
        if (_Elements.Count > 0)
            return;

        Extractor.Instance.Progress(0);

        if (Headers == null)
        {
            Headers = new PakHeaders();
            Headers.FileName = FileName;
            Headers.One = Stream.GetInt();
            Headers.FileSize = Stream.GetInt();
            Headers.Padding = Stream.GetInt();

            Headers.Hsize = Stream.GetInt();

            Headers.HeaderSize = Headers.Hsize / 60;

            for (int i = 0; i < Headers.HeaderSize; i++)
            {
                FileHeader Header = new FileHeader();

                string Unk1 = Stream.GetString(20);

                Header.ZSize = Stream.GetInt();
                Header.Size = Stream.GetInt();
                Header.Unk2 = Stream.GetInt();
                Header.Start = Stream.GetInt();

                string Unk3 = Stream.GetString(24);

                Headers.Files.Add(Header);
            }

            ExtractorMgr.SaveXml(Headers);
        }

        int Total = Headers.Files.Count;
        for(int i=0;i<Total;++i)
        {
            Extractor.Instance.Progress((i * 100) / Total);

            FileHeader Header = Headers.Files[i];
            ElementPak Ep = new ElementPak(this, Header, Stream);
            _Elements.Add(Ep);
        }

        Extractor.Instance.Progress(100);
    }

    public void PrintHeaders()
    {
        Extractor.Instance.ClearData();
        foreach (ElementPak Ep in _Elements)
        {
            Extractor.Instance.AddData("" + Ep.Id, Ep.GetExtention(), "" + Ep.Header.ZSize, "" + Ep.Header.Size, "Extract");
        }
    }
}

public class ElementPak
{
    public int Id = 0;
    public FileHeader Header;
    public PakFile Owner;

    public byte[] dat = null;

    public ExtendedFileStream Stream = null;
    public ElementPak(PakFile Owner,FileHeader Header,ExtendedFileStream Stream)
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

public class ExtendedFileStream : FileStream
{
    public ExtendedFileStream(string Filename) : base(Filename, FileMode.Open) { }

    public short GetShort()
    {
        int typesize = sizeof(short);

        byte[] dat = new byte[typesize];
        Read(dat, 0, typesize);

        return BitConverter.ToInt16(dat, 0);
    }

    public int GetInt()
    {
        int typesize = sizeof(int);

        byte[] dat = new byte[typesize];
        Read(dat, 0, typesize);

        return BitConverter.ToInt32(dat, 0);
    }

    public long GetLong()
    {
        int typesize = sizeof(long);

        byte[] dat = new byte[typesize];
        Read(dat, 0, typesize);

        return BitConverter.ToInt64(dat, 0);
    }

    public string GetString(int size)
    {
        byte[] dat = new byte[size];
        Read(dat, 0, size);

        return Encoding.UTF8.GetString(dat);
    }
}