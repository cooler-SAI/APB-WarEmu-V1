using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    public class Character_Objectives
    {
        public Quest_Objectives Objective;
        public int ObjectiveID;
        public int Count;

        public bool IsDone()
        {
            if (Objective == null)
                return false;
            else
                return Count >= Objective.ObjCount;
        }
    }

    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Character_quests", DatabaseName = "Characters")]
    [Serializable]
    public class Character_quest : DataObject
    {
        [DataElement(AllowDbNull = false)]
        public int CharacterID;

        [DataElement(AllowDbNull = false)]
        public UInt16 QuestID;

        [DataElement(AllowDbNull=false)]
        public string Objectives
        {
            get
            {
                string Value = "";
                foreach (Character_Objectives Obj in _Objectives)
                    Value += Obj.ObjectiveID + ":" + Obj.Count + "|";
                return Value;
            }
            set
            {
                if (value.Length <= 0)
                    return;

                string[] Objs = value.Split('|');

                foreach (string Obj in Objs)
                {
                    if (Obj.Length <= 0)
                        continue;

                    int ObjectiveID = int.Parse(Obj.Split(':')[0]);
                    int Count = int.Parse(Obj.Split(':')[1]);

                    Character_Objectives CObj = new Character_Objectives();
                    CObj.ObjectiveID = ObjectiveID;
                    CObj.Count = Count;
                    _Objectives.Add(CObj);
                }
            }
        }

        [DataElement(AllowDbNull = false)]
        public bool Done;

        public bool IsDone()
        {
            return _Objectives.TrueForAll(obj => obj.IsDone());
        }

        public List<Character_Objectives> _Objectives = new List<Character_Objectives>();
    }
}
