using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    // Valeur Fixe d'un character
    [DataTable(PreCache = false, TableName = "Characters_socials", DatabaseName = "Characters")]
    [Serializable]
    public class Character_social : DataObject
    {
        private int _CharacterId;

        private int _DistCharacterId;
        private string _DistName;
        private byte _Friend;
        private byte _Ignore;

        [DataElement(AllowDbNull = false)]
        public int CharacterId
        {
            get { return _CharacterId; }
            set { _CharacterId = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public int DistCharacterId
        {
            get { return _DistCharacterId; }
            set { _DistCharacterId = value; Dirty = true; }
        }

        [DataElement(Varchar=255,AllowDbNull = false)]
        public string DistName
        {
            get { return _DistName; }
            set { _DistName = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte Friend
        {
            get { return _Friend; }
            set { _Friend = value; Dirty = true; }
        }

        [DataElement(AllowDbNull = false)]
        public byte Ignore
        {
            get { return _Ignore; }
            set { _Ignore = value; Dirty = true; }
        }

        public object Event;

        public T GetEvent<T>()
        {
            return (T)Convert.ChangeType(Event, typeof(T));
        }
    }
}
