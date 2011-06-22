using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Quests_Creature_Starter", DatabaseName = "World")]
    [Serializable]
    public class Quest_Creature_Starter : DataObject
    {
        [DataElement()]
        public UInt16 Entry;

        [DataElement()]
        public uint CreatureID;
    }
}
