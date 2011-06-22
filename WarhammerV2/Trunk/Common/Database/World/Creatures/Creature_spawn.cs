using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Creature_spawns", DatabaseName = "World")]
    [Serializable]
    public class Creature_spawn : DataObject
    {
        public Creature_proto Proto;

        private uint _Guid;
        private uint _Entry;
        private ushort _ZoneId;
        private int _WorldX;
        private int _WorldY;
        private int _WorldZ;
        private int _WorldO;
        private string _Bytes;
        private byte _Icone;
        private byte _Emote;
        private ushort _Title;

        [PrimaryKey(AutoIncrement = true)]
        public uint Guid
        {
            get { return _Guid; }
            set { _Guid = value; Dirty = true; }
        }

        [DataElement(AllowDbNull=false)]
        public uint Entry
        {
            get { return _Entry; }
            set { _Entry = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public ushort ZoneId
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
        public string Bytes
        {
            get { return _Bytes; }
            set { _Bytes = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte Icone
        {
            get { return _Icone; }
            set { _Icone = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte Emote
        {
            get { return _Emote; }
            set { _Emote = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public ushort Title
        {
            get { return _Title; }
            set { _Title = value; Dirty = true; }
        }


        public byte[] bBytes
        {
            get
            {
                List<byte> Btes = new List<byte>();
                string[] Strs = _Bytes.Split(';');
                foreach (string Str in Strs)
                    if (Str.Length > 0)
                        Btes.Add(byte.Parse(Str));

                Btes.Remove(4);
                Btes.Remove(5);
                Btes.Remove(7);

                return Btes.ToArray();
            }
        }
    }
}
