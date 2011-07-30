using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    public enum Objective_Type
    {
        QUEST_UNKNOWN = 0,
        QUEST_SPEACK_TO = 1,
        QUEST_KILL_MOB = 2,
        QUEST_USE_GO = 3,
        QUEST_GET_ITEM = 4,
        QUEST_KILL_PLAYERS = 5,
        QUEST_PROTECT_UNIT = 6,
    };

    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Quests_Objectives", DatabaseName = "World")]
    [Serializable]
    public class Quest_Objectives : DataObject
    {
        [PrimaryKey(AutoIncrement=true)]
        public int Guid;

        [DataElement()]
        public UInt16 Entry;

        [DataElement()]
        public uint ObjType;

        [DataElement()]
        public uint ObjCount;

        [DataElement()]
        public string Description;

        [DataElement()]
        public string ObjID;

        public byte num;
        public Quest Quest;
        public Item_Info Item = null;
        public Creature_proto Creature = null;
    }
}
