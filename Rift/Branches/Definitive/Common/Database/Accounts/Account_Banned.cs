using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

[DataTable(DatabaseName = "Accounts", TableName = "Accounts_Banned", PreCache = true)]
[Serializable]
public class Account_banned : DataObject
{
    [PrimaryKey]
    public long Id;

    [DataElement]
    public DateTime BanStart;

    [DataElement]
    public DateTime BanEnd;

    [DataElement(Varchar = 255)]
    public string Reason;

    [DataElement(Varchar = 32)]
    public string BannedBy;

}