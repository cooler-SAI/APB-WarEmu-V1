using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Characters_items", DatabaseName = "Characters")]
    [Serializable]
    public class Character_items : DataObject
    {
        private long _Guid;
        private int _CharacterId;
        private uint _Entry;
        private UInt16 _SlotId;
        private uint _ModelId;
        private UInt16 _Counts;

        public Character_items()
            : base()
        {

        }

        [DataElement(AllowDbNull = false)]
        public long Guid
        {
            get { return _Guid; }
            set { _Guid = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int CharacterId
        {
            get { return _CharacterId; }
            set { _CharacterId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint Entry
        {
            get { return _Entry; }
            set { _Entry = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public UInt16 SlotId
        {
            get { return _SlotId; }
            set { _SlotId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint ModelId
        {
            get { return _ModelId; }
            set { _ModelId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public UInt16 Counts
        {
            get { return _Counts; }
            set { _Counts = value; Dirty = true; }
        }
    }
}
