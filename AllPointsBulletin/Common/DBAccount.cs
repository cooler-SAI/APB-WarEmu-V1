using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

using Common;

[DataTable(PreCache = true, TableName = "Accounts", DatabaseName = "CharDB")]
[Serializable]
public class DBAccount : DataObject
{
    private int _Id;            // Id du compte
    private string _Email;      // Email du compte
    private string _Password;   // Pass du compte
    private string _RealTag;    // Tag du compte
    private int _Points;        // Points APB
    private int _PlayTime;      // Temps de jeux
    private byte _IsGm;         // Compte GM ?
    private int _WorldId;       // World sur lekel se trouve le compte

    public DBAccount()
    {

    }

    [PrimaryKey(AutoIncrement = true)]
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
    public string Email
    {
        get { return _Email; }
        set
        {
            _Email = value;
            Dirty = true;
        }
    }

    [DataElement(Varchar = 255)]
    public string Password
    {
        get { return _Password; }
        set
        {
            _Password = value;
            Dirty = true;
        }
    }

    [DataElement(Unique = true, Varchar = 255)]
    public string RealTag
    {
        get { return _RealTag; }
        set
        {
            _RealTag = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int Points
    {
        get { return _Points; }
        set
        {
            _Points = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int PlayTime
    {
        get { return _PlayTime; }
        set
        {
            _PlayTime = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte IsGm
    {
        get { return _IsGm; }
        set
        {
            _IsGm = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int WorldId
    {
        get { return _WorldId; }
        set
        {
            _WorldId = value;
            Dirty = true;
        }
    }
}