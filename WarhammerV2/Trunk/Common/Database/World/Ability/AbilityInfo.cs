using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Ability_Infos", DatabaseName = "World")]
    [Serializable]
    public class Ability_Info : DataObject
    {
        [PrimaryKey]
        public UInt16 Entry;

        [DataElement()]
        public byte Level;

        [DataElement()]
        public byte CareerLine;

        [DataElement]
        public UInt16 AbilityType;

        [DataElement()]
        public int MinRange;

        [DataElement()]
        public int MaxRange;

        [DataElement]
        public int CoolDown;

        [DataElement()]
        public byte TargetType;

        [DataElement()]
        public int Damage;

        [DataElement()]
        public UInt16 ActionPoints;
    }
}
