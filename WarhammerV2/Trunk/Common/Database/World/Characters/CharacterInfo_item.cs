using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "CharacterInfo_items", DatabaseName = "World")]
    [Serializable]
    public class CharacterInfo_item : DataObject
    {
        private byte _CareerLine;
        private uint _Entry;
        private UInt16 _SlotId;
        private UInt16 _Count;
        private uint _ModelId;

        [DataElement(AllowDbNull = false)]
        public byte CareerLine
        {
            get { return _CareerLine; }
            set { _CareerLine = value; Dirty = true; }
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
        public UInt16 Count
        {
            get { return _Count; }
            set { _Count = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public uint ModelId
        {
            get { return _ModelId; }
            set { _ModelId = value; Dirty = true; }
        }
    }
}
