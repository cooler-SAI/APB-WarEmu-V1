using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
    [Serializable]
    public class WorldInfo
    {
        public int RpcID = 0;
        public int Port = 0;
        public int Ip = 0;

        public DBWorldInfo _Info;

        public WorldInfo(DBWorldInfo Info)
        {
            _Info = Info;
        }
    }
}
