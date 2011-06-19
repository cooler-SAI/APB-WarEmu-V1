using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(false, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 1)]
    public class WorldMgr : RpcObject
    {
        static public MySQLObjectDatabase WorldDB;

        static public Dictionary<uint, CacheData> Datas;
        static public Dictionary<uint, CacheTemplate> Templates;
        static public Dictionary<long, TextInfo> TextInfos;

        public TextInfo GetText(long ID)
        {
            TextInfo Info;
            TextInfos.TryGetValue(ID, out Info);
            return Info;
        }

        public CacheData GetData(uint CacheID)
        {
            CacheData Data;
            Datas.TryGetValue(CacheID, out Data);
            return Data;
        }

        public CacheTemplate GetTemplate(uint CacheID)
        {
            CacheTemplate Template;
            Templates.TryGetValue(CacheID,out Template);
            return Template;
        }

        public CacheTemplate[] GetTemplates()
        {
            return Templates.Values.ToArray();
        }

        public CacheData[] GetDatas()
        {
            return Datas.Values.ToArray();
        }

        public void LoadCache()
        {
            Datas = new Dictionary<uint, CacheData>();
            Templates = new Dictionary<uint, CacheTemplate>();
            TextInfos = new Dictionary<long, TextInfo>();

            CacheData[] Dts = WorldDB.SelectAllObjects<CacheData>().ToArray();
            CacheTemplate[] Cte = WorldDB.SelectAllObjects<CacheTemplate>().ToArray();
            TextInfo[] Tis = WorldDB.SelectAllObjects<TextInfo>().ToArray();

            foreach (TextInfo Txt in Tis)
                TextInfos.Add(Txt.ID, Txt);

            foreach (CacheData Data in Dts)
            {
                Data.Field7 = GetText(Data.TextID_1);
                Data.Field8 = GetText(Data.TextID_2);
                Datas.Add(Data.CacheID, Data);
            }

            foreach (CacheTemplate Tm in Cte)
            {
                Tm.Field40 = GetText(Tm.TextID);
                Templates.Add(Tm.CacheID, Tm);
            }

            Log.Success("LoadCache", "Loaded : " + Datas.Count + Templates.Count + " Caches");
        }

        static public CacheUpdate BuildCache(uint CacheID, long CacheType, ISerializablePacket Packet)
        {
            CacheUpdate Data = new CacheUpdate();
            Data.CacheType = CacheType;
            Data.CacheID = CacheID;
            Data.CacheDatas = new List<ISerializablePacket>() { Packet };
            return Data;
        }
    }
}
