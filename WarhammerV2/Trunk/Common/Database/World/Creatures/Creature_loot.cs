using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Creature_loots", DatabaseName = "World")]
    [Serializable]
    public class Creature_loot : DataObject
    {
        [DataElement()]
        public uint Entry;

        [DataElement()]
        public uint ItemId;

        [DataElement()]
        public float Pct;

        public Item_Infos Info;
    }
}