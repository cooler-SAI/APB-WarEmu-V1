using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

[DataTable(PreCache = true, TableName = "Characters", DatabaseName = "CharDB")]
[Serializable]
public class DBCharacter : DataObject
{
    private int _Id;        // Id du personnage
    private int _AcctId;    // Acount Id du personnage

    private byte _SlotId;   // Slot Id du personnage
    private string _Name;   // Nom du personnage
    private byte _Faction;  // Faction du personnage (Enf,Crim)
    private byte _Gender;   // Genre : 0 Homme, 1 Femme
    private int _Version;   // Version du perso
    private int _ChangeList;// Change List
    private int _Seconds;   // Temps de création du personnage
    private int _Money;     // Argent
    private int _Rank;      // Rang
    private byte _Threat;    // Threat
    private string _Custom; // Info de customisation

    private byte[] _aCustom;// Valeur du custom en array de byte

    public DBCharacter()
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

    [DataElement]
    public int AcctId
    {
        get{ return _AcctId; }
        set
        {
            _AcctId = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte SlotId
    {
        get { return _SlotId; }
        set
        {
            _SlotId = value;
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
    public byte Faction
    {
        get{ return _Faction; }
        set
        {
            _Faction = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Gender
    {
        get{ return _Gender; }
        set
        {
            _Gender = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int Version
    {
        get
        {
            return _Version;
        }
        set
        {
            _Version = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int ChangeList
    {
        get
        {
            return _ChangeList;
        }
        set
        {
            _ChangeList = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int Seconds
    {
        get
        {
            return _Seconds;
        }
        set
        {
            _Seconds = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int Money
    {
        get
        {
            return _Money;
        }
        set
        {
            _Money = value;
            Dirty = true;
        }
    }

    [DataElement]
    public int Rank
    {
        get
        {
            return _Rank;
        }
        set
        {
            _Rank = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Threat
    {
        get
        {
            return _Threat;
        }
        set
        {
            _Threat = value;
            Dirty = true;
        }
    }

    [DataElement]
    public string Custom
    {
        get { return _Custom; }
        set
        {
            _Custom = value;
            Dirty = true;
        }
    }

    public byte[] GetaCustom()
    {
        if(_aCustom == null || _aCustom.Length <= 1)
        {
            string[] values = _Custom.Split('-');
            _aCustom = new byte[values.Length];

            for (int i = 0; i < _aCustom.Length; ++i)
                _aCustom[i] = values[i].Length > 1 ? Convert.ToByte(values[i], 16) : (byte)0;

        }

        return _aCustom;
    }
}

