using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Accounts", DatabaseName = "Accounts")]
    [Serializable]
    public class Account : DataObject
    {
        private int _AccountId;
        private string _Username;
        private string _Password;
        private string _Ip;
        private string _Token;
        private byte _GmLevel;

        public Account()
        {
        }

        [PrimaryKey(AutoIncrement = true)]
        public int AccountId
        {
            get { return _AccountId; }
            set { _AccountId = value; Dirty = true; }
        }

        [DataElement(Unique = true, Varchar = 255)]
        public string Username
        {
            get { return _Username; }
            set
            {
                _Username = value;
                Dirty = true;
            }
        }

        [DataElement(Varchar = 255)]
        public string Password
        {
            get { return _Password; }
            set
            {
                _Password = value;
                Dirty = true;
            }
        }

        [DataElement(Varchar = 255)]
        public string Ip
        {
            get { return _Ip; }
            set
            {
                _Ip = value;
                Dirty = true;
            }
        }

        [DataElement(Varchar = 255)]
        public string Token
        {
            get { return _Token; }
            set
            {
                _Token = value;
                Dirty = true;
            }
        }

        [DataElement(AllowDbNull=false)]
        public byte GmLevel
        {
            get { return _GmLevel; }
            set
            {
                _GmLevel = value;
                Dirty = true;
            }
        }
    }
}
