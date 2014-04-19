using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [DataTable(PreCache = false, TableName = "character_mails", DatabaseName = "Characters")]
    [Serializable]
    public class Character_mail : DataObject
    {
        [DataElement()]
        public UInt32 Guid;

        [DataElement()]
        public UInt32 SenderID;

        [DataElement()]
        public UInt32 ReceiverID;

        [DataElement()]
        public string SenderName;

        [DataElement()]
        public UInt32 SendDate;

        [DataElement(Varchar=255)]
        public string Title;

        [DataElement()]
        public string Message;

        [DataElement()]
        public bool CR;

        [DataElement()]
        public bool Opened;

        [DataElement()]
        public UInt32 Money;

        [DataElement()]
        public UInt32[] Items;
    }
}
