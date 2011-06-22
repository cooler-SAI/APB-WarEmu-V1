using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Renown_Infos", DatabaseName = "World")]
    [Serializable]
    public class Renown_Info : DataObject
    {
        public byte _Level;
        public uint _Renown;

        [DataElement(Unique = true)]
        public byte Level
        {
            get { return _Level; }
            set { _Level = value; }
        }

        [DataElement()]
        public uint Renown
        {
            get { return _Renown; }
            set { _Renown = value; }
        }
    }
}
