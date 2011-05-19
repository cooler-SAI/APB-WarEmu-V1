using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Xp_Infos", DatabaseName = "World")]
    [Serializable]
    public class Xp_Info : DataObject
    {
        public byte _Level;
        public uint _Xp;
        public int _Adv1;
        public int _Adv2;

        [DataElement(Unique = true)]
        public byte Level
        {
            get { return _Level; }
            set { _Level = value; }
        }

        [DataElement()]
        public uint Xp
        {
            get { return _Xp; }
            set { _Xp = value; }
        }

        [DataElement()]
        public int Adv1
        {
            get { return _Adv1; }
            set { _Adv1 = value; }
        }

        [DataElement()]
        public int Adv2
        {
            get { return _Adv2; }
            set { _Adv2 = value; }
        }
    }
}