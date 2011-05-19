using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Tok_Infos", DatabaseName = "World")]
    [Serializable]
    public class Tok_Info : DataObject
    {
        [PrimaryKey()]
        public uint Entry;

        [DataElement(Varchar = 255)]
        public string Name;

        [DataElement()]
        public uint Xp;

        [DataElement()]
        public uint Section;

        [DataElement()]
        public uint Index;

        [DataElement()]
        public uint Flag;

        [DataElement(Varchar = 255)]
        public string EventName;
    }
}