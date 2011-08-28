using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "GameObject_protos", DatabaseName = "World")]
    [Serializable]
    public class GameObject_proto : DataObject
    {
        [DataElement(Unique=true,AllowDbNull=false)]
        public uint Entry;

        [DataElement(Varchar=255)]
        public string Name;

        [DataElement()]
        public UInt16 DisplayID;

        [DataElement()]
        public UInt16 Scale;

        [DataElement()]
        public byte Level;

        [DataElement()]
        public byte Faction;

        [DataElement()]
        public uint HealthPoints;

        [DataElement()]
        public uint AttackTime;

        [DataElement()]
        public uint MinDamage;

        [DataElement()]
        public uint MaxDamage;

        [DataElement()]
        public UInt16[] Unks = new UInt16[6];

        public UInt16 GetUnk(int Id)
        {
            if (Id >= Unks.Length)
                return 0;

            return Unks[Id];
        }

        [DataElement(Varchar = 255)]
        public string ScriptName;
    }
}
