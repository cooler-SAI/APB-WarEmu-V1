using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "CharacterInfo_stats", DatabaseName = "World")]
    [Serializable]
    public class CharacterInfo_stats : DataObject
    {
        private byte _CareerLine;
        private byte _Level;
        private byte _StatId;
        private ushort _StatValue;

        [DataElement(AllowDbNull = false)]
        public byte CareerLine
        {
            get { return _CareerLine; }
            set { _CareerLine = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte Level
        {
            get { return _Level; }
            set { _Level = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte StatId
        {
            get { return _StatId; }
            set { _StatId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public UInt16 StatValue
        {
            get { return _StatValue; }
            set { _StatValue = value; Dirty = true; }
        }

    }
}
