using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Quests_Creature_Finisher", DatabaseName = "World")]
    [Serializable]
    public class Quest_Creature_Finisher : DataObject
    {
        [DataElement()]
        public UInt16 Entry;

        [DataElement()]
        public uint CreatureID;
    }
}
