using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Random_names", DatabaseName = "World")]
    [Serializable]
    public class Random_name : DataObject
    {
        private string _Name;

        [DataElement(Unique = true, Varchar = 255)]
        public string Name
        {
            get { return _Name; }
            set { _Name = value; }
        }
    }
}
