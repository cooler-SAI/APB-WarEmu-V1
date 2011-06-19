using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Serializable]
    [DataTable(DatabaseName = "World", TableName = "TextInfo", PreCache = true)]
    [ISerializableAttribute((long)Opcodes.WorldText)]
    public class TextInfo : ISerializablePacket
    {        
        [PrimaryKey]
        [Unsigned7Bit(0)]
        public long ID;

        [DataElement()]
        [ArrayBit(1)]
        public string Text;
    }
}
