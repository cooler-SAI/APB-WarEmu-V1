using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace MapServer
{
    [aConfigAttributes("Configs/MapServer.xml")]
    public class MapConfig : aConfig
    {
        public RpcClientConfig ClientInfo = new RpcClientConfig("127.0.0.1","127.0.0.1",6499);
        public MapServerInfo ServerInfo = new MapServerInfo();
        public LogInfo LogLevel = new LogInfo();
    }
}
