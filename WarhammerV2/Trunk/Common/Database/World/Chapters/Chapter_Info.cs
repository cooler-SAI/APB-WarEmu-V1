using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Database;

namespace Common
{
    [DataTable(PreCache = false, TableName = "Chapter_Infos", DatabaseName = "World")]
    [Serializable]
    public class Chapter_Info : DataObject
    {
        [PrimaryKey()]
        public uint Entry;

        [DataElement()]
        public ushort ZoneId;

        [DataElement(Varchar=255)]
        public string Name;

        [DataElement()]
        public uint CreatureEntry;

        [DataElement()]
        public uint InfluenceEntry;

        [DataElement(Varchar = 30)]
        public string Race;

        [DataElement()]
        public uint ChapterRank;

        [DataElement()]
        public ushort PinX;

        [DataElement()]
        public ushort PinY;

        [DataElement()]
        public uint TokEntry;

        [DataElement()]
        public uint TokExploreEntry;

        public ushort OffX;
        public ushort OffY;
    }
}