using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace WorldServer
{
    [aConfigAttributes("Configs/WorldServerConfig.xml")]
    public class WorldServerConfig : aConfig
    {
        public LogInfo LogLevel = new LogInfo();
        public RpcClientConfig FileServerRpc = new RpcClientConfig("127.0.0.1", "127.0.0.1", 2100);
        public RpcClientConfig CharacterServerRpc = new RpcClientConfig("127.0.0.1", "127.0.0.1", 2000);
        public string WorldName = "APS";
        public int WorldID = 1;
        public string WorldServerIp = "127.0.0.1";
        public int WorldServerPort = 2107;
    }
}
