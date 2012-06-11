using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace LobbyServer
{
    [aConfigAttributes("Configs/LobbyServerConfig.xml")]
    public class LobbyServerConfig : aConfig
    {
        public LogInfo LogLevel = new LogInfo();
        public RpcClientConfig FileServerRpc = new RpcClientConfig("127.0.0.1", "127.0.0.1", 2100);
        public RpcClientConfig CharacterServerRpc = new RpcClientConfig("127.0.0.1", "127.0.0.1", 2000);
        public int ClientServerPort = 2106;
        public string ClientVersion = "1.4.1";
        public int ClientBuild = 555239;
    }
}
