using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

using FrameWork;

namespace Common
{
    [Serializable]
    public class MapServerInfo
    {
        public string MapIP = "127.0.0.1";
        public int MapPort = 6950;
        public string MapAdress
        {
            get
            {
                return MapIP + ":" + MapPort;
            }
        }
        public int PlayerCount = 0;

        #region Remoting

        [XmlIgnore]
        public RpcClientInfo RpcInfo;

        public T GetObject<T>() where T : RpcObject
        {
            return RpcServer.GetObject<T>(RpcInfo);
        }

        #endregion
    }
}
