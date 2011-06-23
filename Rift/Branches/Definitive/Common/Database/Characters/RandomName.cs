using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

[DataTable(DatabaseName = "Characters", TableName = "RandomNames", PreCache = true)]
[Serializable]
public class RandomName : DataObject
{
    [DataElement(Unique = true, Varchar = 19)]
    public string Name;
}
