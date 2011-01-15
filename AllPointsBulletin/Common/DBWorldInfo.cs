using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

[DataTable(PreCache = true, TableName = "WorldsInfo", DatabaseName = "CharDB")]
[Serializable]
public class DBWorldInfo : DataObject
{
    private int _Id;
    private string _Name;
    private byte _Status;
    private byte _Population;
    private byte _Enf;
    private byte _Crim;

    public DBWorldInfo()
    {

    }

    [DataElement(Unique = true)]
    public int Id
    {
        get { return _Id; }
        set
        {
            _Id = value;
            Dirty = true;
        }
    }

    [DataElement(Unique = true, Varchar = 255)]
    public string Name
    {
        get{ return _Name; }
        set
        {
            _Name = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Status
    {
        get
        {
            return _Status;
        }
        set
        {
            _Status = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Population
    {
        get{ return _Population; }
        set
        {
            _Population = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Enf
    {
        get { return _Enf; }
        set
        {
            _Enf = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Crim
    {
        get { return _Crim; }
        set
        {
            _Crim = value;
            Dirty = true;
        }
    }
}