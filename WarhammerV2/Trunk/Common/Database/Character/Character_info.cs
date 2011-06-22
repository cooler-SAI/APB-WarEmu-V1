using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Changante d'un character
    [DataTable(PreCache = false, TableName = "Character_value", DatabaseName = "Characters")]
    [Serializable]
    public class Character_value : DataObject
    {
        private int _CharacterId;
        private byte _Level;
        private uint _Xp;
        private int _RestXp;
        private uint _Renown;
        private byte _RenownRank;
        private uint _Money;
        private int _Speed;
        private int _RegionId;
        private UInt16 _ZoneId;
        private int _WorldX;
        private int _WorldY;
        private int _WorldZ;
        private int _WorldO;
        private ushort _RallyPoint;
        private uint _Skills;
        private byte _BagBuy;
        private bool _Online;

        public Character_value()
            : base()
        {

        }

        [DataElement(Unique = true)]
        public int CharacterId
        {
            get { return _CharacterId; }
            set { _CharacterId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull=false)]
        public byte Level
        {
            get { return _Level; }
            set { _Level = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint Xp
        {
            get { return _Xp; }
            set { _Xp = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int RestXp
        {
            get { return _RestXp; }
            set { _RestXp = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint Renown
        {
            get { return _Renown; }
            set { _Renown = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte RenownRank
        {
            get { return _RenownRank; }
            set { _RenownRank = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint Money
        {
            get { return _Money; }
            set { _Money = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int Speed
        {
            get { return _Speed; }
            set { _Speed = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int RegionId
        {
            get { return _RegionId; }
            set { _RegionId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public UInt16 ZoneId
        {
            get { return _ZoneId; }
            set { _ZoneId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int WorldX
        {
            get { return _WorldX; }
            set { _WorldX = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int WorldY
        {
            get { return _WorldY; }
            set { _WorldY = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int WorldZ
        {
            get { return _WorldZ; }
            set { _WorldZ = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int WorldO
        {
            get { return _WorldO; }
            set { _WorldO = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public ushort RallyPoint
        {
            get { return _RallyPoint; }
            set { _RallyPoint = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte BagBuy
        {
            get { return _BagBuy; }
            set { _BagBuy = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint Skills
        {
            get { return _Skills; }
            set { _Skills = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public bool Online
        {
            get { return _Online; }
            set { _Online = value; Dirty = true; }
        }
    }
}
