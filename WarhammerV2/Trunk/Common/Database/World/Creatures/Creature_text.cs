using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Creature_texts", DatabaseName = "World")]
    [Serializable]
    public class Creature_text : DataObject
    {
        [DataElement()]
        public uint Entry;

        [DataElement()]
        public string Text;
    }
}
