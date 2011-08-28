using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "GameObject_spawns", DatabaseName = "World")]
    [Serializable]
    public class GameObject_spawn : DataObject
    {
        public GameObject_proto Proto;

        [PrimaryKey(AutoIncrement=true)]
        public uint Guid;

        [DataElement()]
        public uint Entry;

        [DataElement()]
        public ushort ZoneId;

        [DataElement()]
        public int WorldX;

        [DataElement()]
        public int WorldY;

        [DataElement()]
        public int WorldZ;

        [DataElement()]
        public int WorldO;

        [DataElement()]
        public uint DisplayID;
    }
}
