using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Character_Toks", DatabaseName = "Characters")]
    [Serializable]
    public class Character_tok : DataObject
    {
        [DataElement()]
        public int CharacterId;

        [DataElement()]
        public uint TokEntry;

        [DataElement()]
        public uint Count;
    }
}
